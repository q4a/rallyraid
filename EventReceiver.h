
#ifndef EVENTRECEIVER_H
#define EVENTRECEIVER_H

#include <map>
#include <vector>
#include <string>
#include <utility>
#include <OgreString.h>

namespace OIS
{
    class InputManager;
    class Mouse;
    class Keyboard;
    class JoyStick;
}

class KeyConfig;

class EventReceiver
{
public:
    //typedef std::map<std::string, unsigned int> strToKey_t;
    //typedef std::vector<std::string> keyToStr_t;
private:
    struct KeyProperty
    {
        bool            continous;
        KeyConfig*      primaryKeyConfig;
        KeyConfig*      secondaryKeyConfig;
    };
    typedef std::map<Ogre::String, KeyProperty> keyMap_t;

private:
    static const Ogre::String keyMappingFilename;

public:
    EventReceiver();
    ~EventReceiver();

    void recalibrate();
    void checkEvents();
    void updateWindow(unsigned int width, unsigned int height);
    void saveKeyMapping();

private:
    void loadKeyMapping();

private:
    //OIS Input devices
    OIS::InputManager*  inputManager;
    OIS::Mouse*         mouse;
    OIS::Keyboard*      keyboard;
    OIS::JoyStick*      joystick;

    //OIS::JoyStickState  centralJoystickState;
    float               deadZone;
    KeyConfig*          test_kc;
    keyMap_t            keyMap;
};

#endif // EVENTRECEIVER_H
