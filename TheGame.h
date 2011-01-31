#ifndef THEGAME_H
#define THEGAME_H

#include "stdafx.h"

class EventReceiver;

class TheGame
{
public:
    static TheGame* getInstance();
    static void destroy();

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

    bool                            terminate;
    size_t                          windowId;
    irr::core::dimension2du         lastScreenSize;
    unsigned int                    failed_render;
    unsigned int                    tick;
    unsigned int                    last_tick;
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

#endif // THEGAME_H
