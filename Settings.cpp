
#include "Settings.h"
#include "ConfigFile.h"
#include "StringConverter.h"

Settings* Settings::settings = 0;
const std::string Settings::settingsFilename = "data/Dakar2012_settings.cfg";

void Settings::initialize()
{
    if (settings == 0)
    {
        settings = new Settings();
    }
}

void Settings::finalize()
{
    if (settings)
    {
        delete settings;
        settings = 0;
    }
}


Settings::Settings()
    : preloadObjects(false),
      cacheObjects(true),
      targetFps(60),
      nonshaderLight(false),
      objectWireSize(100),
      objectWireNum(11),
      objectDensity(10)
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
                preloadObjects = StringConverter::parseBool(valName, false);
            } else if (keyName == "cache_objects")
            {
                cacheObjects = StringConverter::parseBool(valName, true);
            } else if (keyName == "target_fps")
            {
                targetFps = StringConverter::parseUnsignedInt(valName, 60);
            } else if (keyName == "nonshader_light")
            {
                nonshaderLight = StringConverter::parseBool(valName, false);
            } else if (keyName == "object_wire_size")
            {
                objectWireSize = StringConverter::parseUnsignedInt(valName, 100);
            } else if (keyName == "object_wire_num")
            {
                objectWireNum = StringConverter::parseUnsignedInt(valName, 11);
            } else if (keyName == "object_density")
            {
                objectDensity = StringConverter::parseUnsignedInt(valName, 10);
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
    ret = fprintf(f, "target_fps=%u\n", targetFps);
    ret = fprintf(f, "nonshader_light=%s\n", nonshaderLight?"yes":"no");
    ret = fprintf(f, "object_wire_size=%u\n", objectWireSize);
    ret = fprintf(f, "object_wire_num=%u\n", objectWireNum);
    ret = fprintf(f, "object_density=%u\n", objectDensity);

    fclose(f);
}
