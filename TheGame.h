#ifndef THEGAME_H
#define THEGAME_H

#include "stdafx.h"

class EventReceiver;
class Shaders;
class OffsetManager;
class TheEarth;

class TheGame
{
public:
    static void initialize();
    static void finalize();
    static TheGame* getInstance() {return theGame;}

public:
    void loop();

    irr::IrrlichtDevice*            getDevice();
    irr::video::IVideoDriver*       getDriver();
    irr::scene::ISceneManager*      getSmgr();
    irr::scene::ICameraSceneNode*   getCamera();
    size_t                          getWindowId();
    irr::gui::IGUIEnvironment*      getEnv();
    const irr::core::dimension2du&  getScreenSize();
    unsigned int                    getTick();
    void                            switchCamera();
    Shaders*                        getShaders();

    bool                            getPhysicsOngoing() {return physicsOngoing;}
    void                            setPhysicsOngoing(bool newValue) {physicsOngoing = newValue;}

private:
    TheGame();
    ~TheGame();

protected:

private:
    static TheGame*                 theGame;

    void readSettings(irr::SIrrlichtCreationParameters& params);

private:
    // members
    irr::IrrlichtDevice*            device;
    irr::video::IVideoDriver*       driver;
    irr::scene::ISceneManager*      smgr;
    irr::scene::ICameraSceneNode*   camera;
    irr::scene::ICameraSceneNode*   fix_camera;
    irr::scene::ICameraSceneNode*   fps_camera;
    irr::gui::IGUIEnvironment*      env;

    EventReceiver*                  eventReceiver;
    Shaders*                        shaders;
    OffsetManager*                  offsetManager;
    TheEarth*                       earth;

    bool                            terminate;
    size_t                          windowId;
    irr::core::dimension2du         lastScreenSize;
    unsigned int                    failed_render;
    unsigned int                    tick;
    unsigned int                    lastPhysTick;
    unsigned int                    lastSlowTick;
    bool                            physicsOngoing;

    irr::gui::IGUIStaticText*       testText;
};

inline irr::IrrlichtDevice* TheGame::getDevice()
{
    return device;
}

inline irr::video::IVideoDriver* TheGame::getDriver()
{
    return driver;
}

inline irr::scene::ISceneManager* TheGame::getSmgr()
{
    return smgr;
}

inline irr::scene::ICameraSceneNode* TheGame::getCamera()
{
    return camera;
}

inline size_t TheGame::getWindowId()
{
    return windowId;
}

inline irr::gui::IGUIEnvironment* TheGame::getEnv()
{
    return env;
}

inline const irr::core::dimension2du& TheGame::getScreenSize()
{
    return driver->getScreenSize();
}

inline unsigned int TheGame::getTick()
{
    return tick;
}

inline Shaders* TheGame::getShaders()
{
    return shaders;
}

#endif // THEGAME_H
