
#include "Settings.h"
#include "ConfigFile.h"
#include "StringConverter.h"

Settings* Settings::settings = 0;
const std::string Settings::settingsFilename = "data/Dakar2012_settings.cfg";

Settings* Settings::getInstance()
{
    if (settings == 0)
    {
        settings = new Settings();
    }
    return settings;
}

void Settings::destroy()
{
    if (settings)
    {
        delete settings;
        settings = 0;
    }
}


Settings::Settings()
    : preloadObjects(true),
      cacheObjects(true)
{
}

Settings::~Settings()
{
}

void Settings::read()
{
    ConfigFile cf;
    cf.load(settingsFilename);

    dprintf(MY_DEBUG_NOTE, "Read settings file:\n");
    // Go through all sections & settings in the file
    ConfigFile::SectionIterator seci = cf.getSectionIterator();

    std::string secName, keyName, valName;
    while (seci.hasMoreElements())
    {
        secName = seci.peekNextKey();
        dprintf(MY_DEBUG_NOTE, "\tSection: %s\n", secName.c_str());
        ConfigFile::SettingsMultiMap *settings = seci.getNext();
        ConfigFile::SettingsMultiMap::iterator i;
        for (i = settings->begin(); i != settings->end(); ++i)
        {
            keyName = i->first;
            valName = i->second;
            dprintf(MY_DEBUG_NOTE, "\t\ttype: %s, arch: %s\n", keyName.c_str(), valName.c_str());

            if (keyName == "preload_objects")
            {
                preloadObjects = StringConverter::parseBool(valName, true);
            } else if (keyName == "cache_objects")
            {
                cacheObjects = StringConverter::parseBool(valName, true);
            }
        }
    }
}

void Settings::write()
{
    FILE* f;
    int ret = 0;

    dprintf(MY_DEBUG_NOTE, "Write settings file:\n");

    errno_t error = fopen_s(&f, settingsFilename.c_str(), "w+");
    if (error)
    {
        dprintf(MY_DEBUG_ERROR, "Unable to open settings file for write: %s\n", settingsFilename.c_str());
        return;
    }

    ret = fprintf(f, "preload_objects=%s\n", preloadObjects?"yes":"no");
    ret = fprintf(f, "cache_objects=%s\n", cacheObjects?"yes":"no");

    fclose(f);
}
