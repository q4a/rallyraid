
#include "KeyConfig.h"
#include "stdafx.h"

#include <OISKeyboard.h>
#include <OISJoyStick.h>

#include <OgreMath.h>
#include <assert.h>

OIS::JoyStickState KeyConfig::centralJoystickState;

KeyConfig* KeyConfig::getKeyConfig(OIS::Keyboard* keyboard, const OIS::JoyStickState& joystickState, float deadZone, bool continous)
{
    KeyConfig* ret = 0;

    if (joystickState.mAxes.size() != centralJoystickState.mAxes.size() ||
        joystickState.mButtons.size() != centralJoystickState.mButtons.size()
        )
    {
        dprintf(MY_DEBUG_INFO, "joystick state has been changed: buttons: %lu -> %lu, axis: %lu -> %lu\n",
            joystickState.mButtons.size(), centralJoystickState.mButtons.size(),
            joystickState.mAxes.size(), centralJoystickState.mAxes.size());
        centralJoystickState = joystickState;
        return ret;
    }

    for (/*OIS::KeyCode*/unsigned int i = OIS::KC_1; i <= OIS::KC_MEDIASELECT; i++)
    {
        if (keyboard->isKeyDown((OIS::KeyCode)i))
        {
            ret = new KeyConfigKeyboard();
            ret->continous = continous;
            ret->key = i;
            dprintf(MY_DEBUG_INFO, "keyboard assign: %u\n", i);
            return ret;
        }
    }

    for (unsigned int i = 0; i < joystickState.mButtons.size(); i++)
    {
        if (joystickState.mButtons[i])
        {
            ret = new KeyConfigJoystickButton();
            ret->continous = continous;
            ret->key = i;
            dprintf(MY_DEBUG_INFO, "joystick button assign: %u\n", i);
            return ret;
        }
    }

    for (unsigned int i = 0; i < 4; i++)
    {
        if (joystickState.mPOV[i].direction)
        {
            ret = new KeyConfigJoystickPov();
            ret->key = i;
            ret->continous = continous;
            ((KeyConfigJoystickPov*)ret)->key2 = joystickState.mPOV[i].direction;
            dprintf(MY_DEBUG_INFO, "joystick pov assign: %u %x\n", i, joystickState.mPOV[i].direction);
            return ret;
        }
    }
    

    for (unsigned int i = 0; i < joystickState.mAxes.size(); i++)
    {
        if (Ogre::Math::IAbs(centralJoystickState.mAxes[i].abs - joystickState.mAxes[i].abs) > 10000)
        {
            if (joystickState.mAxes[i].abs == OIS::JoyStick::MIN_AXIS ||
                joystickState.mAxes[i].abs == OIS::JoyStick::MAX_AXIS ||
                Ogre::Math::IAbs(centralJoystickState.mAxes[i].abs - joystickState.mAxes[i].abs) > 20000)
            {
                dprintf(MY_DEBUG_INFO, "joystick state has been changed, but recalibrate: axes[%u]: %d -> %d\n", i,
                    centralJoystickState.mAxes[i].abs, joystickState.mAxes[i].abs);
                centralJoystickState = joystickState;
                return ret;
            }
            
            KeyConfigJoystickAxis* kc = new KeyConfigJoystickAxis();
            kc->key = i;
            kc->from = centralJoystickState.mAxes[i].abs;
            if (joystickState.mAxes[i].abs < kc->from/*-5000*/)
            {
                kc->to = OIS::JoyStick::MIN_AXIS;
            }
            else //if (joystickState.mAxes[i].abs > kc->from/*5000*/)
            {
                kc->to = OIS::JoyStick::MAX_AXIS;
            }
            dprintf(MY_DEBUG_INFO, "joystick axis assign: %u\n", i);
            return kc;
        }
    }

    for (unsigned int i = 0; i < 4; i++)
    {
        if (Ogre::Math::IAbs(centralJoystickState.mSliders[i].abX - joystickState.mSliders[i].abX) > 10000)
        {
            if (joystickState.mSliders[i].abX == OIS::JoyStick::MIN_AXIS ||
                joystickState.mSliders[i].abX == OIS::JoyStick::MAX_AXIS ||
                Ogre::Math::IAbs(centralJoystickState.mSliders[i].abX - joystickState.mSliders[i].abX) > 20000)
            {
                dprintf(MY_DEBUG_INFO, "joystick state has been changed, but recalibrate: slider[%u] x: %d -> %d\n", i,
                    centralJoystickState.mSliders[i].abY, joystickState.mSliders[i].abY);
                centralJoystickState = joystickState;
                return ret;
            }
            
            KeyConfigJoystickAxis* kc = new KeyConfigJoystickSliderX();
            kc->key = i;
            kc->from = centralJoystickState.mSliders[i].abX;
            if (joystickState.mSliders[i].abX < kc->from/*-5000*/)
            {
                kc->to = OIS::JoyStick::MIN_AXIS;
            }
            else //if (joystickState.mSliders[i].abX > kc->from/*5000*/)
            {
                kc->to = OIS::JoyStick::MAX_AXIS;
            }
            dprintf(MY_DEBUG_INFO, "joystick slide x assign: %u\n", i);
            return kc;
        }
    }

    for (unsigned int i = 0; i < 4; i++)
    {
        if (Ogre::Math::IAbs(centralJoystickState.mSliders[i].abY - joystickState.mSliders[i].abY) > 10000)
        {
            if (joystickState.mSliders[i].abY == OIS::JoyStick::MIN_AXIS ||
                joystickState.mSliders[i].abY == OIS::JoyStick::MAX_AXIS ||
                Ogre::Math::IAbs(centralJoystickState.mSliders[i].abY - joystickState.mSliders[i].abY) > 20000)
            {
                dprintf(MY_DEBUG_INFO, "joystick state has been changed, but recalibrate: slider[%u] y: %d -> %d\n", i,
                    centralJoystickState.mSliders[i].abY, joystickState.mSliders[i].abY);
                centralJoystickState = joystickState;
                return ret;
            }
            
            KeyConfigJoystickAxis* kc = new KeyConfigJoystickSliderY();
            kc->key = i;
            kc->from = centralJoystickState.mSliders[i].abY;
            if (joystickState.mSliders[i].abY < kc->from/*-5000*/)
            {
                kc->to = OIS::JoyStick::MIN_AXIS;
            }
            else //if (joystickState.mSliders[i].abY > kc->from/*5000*/)
            {
                kc->to = OIS::JoyStick::MAX_AXIS;
            }
            dprintf(MY_DEBUG_INFO, "joystick slide y assign: %u (%d - %d)\n", i, centralJoystickState.mSliders[i].abY, joystickState.mSliders[i].abY);
            return kc;
        }
    }

    return ret;
}

KeyConfig* KeyConfig::getKeyConfig(KeyConfig::type_t p_type, unsigned int p_key, unsigned int p_key2, int p_from, int p_to)
{
    switch (p_type)
    {
        case KeyConfig::Keyboard:
        {
            KeyConfigKeyboard* ret = new KeyConfigKeyboard();
            ret->key = p_key;
            return ret;
            break;
        }
        case KeyConfig::JoystickButton:
        {
            KeyConfigJoystickButton* ret = new KeyConfigJoystickButton();
            ret->key = p_key;
            return ret;
            break;
        }
        case KeyConfig::JoystickAxis:
        {
            KeyConfigJoystickAxis* ret = new KeyConfigJoystickAxis();
            ret->key = p_key;
            ret->from = p_from;
            ret->to = p_to;
            return ret;
            break;
        }
        case KeyConfig::JoystickSliderX:
        {
            KeyConfigJoystickSliderX* ret = new KeyConfigJoystickSliderX();
            ret->key = p_key;
            ret->from = p_from;
            ret->to = p_to;
            return ret;
            break;
        }
        case KeyConfig::JoystickSliderY:
        {
            KeyConfigJoystickSliderY* ret = new KeyConfigJoystickSliderY();
            ret->key = p_key;
            ret->from = p_from;
            ret->to = p_to;
            return ret;
            break;
        }
        case KeyConfig::JoystickPov:
        {
            KeyConfigJoystickPov* ret = new KeyConfigJoystickPov();
            ret->key = p_key;
            ret->key2 = p_key2;
            return ret;
            break;
        }
    }

    return 0;
}

void KeyConfig::recalibrate(OIS::JoyStick* joystick)
{
    centralJoystickState = joystick->getJoyStickState();
}

// class KeyConfig
KeyConfig::KeyConfig(type_t p_type)
    : continous(true), active(false), type(p_type), key((unsigned int)-1)
{
}

KeyConfig::~KeyConfig()
{
}

float KeyConfig::getPercentage(int state)
{
    bool bstate = state != 0;
    if (continous)
    {
        active = bstate;
        return active ? 1.0f : 0.0f;
    }
    else
    {
        if (bstate != active)
        {
            active = bstate;
            return active ? 1.0f : 0.0f;
        }
    }
    return 0.0f;
}

// class KeyConfigKeyboard : public KeyConfig
KeyConfigKeyboard::KeyConfigKeyboard()
    : KeyConfig(KeyConfig::Keyboard)
{
}

KeyConfigKeyboard::~KeyConfigKeyboard()
{
}

float KeyConfigKeyboard::getPercentage(OIS::Keyboard* keyboard, const OIS::JoyStickState& joytickState)
{
    return KeyConfig::getPercentage((int)(keyboard->isKeyDown((OIS::KeyCode)key)));
}

// class KeyConfigJoystickButton : public KeyConfig
KeyConfigJoystickButton::KeyConfigJoystickButton()
    : KeyConfig(KeyConfig::JoystickButton)
{
}

KeyConfigJoystickButton::~KeyConfigJoystickButton()
{
}

float KeyConfigJoystickButton::getPercentage(OIS::Keyboard* keyboard, const OIS::JoyStickState& joystickState)
{
    return KeyConfig::getPercentage(joystickState.mButtons[key]);
}

// class KeyConfigJoystickPov : public KeyConfig
KeyConfigJoystickPov::KeyConfigJoystickPov()
    : KeyConfig(KeyConfig::JoystickPov), key2((unsigned int)-1)
{
}

KeyConfigJoystickPov::~KeyConfigJoystickPov()
{
}

float KeyConfigJoystickPov::getPercentage(OIS::Keyboard* keyboard, const OIS::JoyStickState& joystickState)
{
    return KeyConfig::getPercentage((int)(joystickState.mPOV[key].direction == key2));
}

// class KeyConfigJoystickAxis : public KeyConfig
KeyConfigJoystickAxis::KeyConfigJoystickAxis()
    : KeyConfig(KeyConfig::JoystickAxis), from(OIS::JoyStick::MIN_AXIS), to(OIS::JoyStick::MAX_AXIS)
{
}

KeyConfigJoystickAxis::KeyConfigJoystickAxis(KeyConfig::type_t p_type)
    : KeyConfig(p_type), from(OIS::JoyStick::MIN_AXIS), to(OIS::JoyStick::MAX_AXIS)
{
}

KeyConfigJoystickAxis::~KeyConfigJoystickAxis()
{
}

float KeyConfigJoystickAxis::getPercentage(OIS::Keyboard* keyboard, const OIS::JoyStickState& joystickState)
{
    return KeyConfigJoystickAxis::getPercentage(joystickState.mAxes[key].abs);
}

float KeyConfigJoystickAxis::getPercentage(int state)
{
    int min = to < from ? to : from;
    int max = to > from ? to : from;

    /*if (min < 0)
    {
        max += -min;
        state += -min;
        min = 0;
    }*/

    state = Ogre::Math::Clamp(state, min, max);
    
    if (to > from)
    {
        return (float)(state - min) / (float)(max - min);
    }
    else
    {
        return (float)(max - state) / (float)(max - min);
    }
}

// class KeyConfigJoystickSliderX : public KeyConfigJoystickAxis
KeyConfigJoystickSliderX::KeyConfigJoystickSliderX()
    : KeyConfigJoystickAxis(KeyConfig::JoystickSliderX)
{
}

KeyConfigJoystickSliderX::~KeyConfigJoystickSliderX()
{
}

float KeyConfigJoystickSliderX::getPercentage(OIS::Keyboard* keyboard, const OIS::JoyStickState& joystickState)
{
    return KeyConfigJoystickAxis::getPercentage(joystickState.mSliders[key].abX);
}

// class KeyConfigJoystickSliderY : public KeyConfigJoystickAxis
KeyConfigJoystickSliderY::KeyConfigJoystickSliderY()
    : KeyConfigJoystickAxis(KeyConfig::JoystickSliderY)
{
}

KeyConfigJoystickSliderY::~KeyConfigJoystickSliderY()
{
}

float KeyConfigJoystickSliderY::getPercentage(OIS::Keyboard* keyboard, const OIS::JoyStickState& joystickState)
{
    return KeyConfigJoystickAxis::getPercentage(joystickState.mSliders[key].abY);
}

