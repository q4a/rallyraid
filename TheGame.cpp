
#include "TheGame.h"
#include "stdafx.h"
#include "EventReceiver.h"

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
    : root(0), window(0), smgr(0), camera(0),
      eventReceiver(0),
      terminate(true)
{
    dprintf(MY_DEBUG_INFO, "TheGame::TheGame(): this: %p\n", this);
    theGame = this;

    const Ogre::String pluginFileName = "data/Dakar2012_plugin.cfg";
    const Ogre::String configFileName = "data/Dakar2012.cfg";
    const Ogre::String logFileName = "log/Dakar2012.log";

    root = new Ogre::Root(pluginFileName, configFileName, logFileName);

    if (!root->restoreConfig())
    {
        dprintf(MY_DEBUG_WARNING, "No default config file found! Show configuration window!\n");
        if (root->showConfigDialog())
        {
            terminate = false;
        }
        else
        {
            dprintf(MY_DEBUG_ERROR, "User canceled the configuration window. Program will exit\n");
        }
    }
    else
    {
        terminate = false;
    }

    if (!terminate)
    {
        setupResources();
        window = root->initialise(true, "Dakar 2012 Game");
        smgr = root->createSceneManager(Ogre::ST_GENERIC);
        camera = smgr->createCamera("DefaultCam");
        camera->setNearClipDistance(0.5);
        // no use for stencil shadow: camera->setFarClipDistance(12500.0);

        Ogre::Viewport* vp = window->addViewport(camera);
        vp->setBackgroundColour(Ogre::ColourValue(0.3f, 0.3f, 0.3f));
        camera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));

        Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

        eventReceiver = new EventReceiver();
        windowResized(window);

        Ogre::WindowEventUtilities::addWindowEventListener(window, this);
        root->addFrameListener(this);
    }
}

TheGame::~TheGame()
{
    dprintf(MY_DEBUG_IMPINFO, "TheGame::~TheGame(): this: %p\n", this);

    Ogre::WindowEventUtilities::addWindowEventListener(window, this);

    if (eventReceiver)
    {
        delete eventReceiver;
        eventReceiver = 0;
    }

    window = 0;
    smgr = 0;
    camera = 0;

    if (root)
    {
        delete root;
        root = 0;
    }

}

void TheGame::setupResources()
{
    // Load resource paths from config file
    Ogre::ConfigFile cf;
    cf.load("data/Dakar2012_resources.cfg");

    dprintf(MY_DEBUG_NOTE, "Read resource file:\n");
    // Go through all sections & settings in the file
    Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

    Ogre::String secName, typeName, archName;
    while (seci.hasMoreElements())
    {
        secName = seci.peekNextKey();
        dprintf(MY_DEBUG_NOTE, "\tKey: %s\n", secName.c_str());
        Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
        Ogre::ConfigFile::SettingsMultiMap::iterator i;
        for (i = settings->begin(); i != settings->end(); ++i)
        {
            typeName = i->first;
            archName = i->second;
            dprintf(MY_DEBUG_NOTE, "\t\ttype: %s, arch: %s\n", typeName.c_str(), archName.c_str());
            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
                archName, typeName, secName);
        }
    }
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
    camera->setPosition(0, 10, 20);
    camera->lookAt(0, 2, 0);

    smgr->setAmbientLight(Ogre::ColourValue(0.3f, 0.3f, 0.3f));
    smgr->setShadowTechnique(/*Ogre::SHADOWTYPE_STENCIL_ADDITIVE*//*Ogre::SHADOWTYPE_STENCIL_MODULATIVE*/Ogre::SHADOWTYPE_TEXTURE_MODULATIVE);
    smgr->setShadowTextureSize(2048);
    smgr->setShadowColour(Ogre::ColourValue(0.2f, 0.2f, 0.2f));

    Ogre::Entity* ent = smgr->createEntity("data/objects/meshes/vw3.mesh");
    ent->setMaterialName("car/vw3");
    ent->setCastShadows(true);
    Ogre::SceneNode* node = smgr->getRootSceneNode()->createChildSceneNode();
    node->attachObject(ent);
    //node->yaw(Ogre::Degree(45));

    Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0);
    Ogre::MeshManager::getSingleton().createPlane("ground",
        Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        plane, 1500, 1500, 20, 20, true, 1, 5, 5, Ogre::Vector3::UNIT_Z);
    Ogre::Entity* entG = smgr->createEntity("valami", "ground");
    smgr->getRootSceneNode()->createChildSceneNode()->attachObject(entG);
    entG->setMaterialName("test/rockwall");
    entG->setCastShadows(false);

    //node->yaw(Ogre::Degree(45), Ogre::Node::TS_WORLD);
    //node->setOrientation(1, 0, Ogre::Math::HALF_PI/*Ogre::Degree(90).valueRadians()*/, 0);
    //Ogre::Quaternion myq = node->getOrientation();
    //printf("w: %f, x: %f, y: %f, z: %f\n", myq.w, myq.x, myq.y, myq.z);
    //exit(0);

    Ogre::Light* dLight = smgr->createLight("dLight");
    dLight->setType(Ogre::Light::LT_DIRECTIONAL);
    dLight->setDiffuseColour(Ogre::ColourValue(0.9f, 0.9f, 0.9f));
    dLight->setSpecularColour(Ogre::ColourValue(0.4f, 0.4f, 0.4f));
    dLight->setDirection(0, -1, 1);

    camera->yaw(Ogre::Degree(30));
    /* end test */

    root->startRendering();

    dprintf(MY_DEBUG_INFO, "TheGame::loop() end\n");
}

// Ogre::WindowEventListener
//Adjust mouse clipping area
void TheGame::windowResized(Ogre::RenderWindow* rw)
{
    if (rw == window)
    {
        unsigned int width, height, depth;
        int left, top;
        rw->getMetrics(width, height, depth, left, top);

        if (eventReceiver)
        {
            eventReceiver->updateWindow(width, height);
        }

        dprintf(MY_DEBUG_NOTE, "Main window resized: width: %u, heigth: %u, depth: %u, left: %d, top: %d\n",
            width, height, depth, left, top);
    }
}

//Unattach OIS before window shutdown (very important under Linux)
bool TheGame::windowClosing(Ogre::RenderWindow* rw)
{
    if (rw == window)
    {
        dprintf(MY_DEBUG_INFO, "Main window closing\n");
    }
    return true;
}

void TheGame::windowClosed(Ogre::RenderWindow* rw)
{
    if (rw == window)
    {
        dprintf(MY_DEBUG_INFO, "Main window closed\n");

        if (eventReceiver)
        {
            delete eventReceiver;
            eventReceiver = 0;
        }

        terminate = true;
    }
}

void TheGame::windowFocusChange(Ogre::RenderWindow *rw)
{
    if (rw == window)
    {
        dprintf(MY_DEBUG_INFO, "Main window focus change\n");
    }
}

// Ogre::FrameRenderListener
bool TheGame::frameStarted(const Ogre::FrameEvent &evt)
{
    if (terminate) return false;
    return !terminate;
}

bool TheGame::frameRenderingQueued(const Ogre::FrameEvent &evt)
{
    if (terminate) return false;

    if (eventReceiver)
    {
        eventReceiver->checkEvents();
    }

    return !terminate;
}
