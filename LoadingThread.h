#ifndef LOADINGTHREAD_H
#define LOADINGTHREAD_H

#include "stdafx.h"
#include "MyThread.h"
#include <irrlicht.h>

class ScreenQuad;

class LoadingThread : public MyThread
{
public:
    static void initialize();
    static void finalize();
    static LoadingThread* getInstance() {return loadingThread;}

public:
    // starts the loading screen thread
    void startLoading();

    // stops the loading screen thread
    void endLoading();
    
    // returns if the loading is active
    bool getLoading(); // inline

private:
    LoadingThread();
    virtual ~LoadingThread();

    virtual void run();

private:
    static LoadingThread*   loadingThread;

private:
    volatile bool           loading;
    volatile bool           running;
    ScreenQuad*             bgQuad;
    irr::gui::IGUIStaticText* staticTextLoading;
};

inline bool LoadingThread::getLoading()
{
    return loading;
}

#endif // LOADINGTHREAD_H
