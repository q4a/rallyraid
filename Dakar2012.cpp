// Dakar2012.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "TheGame.h"

#ifndef __linux__
#include <Windows.h>
#endif

#include <OgreException.h>

//int _tmain(int argc, _TCHAR* argv[])
int main()
{
    printf("hello world!\n");

    try
    {
        TheGame* theGame = TheGame::getInstance();

        theGame->loop();
    TheGame::destroy();
    } catch (Ogre::Exception& e)
    {
        printf("FATAL EXCEPTION: %s\n", e.getFullDescription().c_str());
#ifndef __linux__
        MessageBox(NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#endif
    }
	return 0;
}

