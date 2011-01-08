
#include "EventReceiver.h"
#include "TheGame.h"
#include "KeyConfig.h"

#include "stdafx.h"

#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>
#include <OISJoyStick.h>

EventReceiver::EventReceiver()
    : inputManager(0),
      mouse(0),
      keyboard(0),
      joystick(0),
      deadZone(0.01f),
      test_kc(0)
{
    OIS::ParamList pl;
    size_t windowHnd = 0;
    std::ostringstream windowHndStr;

    TheGame::getInstance()->getWindow()->getCustomAttribute("WINDOW", &windowHnd);
    windowHndStr << windowHnd;
    pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

    inputManager = OIS::InputManager::createInputSystem(pl);

    keyboard = static_cast<OIS::Keyboard*>(inputManager->createInputObject(OIS::OISKeyboard, false));
    mouse = static_cast<OIS::Mouse*>(inputManager->createInputObject(OIS::OISMouse, false));
    joystick = static_cast<OIS::JoyStick*>(inputManager->createInputObject(OIS::OISJoyStick, false));

    recalibrate();
}

EventReceiver::~EventReceiver()
{
    if (inputManager)
    {
        inputManager->destroyInputObject(mouse);
        mouse = 0;
        inputManager->destroyInputObject(keyboard);
        keyboard = 0;
        inputManager->destroyInputObject(joystick);
        joystick = 0;
 
        OIS::InputManager::destroyInputSystem(inputManager);
        inputManager = 0;
    }
}

void EventReceiver::recalibrate()
{
    joystick->capture();
    //centralJoystickState = joystick->getJoyStickState();
    KeyConfig::recalibrate(joystick);
}

void EventReceiver::checkEvents()
{
    //Need to capture/update each device
    keyboard->capture();
    mouse->capture();
    joystick->capture();

    const OIS::JoyStickState joystickState = joystick->getJoyStickState();
    
    if (test_kc == 0)
    {
        test_kc = KeyConfig::getKeyConfig(keyboard, joystickState, deadZone, false);
        if (test_kc)
        {
            dprintf(MY_DEBUG_INFO, "test_kc assigned: %d\n", test_kc->key);
            //assert(0);
        }
    }
    else
    {
        float pressed = test_kc->getPercentage(keyboard, joystickState);
        if (pressed > deadZone)
        {
            dprintf(MY_DEBUG_INFO, "test_kc pressed: %f\n", pressed);
        }
    }
    if (keyboard->isKeyDown(OIS::KC_ESCAPE))
    {
        delete test_kc;
        test_kc = 0;
    }
    
    //printf("%s\n", keyboard->isKeyDown(OIS::KC_K)?"true":"false");
    /*
    printf("Axes: %lu\n", joystickState.mAxes.size());
    for (unsigned int i = 0; i < joystickState.mAxes.size(); i++)
    {
        printf("\t%u: %d (%d)\n", i, joystickState.mAxes[i].abs, joystickState.mAxes[i].rel);
    }
    printf("Sliders:\n");
    for (unsigned int i = 0; i < 4; i++)
    {
        printf("\t%u: %d, %d\n", i, joystickState.mSliders[i].abX, joystickState.mSliders[i].abY);
    }
    */
}

void EventReceiver::updateWindow(unsigned int width, unsigned int height)
{
    if (mouse)
    {
        const OIS::MouseState &ms = mouse->getMouseState();
        ms.width = width;
        ms.height = height;
    }
}
