#ifndef SETTINGS_H
#define SETTINGS_H

#include "stdafx.h"
#include <string>

class Settings
{
public:
    static void initialize();
    static void finalize();
    static Settings* getInstance() {return settings;}

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
    unsigned int            targetFps;
    bool                    nonshaderLight;
    unsigned int            objectWireSize;
    unsigned int            objectWireNum;
    unsigned int            objectDensity;
    bool                    useTerrainDetail;
    bool                    showNames;
    unsigned int            difficulty;
    bool                    navigationAssistant;
};

#endif // SETTINGS_H
