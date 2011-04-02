
#include "Race.h"
#include "RaceManager.h"
#include "Day.h"
#include "ConfigDirectory.h"
#include "ConfigFile.h"
#include "stdafx.h"


Race::Race(const std::string& raceName, bool& ret)
    : raceName(raceName),
      raceLongName(),
      shortDescription(),
      dayMap()
{
    ret = read();
}

Race::~Race()
{
    for (dayMap_t::const_iterator dit = dayMap.begin();
         dit != dayMap.end();
         dit++)
    {
        delete dit->second;
    }
    dayMap.clear();
}
    
bool Race::read()
{
    bool ret = readCfg();
    if (ret)
    {
        ret = readDays();
        if (ret)
        {
            readShortDescription();
        }
    }
    return ret;
}

bool Race::readCfg()
{
    ConfigFile cf;
    cf.load(RACE_DIR(raceName)+"/"+RACE_CFG);

    dprintf(MY_DEBUG_NOTE, "Read race cfg file: %s\n", raceName.c_str());
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
                raceLongName = valName.c_str();
            }/* else if (keyName == "cache_objects")
            {
                cacheObjects = StringConverter::parseBool(valName, true);
            }*/
        }
    }
    return true;
}

bool Race::readDays()
{
    ConfigDirectory::fileList_t fileList;
    
    dprintf(MY_DEBUG_NOTE, "Read race (%s) directory:\n", raceName.c_str());

    bool ret = ConfigDirectory::load(RACE_DIR(raceName).c_str(), DAY_CFG.c_str(), fileList);
    
    if (!ret)
    {
        dprintf(MY_DEBUG_WARNING, "unable to read race (%s) directory\n", raceName.c_str());
        return false;
    }
    
    for (ConfigDirectory::fileList_t::const_iterator it = fileList.begin();
         it != fileList.end();
         it++)
    {
        std::string dayName = it->c_str();
        Day* day = new Day(raceName, dayName, ret);
        if (!ret)
        {
            delete day;
        }
        else
        {
            dayMap[dayName] = day;
        }
    }
    return true;
}

void Race::readShortDescription()
{
    RaceManager::readShortDescription(RACE_DIR(raceName) + "/description.txt", shortDescription);
}
