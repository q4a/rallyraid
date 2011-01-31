#ifndef SETTINGS_H
#define SETTINGS_H

#include "stdafx.h"

class Settings
{
public:
    static Settings* getInstance();
    static void destroy();

private:
    static Settings* settings;

private:
    Settings();
    ~Settings();

public:
    void read();

public:
};

#endif // SETTINGS_H
