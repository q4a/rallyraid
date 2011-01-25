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


    EventReceiver*                  eventReceiver;

    bool                            terminate;
    size_t                          windowId;
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

#endif // THEGAME_H
