
#include "TheGame.h"
#include "stdafx.h"
#include "EventReceiver.h"
#include "Settings.h"
#include "ScreenQuad.h"
#include "ShadersSM20.h"
#include "OffsetManager.h"
#include "hk.h"

// static stuff
TheGame* TheGame::theGame = 0;

TheGame* TheGame::getInstance()
{
    dprintf(MY_DEBUG_INFO, "TheGame::getInstance(): theGame: %p\n", theGame);
    if (theGame == 0)
    {
        theGame = new TheGame();
    }
    return theGame;
}

void TheGame::destroy()
{
    dprintf(MY_DEBUG_IMPINFO, "TheGame::destroy(): theGame: %p\n", theGame);
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
      offsetManager(OffsetManager::getInstance()),
      terminate(true),
      windowId(0),
      lastScreenSize(),
      failed_render(0),
      tick(0)
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

        driver = device->getVideoDriver();
        smgr = device->getSceneManager();
        env = device->getGUIEnvironment();
        fix_camera = smgr->addCameraSceneNode();
        fps_camera = smgr->addCameraSceneNodeFPS(0, 100.f, 0.5f);
        camera = fps_camera;
        smgr->setActiveCamera(camera);
        lastScreenSize = getScreenSize();

        eventReceiver = new EventReceiver();
        shaders = new ShadersSM20();
        hk::initialize();
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
    Settings::destroy();
    OffsetManager::destroy();
    hk::finalize();
}

void TheGame::readSettings(irr::SIrrlichtCreationParameters& params)
{
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
    camera->setPosition(irr::core::vector3df(0.f, 10.f, 20.f));
    camera->setTarget(irr::core::vector3df(0.f, 2.f, 0.f));

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
    last_tick = tick = device->getTimer()->getTime();

    ScreenQuad testQuad(driver, irr::core::position2di(10, 10), irr::core::dimension2du(400, 300));
    // ScreenQuad testQuad(driver, irr::core::position2di(0, 0), driver->getScreenSize());

    testQuad.getMaterial().setTexture(0, driver->getTexture("data/bg/dakar_bg1.jpg"));
    testQuad.getMaterial().MaterialType = shaders->quad2d;
    //testQuad.getMaterial().setTexture(0, driver->getTexture("data/menu_textures/bg_frame_main_1280.png"));
    testQuad.rotate(30.0f, irr::core::position2di(150, 150));

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
            offsetManager->update(camera->getPosition());

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
            smgr->drawAll();
            env->drawAll();
            testQuad.render();

            driver->endScene();

            int fps = driver->getFPS();

            last_tick = tick;
        } // if (window->active)
        else
        {
            device->sleep(100);
            last_tick = device->getTimer()->getTime();
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
