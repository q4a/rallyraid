#ifndef THEGAME_H
#define THEGAME_H

#include <OgreFrameListener.h>
#include <OgreWindowEventUtilities.h>

namespace Ogre
{
    class Root;
    class RenderWindow;
    class SceneManager;
    class Camera;
}
class EventReceiver;

class TheGame : public Ogre::FrameListener, public Ogre::WindowEventListener
{
public:
    static TheGame* getInstance();
    static void destroy();

public:
    void loop();

    Ogre::Root*         getRoot();
    Ogre::RenderWindow* getWindow();
    Ogre::SceneManager* getSceneManager();
    Ogre::Camera*       getCamera();

private:
    TheGame();
    ~TheGame();

protected:
    // Ogre::WindowEventListener
    //Adjust mouse clipping area
    virtual void windowResized(Ogre::RenderWindow* rw);
    //Unattach OIS before window shutdown (very important under Linux)
    virtual bool windowClosing(Ogre::RenderWindow* rw);
    virtual void windowClosed(Ogre::RenderWindow* rw);
    virtual void windowFocusChange(Ogre::RenderWindow *rw);

    // Ogre::FrameRenderListener
    virtual bool frameStarted(const Ogre::FrameEvent &evt);
    virtual bool frameRenderingQueued(const Ogre::FrameEvent &evt);
    //virtual bool 	frameEnded (const FrameEvent &evt)

private:
    static TheGame*     theGame;

    void setupResources();

private:
    // members
    Ogre::Root*             root;
    Ogre::RenderWindow*     window;
    Ogre::SceneManager*     smgr;
    Ogre::Camera*           camera;

    EventReceiver*          eventReceiver;

    bool                    terminate;
};

inline Ogre::Root* TheGame::getRoot()
{
    return root;
}

inline Ogre::RenderWindow* TheGame::getWindow()
{
    return window;
}

inline Ogre::SceneManager* TheGame::getSceneManager()
{
    return smgr;
}

inline Ogre::Camera* TheGame::getCamera()
{
    return camera;
}

#endif // THEGAME_H
