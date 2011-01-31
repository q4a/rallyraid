
#include "Settings.h"
#include "ConfigFile.h"

Settings* Settings::settings = 0;

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
{
}

Settings::~Settings()
{
}

void Settings::read()
{
    ConfigFile cf;
    cf.load("data/Dakar2012_settings.cfg");

    dprintf(MY_DEBUG_NOTE, "Read resource file:\n");
    // Go through all sections & settings in the file
    ConfigFile::SectionIterator seci = cf.getSectionIterator();

    std::string secName, typeName, archName;
    while (seci.hasMoreElements())
    {
        secName = seci.peekNextKey();
        dprintf(MY_DEBUG_NOTE, "\tKey: %s\n", secName.c_str());
        ConfigFile::SettingsMultiMap *settings = seci.getNext();
        ConfigFile::SettingsMultiMap::iterator i;
        for (i = settings->begin(); i != settings->end(); ++i)
        {
            typeName = i->first;
            archName = i->second;
            dprintf(MY_DEBUG_NOTE, "\t\ttype: %s, arch: %s\n", typeName.c_str(), archName.c_str());
        }
    }
}
