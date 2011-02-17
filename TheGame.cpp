
#include "TheGame.h"
#include "stdafx.h"
#include "EventReceiver.h"
#include "Settings.h"
#include "ScreenQuad.h"
#include "ShadersSM20.h"
#include "OffsetManager.h"
#include "OffsetObject.h"
#include "hk.h"
#include "ObjectPoolManager.h"
#include "TheEarth.h"

// static stuff
TheGame* TheGame::theGame = 0;

void TheGame::initialize()
{
    dprintf(MY_DEBUG_INFO, "TheGame::initialize(): theGame: %p\n", theGame);
    if (theGame == 0)
    {
        theGame = new TheGame();
    }
}

void TheGame::finalize()
{
    dprintf(MY_DEBUG_IMPINFO, "TheGame::finalize(): theGame: %p\n", theGame);
    //assert(0);
    if (theGame)
    {
        delete theGame;
        theGame = 0;
    }
}


// non-static stuff    
TheGame::TheGame()
    : device(0), driver(0), smgr(0), camera(0), fix_camera(0), fps_camera(0),
      env(0),
      eventReceiver(0),
      shaders(0),
      offsetManager(0),
      earth(0),
      terminate(true),
      windowId(0),
      lastScreenSize(),
      failed_render(0),
      tick(0),
      physicsOngoing(false),
      testText(0)
{
    dprintf(MY_DEBUG_INFO, "TheGame::TheGame(): this: %p\n", this);
    theGame = this;

    irr::SIrrlichtCreationParameters params;
    params.DriverType = irr::video::EDT_DIRECT3D9 /*irr::video::EDT_OPENGL*/;

    readSettings(params);

    device = irr::createDeviceEx(params);

    windowId = (size_t)params.WindowId;

    if (device==0)
    {
         dprintf(MY_DEBUG_ERROR, "unable to create device\n");
         terminate = true;
    }
    else
    {
        terminate = false;

        device->setWindowCaption(L"Dakar 2012");

        dprintf(MY_DEBUG_NOTE, "Initialize Irrlicht members\n");
        driver = device->getVideoDriver();
        smgr = device->getSceneManager();
        env = device->getGUIEnvironment();
        fix_camera = smgr->addCameraSceneNode();
        fps_camera = smgr->addCameraSceneNodeFPS(0, 100.f, 0.5f);
        camera = fps_camera;
        smgr->setActiveCamera(camera);
        lastScreenSize = getScreenSize();

        dprintf(MY_DEBUG_NOTE, "Initialize event receiver\n");
        eventReceiver = new EventReceiver();
        dprintf(MY_DEBUG_NOTE, "Initialize shaders\n");
        shaders = new ShadersSM20();
        dprintf(MY_DEBUG_NOTE, "Initialize Havok\n");
        hk::initialize();
        dprintf(MY_DEBUG_NOTE, "Initialize offset manager\n");
        OffsetManager::initialize();
        offsetManager = OffsetManager::getInstance();
        dprintf(MY_DEBUG_NOTE, "Initialize object pool manager\n");
        ObjectPoolManager::initialize();
        dprintf(MY_DEBUG_NOTE, "Initialize earth\n");
        TheEarth::initialize();
        earth = TheEarth::getInstance();

        testText = env->addStaticText(L"", irr::core::recti(10, 10, 790, 26), false, true, 0, -1, true);
    }
}

TheGame::~TheGame()
{
    dprintf(MY_DEBUG_IMPINFO, "TheGame::~TheGame(): this: %p\n", this);

    if (eventReceiver)
    {
        delete eventReceiver;
        eventReceiver = 0;
    }
    if (shaders)
    {
        delete shaders;
        shaders = 0;
    }

    windowId = 0;
    smgr = 0;
    fps_camera = fix_camera = camera = 0;
    offsetManager = 0;
    earth = 0;

    TheEarth::finalize();
    OffsetManager::finalize();
    ObjectPoolManager::finalize();
    Settings::finalize();
    if (driver)
    {
        driver->removeAllTextures();
        driver->removeAllHardwareBuffers();
        driver = 0;
    }
    if (device)
    {
        device->drop();
        device = 0;
    }
    hk::finalize();
}

void TheGame::readSettings(irr::SIrrlichtCreationParameters& params)
{
    Settings::initialize();
    Settings::getInstance()->read();
}

void TheGame::loop()
{
    dprintf(MY_DEBUG_INFO, "TheGame::loop() begin, terminate: %s\n", terminate?"true":"false");

    if (terminate)
    {
        dprintf(MY_DEBUG_INFO, "TheGame::loop() end: not initialized\n");
        return;
    }

    /* test */
    //irr::core::vector3df initialPos(4190208.f, 1000.f, -6414336.f);
    irr::core::vector3df initialPos(4190225.f, 100.f, -6411350.f);
    irr::core::vector3df initialDir(20.f, -20.f, 20.f);
    camera->setPosition(initialPos);
    camera->setTarget(camera->getPosition()+initialDir);
    earth->createFirst(initialPos, irr::core::vector3df(1.f, 2.f, 1.f));

    smgr->setAmbientLight(irr::video::SColorf(0.3f, 0.3f, 0.3f));
    smgr->setShadowColor(irr::video::SColor(255, 50, 50, 50));

    /*
    Ogre::Light* dLight = smgr->createLight("dLight");
    dLight->setType(Ogre::Light::LT_DIRECTIONAL);
    dLight->setDiffuseColour(Ogre::ColourValue(0.9f, 0.9f, 0.9f));
    dLight->setSpecularColour(Ogre::ColourValue(0.4f, 0.4f, 0.4f));
    dLight->setDirection(0, -1, 1);
    */
    device->run();
    lastPhysTick = lastSlowTick = tick = device->getTimer()->getTime();
    lastPhysTick--;
    const unsigned int step_ms = 1000 / Settings::getInstance()->targetFps;
    const unsigned int slowStep_ms = 100;
    const float step_sec = 1.f / (float)Settings::getInstance()->targetFps;
    unsigned int physUpdate;

    OffsetObject* cameraOObject = new OffsetObject(camera, true);
    cameraOObject->addToManager();
    offsetManager->update(camera->getPosition());

    /*ScreenQuad testQuad(driver, irr::core::position2di(10, 10), irr::core::dimension2du(400, 300));
    // ScreenQuad testQuad(driver, irr::core::position2di(0, 0), driver->getScreenSize());

    testQuad.getMaterial().setTexture(0, driver->getTexture("data/bg/dakar_bg1.jpg"));
    testQuad.getMaterial().MaterialType = shaders->materialMap["quad2d"];
    //testQuad.getMaterial().setTexture(0, driver->getTexture("data/menu_textures/bg_frame_main_1280.png"));
    testQuad.rotate(30.0f, irr::core::position2di(150, 150));*/

    OffsetObject* car = ObjectPoolManager::getInstance()->getObject("vw3", initialPos+initialDir);
    printf("car off: %f, %f (%f, %f)\n", offsetManager->getOffset().X, offsetManager->getOffset().Z,
        car->getNode()->getPosition().X, car->getNode()->getPosition().Z);

    while (device->run() && !terminate)
    {
        if (device->isWindowActive())
        {
            tick = device->getTimer()->getTime();

            // -------------------------------
            //         update events
            // -------------------------------
            if (eventReceiver)
            {
                eventReceiver->checkEvents();
            }
            /*
            if (eventReceiver)
            {
                eventReceiver->updateWindow(width, height);
            }
            */

            // -------------------------------
            //         update physics
            // -------------------------------
            //camera->setTarget(camera->getPosition()+initialDir);
            //printf("off: %f, %f (%f, %f)\n", offsetManager->getOffset().X, offsetManager->getOffset().Z,
            //    camera->getPosition().X, camera->getPosition().Z);
            offsetManager->update(offsetManager->getOffset()+camera->getPosition());
            //printf("off: %f, %f (%f, %f)\n", offsetManager->getOffset().X, offsetManager->getOffset().Z,
            //    camera->getPosition().X, camera->getPosition().Z);
            //assert(0);
            //car->getBody()->activate();

            bool physUpdateDone = false;
            physUpdate = (tick - lastPhysTick) / step_ms;
            if (physUpdate > 10) physUpdate = 10;
            while (lastPhysTick < tick && physUpdate)
            {
                if (physicsOngoing)
                {
                    hk::hkWorld->stepDeltaTime(step_sec);
                }
                lastPhysTick += 16;
                physUpdate--;
                physUpdateDone = true;
            }
            /*
            update dynamic object position
            */
            if (physUpdateDone)
            {
                OffsetObject::updateDynamicToPhys();
            }

            if (lastSlowTick < tick || tick < slowStep_ms)
            {
                lastSlowTick += slowStep_ms;
                irr::core::stringw str = L"Pos: ";
                str += (int)camera->getPosition().X;
                str += L", ";
                str += (int)camera->getPosition().Y;
                str += L", ";
                str += (int)camera->getPosition().Z;
                str += L" - Offset: ";
                str += (int)offsetManager->getOffset().X;
                str += L", ";
                str += (int)offsetManager->getOffset().Z;
                testText->setText(str.c_str());

                earth->update(offsetManager->getOffset()+camera->getPosition(), initialDir);
            }

            // -------------------------------
            //         update graphics
            // -------------------------------
            if (!driver->beginScene(true, true, irr::video::SColor(0,192,192,192)))
            {
                driver->endScene();
                driver->OnResize(lastScreenSize);
                failed_render++;
                if (failed_render > 5)
                {
                    dprintf(MY_DEBUG_ERROR, "failed to render 5 times, quit\n");
                    terminate = true;
                }
                continue;
            }
            failed_render = 0;

            driver->setRenderTarget(0, true, true, irr::video::SColor(0, 0, 0, 255));
            //printf("1\n");
            earth->registerVisual();
            //printf("2\n");
            smgr->drawAll();
            //printf("3\n");
            env->drawAll();
            //printf("4\n");
            //testQuad.render();

            driver->endScene();
            //printf("5\n");

            int fps = driver->getFPS();

            //last_tick = tick;
        } // if (window->active)
        else
        {
            device->sleep(100);
            lastPhysTick = lastSlowTick = device->getTimer()->getTime();
        } // if (!window->active)
    } // wile (run && !terminate)

    dprintf(MY_DEBUG_INFO, "TheGame::loop() end\n");
}

void TheGame::switchCamera()
{
    irr::core::vector3df pos = camera->getPosition();
    irr::core::vector3df tar = camera->getTarget();
    camera = camera == fix_camera ? fix_camera : fps_camera;
    smgr->setActiveCamera(camera);
    camera->setPosition(pos);
    camera->setTarget(tar);
}
