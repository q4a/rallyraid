
#include "EventReceiver.h"
#include "TheGame.h"
#include "KeyConfig.h"

#include "stdafx.h"

#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>
#include <OISJoyStick.h>

const Ogre::String EventReceiver::keyMappingFilename = "data/Dakar2012_keymapping.cfg";

EventReceiver::EventReceiver()
    : inputManager(0),
      mouse(0),
      keyboard(0),
      joystick(0),
      deadZone(0.01f),
      test_kc(0),
      keyMap()
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

    KeyProperty kp;
    kp.primaryKeyConfig = kp.secondaryKeyConfig = 0;
    kp.continous = true;
    keyMap["accelerate"] = kp;
    keyMap["brake"] = kp;
    keyMap["left"] = kp;
    keyMap["right"] = kp;
    keyMap["clutch"] = kp;

    loadKeyMapping();
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

void EventReceiver::loadKeyMapping()
{
    // Load resource paths from config file
    for (keyMap_t::iterator it = keyMap.begin(); it != keyMap.end(); it++)
    {
        delete it->second.primaryKeyConfig;
        delete it->second.secondaryKeyConfig;
        it->second.primaryKeyConfig = it->second.secondaryKeyConfig = 0;
    }

    Ogre::ConfigFile cf;
    cf.load(keyMappingFilename);

    dprintf(MY_DEBUG_NOTE, "Read keymapping file:\n");
    // Go through all sections & settings in the file
    Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

    Ogre::String secName, keyName, valueName;
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
        Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
        for (Ogre::ConfigFile::SettingsMultiMap::iterator i = settings->begin(); i != settings->end(); ++i)
        {
            keyName = i->first;
            valueName = i->second;

            if (keyName == "primary_type")
            {
                primary = true;
                p_type = Ogre::StringConverter::parseUnsignedInt(valueName, 0);
            } else
            if (keyName == "secondary_type")
            {
                secondary = true;
                s_type = Ogre::StringConverter::parseUnsignedInt(valueName, 0);
            } else
            if (keyName == "primary_key")
            {
                primary = true;
                p_key = Ogre::StringConverter::parseUnsignedInt(valueName, (unsigned int)-1);
            } else
            if (keyName == "secondary_key")
            {
                secondary = true;
                s_key = Ogre::StringConverter::parseUnsignedInt(valueName, (unsigned int)-1);
            } else
            if (keyName == "primary_key2")
            {
                primary = true;
                p_key2 = Ogre::StringConverter::parseUnsignedInt(valueName, (unsigned int)-1);
            } else
            if (keyName == "secondary_key2")
            {
                secondary = true;
                s_key2 = Ogre::StringConverter::parseUnsignedInt(valueName, (unsigned int)-1);
            } else
            if (keyName == "primary_from")
            {
                primary = true;
                p_from = Ogre::StringConverter::parseUnsignedInt(valueName, 0);
            } else
            if (keyName == "secondary_from")
            {
                secondary = true;
                s_from = Ogre::StringConverter::parseUnsignedInt(valueName, 0);
            } else
            if (keyName == "primary_to")
            {
                primary = true;
                p_to = Ogre::StringConverter::parseUnsignedInt(valueName, 0);
            } else
            if (keyName == "secondary_to")
            {
                secondary = true;
                s_to = Ogre::StringConverter::parseUnsignedInt(valueName, 0);
            }
        }
        keyMap_t::iterator kit = keyMap.find(secName);
        if (kit != keyMap.end())
        {

        }
    }
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

