
#ifndef KEYCONFIG_H
#define KEYCONFIG_H

/*namespace OIS
{
    class JoyStickState;
}*/
namespace OIS
{
    class Keyboard;
}

#include <OISJoyStick.h>
#include <string>

class KeyConfig
{
public:
    enum type_t
    {
        Keyboard = 0,
        JoystickButton,
        JoystickAxis,
        JoystickSliderX,
        JoystickSliderY,
        JoystickPov
    };
protected:
    KeyConfig(type_t p_type);

public:
    virtual ~KeyConfig();

public:
    static KeyConfig* getKeyConfig(OIS::Keyboard* keyboard, const OIS::JoyStickState& joystickState, float deadZone, bool continous = true);
    static KeyConfig* getKeyConfig(type_t p_type, unsigned int p_key, unsigned int p_key2, int p_from, int p_to);
    static void recalibrate(OIS::JoyStick* joystick);

public:
    //virtual bool isPressed(OIS::Keyboard* keyboard, const OIS::JoyStickState& joystickState) = 0;
    virtual float getPercentage(OIS::Keyboard* keyboard, const OIS::JoyStickState& joystickState) = 0;
    virtual void writeToFile(FILE* f, const std::string& prefix) = 0;

protected:
    virtual float getPercentage(int state);

public:
    static OIS::JoyStickState centralJoystickState;

public:
    bool continous;
    bool active;
    type_t type;
    unsigned int key;
};

class KeyConfigKeyboard : public KeyConfig
{
public:
    KeyConfigKeyboard();
    virtual ~KeyConfigKeyboard();

    //virtual bool isPressed(OIS::Keyboard* keyboard, const OIS::JoyStickState& joystickState);
    virtual float getPercentage(OIS::Keyboard* keyboard, const OIS::JoyStickState& joystickState);
    virtual void writeToFile(FILE* f, const std::string& prefix);
};

class KeyConfigJoystickButton : public KeyConfig
{
public:
    KeyConfigJoystickButton();
    virtual ~KeyConfigJoystickButton();

    //virtual bool isPressed(OIS::Keyboard* keyboard, const OIS::JoyStickState& joystickState);
    virtual float getPercentage(OIS::Keyboard* keyboard, const OIS::JoyStickState& joystickState);
    virtual void writeToFile(FILE* f, const std::string& prefix);
};

class KeyConfigJoystickPov : public KeyConfig
{
public:
    KeyConfigJoystickPov();
    virtual ~KeyConfigJoystickPov();

    //virtual bool isPressed(OIS::Keyboard* keyboard, const OIS::JoyStickState& joystickState);
    virtual float getPercentage(OIS::Keyboard* keyboard, const OIS::JoyStickState& joystickState);
    virtual void writeToFile(FILE* f, const std::string& prefix);

public:
    unsigned int key2;
};

class KeyConfigJoystickAxis : public KeyConfig
{
public:
    KeyConfigJoystickAxis();
    virtual ~KeyConfigJoystickAxis();

protected:
    KeyConfigJoystickAxis(KeyConfig::type_t p_type);

private:
    //virtual bool isPressed(OIS::Keyboard* keyboard, const OIS::JoyStickState& joystickState);
    virtual float getPercentage(OIS::Keyboard* keyboard, const OIS::JoyStickState& joystickState);

public:
    virtual void writeToFile(FILE* f, const std::string& prefix);

protected:
    virtual float getPercentage(int state);

public:
    int from;
    int to;
};

class KeyConfigJoystickSliderX : public KeyConfigJoystickAxis
{
public:
    KeyConfigJoystickSliderX();
    virtual ~KeyConfigJoystickSliderX();

    //virtual bool isPressed(OIS::Keyboard* keyboard, const OIS::JoyStickState& joystickState);
    virtual float getPercentage(OIS::Keyboard* keyboard, const OIS::JoyStickState& joystickState);
};

class KeyConfigJoystickSliderY : public KeyConfigJoystickAxis
{
public:
    KeyConfigJoystickSliderY();
    virtual ~KeyConfigJoystickSliderY();

    //virtual bool isPressed(OIS::Keyboard* keyboard, const OIS::JoyStickState& joystickState);
    virtual float getPercentage(OIS::Keyboard* keyboard, const OIS::JoyStickState& joystickState);
};

#endif // KEYCONFIG_H
