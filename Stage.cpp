
#include "Race.h"
#include "RaceManager.h"
#include "Day.h"
#include "Stage.h"
#include "ConfigFile.h"
#include "stdafx.h"


Stage::Stage(const std::string& raceName, const std::string& dayName, const std::string& stageName, bool& ret)
    : raceName(raceName),
      dayName(dayName),
      stageName(stageName),
      stageLongName(),
      shortDescription()
{
    ret = read();
}

Stage::~Stage()
{
}
    
bool Stage::read()
{
    bool ret = readCfg();
    if (ret)
    {
        readShortDescription();
    }
    return ret;
}

bool Stage::readCfg()
{
    ConfigFile cf;
    cf.load(STAGE_DIR(raceName, dayName, stageName)+"/"+STAGE_CFG);

    dprintf(MY_DEBUG_NOTE, "Read stage cfg file: %s / %s / %s\n", raceName.c_str(), dayName.c_str(), stageName.c_str());
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
            dprintf(MY_DEBUG_NOTE, "\t\tkey: %s, value: %s\n", keyName.c_str(), valName.c_str());

            if (keyName == "long_name")
            {
                stageLongName = valName.c_str();
            }/* else if (keyName == "cache_objects")
            {
                cacheObjects = StringConverter::parseBool(valName, true);
            }*/
        }
    }
    return true;
}

void Stage::readShortDescription()
{
    RaceManager::readShortDescription(STAGE_DIR(raceName, dayName, stageName) + "/description.txt", shortDescription);
}
