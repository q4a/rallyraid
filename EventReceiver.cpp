
#include "EventReceiver.h"
#include "TheGame.h"
#include "KeyConfig.h"
#include "ConfigFile.h"
#include "StringConverter.h"

#include "stdafx.h"

#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>
#include <OISJoyStick.h>

const std::string EventReceiver::keyMappingFilename = "data/Dakar2012_keymapping.cfg";

EventReceiver::EventReceiver()
    : inputManager(0),
      mouse(0),
      keyboard(0),
      joystick(0),
      deadZone(0.01f),
      test_kc(0),
      keyNameMap()
{
    inputManager = OIS::InputManager::createInputSystem(TheGame::getInstance()->getWindowId());

    keyboard = static_cast<OIS::Keyboard*>(inputManager->createInputObject(OIS::OISKeyboard, false));
    //mouse = static_cast<OIS::Mouse*>(inputManager->createInputObject(OIS::OISMouse, false));
    joystick = static_cast<OIS::JoyStick*>(inputManager->createInputObject(OIS::OISJoyStick, false));

    recalibrate();

    KeyProperty kp;
    kp.primaryKeyConfig = kp.secondaryKeyConfig = 0;
    kp.continous = true;
    keyNameMap["accelerate"] = ACCELERATE;
    keyMap[ACCELERATE] = kp;
    keyNameMap["brake"] = BRAKE;
    keyMap[BRAKE] = kp;
    keyNameMap["left"] = LEFT;
    keyMap[LEFT] = kp;
    keyNameMap["right"] = RIGHT;
    keyMap[RIGHT] = kp;
    keyNameMap["clutch"] = CLUTCH;
    keyMap[CLUTCH] = kp;

    loadKeyMapping();
}

EventReceiver::~EventReceiver()
{
    clearKeyMapping();

    if (inputManager)
    {
        //inputManager->destroyInputObject(mouse);
        //mouse = 0;
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

void EventReceiver::updateWindow(unsigned int width, unsigned int height)
{
    if (mouse)
    {
        const OIS::MouseState &ms = mouse->getMouseState();
        ms.width = width;
        ms.height = height;
    }
}

void EventReceiver::saveKeyMapping()
{
}

void EventReceiver::clearKeyMapping()
{
    for (unsigned int kn = ACCELERATE; kn < NUMBER_OF_KEYNAMES; kn++)
    {
        if (keyMap[kn].primaryKeyConfig)
        {
            delete keyMap[kn].primaryKeyConfig;
        }
        if (keyMap[kn].secondaryKeyConfig)
        {
            delete keyMap[kn].secondaryKeyConfig;
        }
        keyMap[kn].primaryKeyConfig = keyMap[kn].secondaryKeyConfig = 0;
    }
}

void EventReceiver::loadKeyMapping()
{
    // Load resource paths from config file
    clearKeyMapping();

    ConfigFile cf;
    cf.load(keyMappingFilename);

    dprintf(MY_DEBUG_NOTE, "Read keymapping file:\n");
    // Go through all sections & settings in the file
    ConfigFile::SectionIterator seci = cf.getSectionIterator();

    std::string secName, keyName, valueName;
    while (seci.hasMoreElements())
    {
        bool primary = false;
        bool secondary = false;
        unsigned int p_type = 0;
        unsigned int p_key = (unsigned int)-1;
        unsigned int p_key2 = (unsigned int)-1;
        int p_from = 0;
        int p_to = 0;
        unsigned int s_type = 0;
        unsigned int s_key = (unsigned int)-1;
        unsigned int s_key2 = (unsigned int)-1;
        int s_from = 0;
        int s_to = 0;

        secName = seci.peekNextKey();
        dprintf(MY_DEBUG_NOTE, "\tKey: %s\n", secName.c_str());
        ConfigFile::SettingsMultiMap *settings = seci.getNext();
        for (ConfigFile::SettingsMultiMap::iterator i = settings->begin(); i != settings->end(); ++i)
        {
            keyName = i->first;
            valueName = i->second;

            if (keyName == "primary_type")
            {
                primary = true;
                p_type = StringConverter::parseUnsignedInt(valueName, 0);
            } else
            if (keyName == "secondary_type")
            {
                secondary = true;
                s_type = StringConverter::parseUnsignedInt(valueName, 0);
            } else
            if (keyName == "primary_key")
            {
                primary = true;
                p_key = StringConverter::parseUnsignedInt(valueName, (unsigned int)-1);
            } else
            if (keyName == "secondary_key")
            {
                secondary = true;
                s_key = StringConverter::parseUnsignedInt(valueName, (unsigned int)-1);
            } else
            if (keyName == "primary_key2")
            {
                primary = true;
                p_key2 = StringConverter::parseUnsignedInt(valueName, (unsigned int)-1);
            } else
            if (keyName == "secondary_key2")
            {
                secondary = true;
                s_key2 = StringConverter::parseUnsignedInt(valueName, (unsigned int)-1);
            } else
            if (keyName == "primary_from")
            {
                primary = true;
                p_from = StringConverter::parseInt(valueName, 0);
            } else
            if (keyName == "secondary_from")
            {
                secondary = true;
                s_from = StringConverter::parseInt(valueName, 0);
            } else
            if (keyName == "primary_to")
            {
                primary = true;
                p_to = StringConverter::parseInt(valueName, 0);
            } else
            if (keyName == "secondary_to")
            {
                secondary = true;
                s_to = StringConverter::parseInt(valueName, 0);
            }
        }
        keyNameMap_t::iterator kit = keyNameMap.find(secName);
        if (kit != keyNameMap.end())
        {
            KeyName kn = kit->second;
            assert(kn < NUMBER_OF_KEYNAMES);
            if (primary)
            {
                keyMap[kn].primaryKeyConfig = KeyConfig::getKeyConfig((KeyConfig::type_t)p_type, p_key, p_key2, p_from, p_to);
            }
            if (secondary)
            {
                keyMap[kn].secondaryKeyConfig = KeyConfig::getKeyConfig((KeyConfig::type_t)s_type, s_key, s_key2, s_from, s_to);
            }
        }
    }
}

void EventReceiver::checkEvents()
{
    //Need to capture/update each device
    keyboard->capture();
    //mouse->capture();
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

#if 0
    // the real event check
    if ((keyMap[ACCELERATE].primaryKeyConfig && keyMap[ACCELERATE].primaryKeyConfig->getPercentage(keyboard, joystickState)) ||
        (keyMap[ACCELERATE].secondaryKeyConfig && keyMap[ACCELERATE].secondaryKeyConfig->getPercentage(keyboard, joystickState))
       )
    {
        dprintf(MY_DEBUG_NOTE, "accelerate pressed\n");
    }

    if ((keyMap[BRAKE].primaryKeyConfig && keyMap[BRAKE].primaryKeyConfig->getPercentage(keyboard, joystickState)) ||
        (keyMap[BRAKE].secondaryKeyConfig && keyMap[BRAKE].secondaryKeyConfig->getPercentage(keyboard, joystickState))
       )
    {
        dprintf(MY_DEBUG_NOTE, "brake pressed\n");
    }

    if ((keyMap[LEFT].primaryKeyConfig && keyMap[LEFT].primaryKeyConfig->getPercentage(keyboard, joystickState)) ||
        (keyMap[LEFT].secondaryKeyConfig && keyMap[LEFT].secondaryKeyConfig->getPercentage(keyboard, joystickState))
       )
    {
        dprintf(MY_DEBUG_NOTE, "left pressed\n");
    }

    if ((keyMap[RIGHT].primaryKeyConfig && keyMap[RIGHT].primaryKeyConfig->getPercentage(keyboard, joystickState)) ||
        (keyMap[RIGHT].secondaryKeyConfig && keyMap[RIGHT].secondaryKeyConfig->getPercentage(keyboard, joystickState))
       )
    {
        dprintf(MY_DEBUG_NOTE, "right pressed\n");
    }
#endif // 0 or 1
}

