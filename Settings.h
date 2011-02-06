#ifndef SETTINGS_H
#define SETTINGS_H

#include "stdafx.h"
#include <string>

class Settings
{
public:
    static Settings* getInstance();
    static void destroy();

private:
    static Settings* settings;
    static const std::string settingsFilename;

private:
    Settings();
    ~Settings();

public:
    void read();
    void write();

public:
    bool                    preloadObjects;
    bool                    cacheObjects;
};

#endif // SETTINGS_H
