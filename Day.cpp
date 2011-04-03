
#include "Race.h"
#include "Day.h"
#include "Stage.h"
#include "ConfigDirectory.h"
#include "ConfigFile.h"
#include "stdafx.h"


Day::Day(const std::string& raceName, const std::string& dayName, bool& ret)
    : raceName(raceName),
      dayName(dayName),
      dayLongName(),
      shortDescription(),
      stageMap(),
      globalObjectList(),
      active(false)
{
    ret = read();
}

Day::~Day()
{
    for (stageMap_t::const_iterator sit = stageMap.begin();
         sit != stageMap.end();
         sit++)
    {
        delete sit->second;
    }
    stageMap.clear();

    deactivate();
    RaceManager::clearGlobalObjects(globalObjectList);
}
    
bool Day::read()
{
    bool ret = readCfg();
    if (ret)
    {
        ret = readStages();
        if (ret)
        {
            readShortDescription();
            readGlobalObjects();
        }
    }
    return ret;
}

bool Day::readCfg()
{
    ConfigFile cf;
    cf.load(DAY_DIR(raceName, dayName)+"/"+DAY_CFG);

    dprintf(MY_DEBUG_NOTE, "Read day cfg file: %s / %s\n", raceName.c_str(), dayName.c_str());
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
                dayLongName = valName;
            }/* else if (keyName == "cache_objects")
            {
                cacheObjects = StringConverter::parseBool(valName, true);
            }*/
        }
    }
    return true;
}

bool Day::readStages()
{
    ConfigDirectory::fileList_t fileList;
    
    dprintf(MY_DEBUG_NOTE, "Read day (%s / %s) directory:\n", raceName.c_str(), dayName.c_str());

    bool ret = ConfigDirectory::load(DAY_DIR(raceName, dayName).c_str(), STAGE_CFG.c_str(), fileList);
    
    if (!ret)
    {
        dprintf(MY_DEBUG_WARNING, "unable to read day (%s / %s) directory\n", raceName.c_str(), dayName.c_str());
        return false;
    }
    
    for (ConfigDirectory::fileList_t::const_iterator it = fileList.begin();
         it != fileList.end();
         it++)
    {
        std::string stageName = it->c_str();
        Stage* stage = new Stage(raceName, dayName, stageName, ret);
        if (!ret)
        {
            delete stage;
        }
        else
        {
            stageMap[stageName] = stage;
        }
    }
    return true;
}

void Day::readShortDescription()
{
    RaceManager::readShortDescription(DAY_DIR(raceName, dayName) + "/description.txt", shortDescription);
}

void Day::readGlobalObjects()
{
    RaceManager::readGlobalObjects(DAY_DIR(raceName, dayName) + "/" + OBJECTS_CFG, globalObjectList);
}

void Day::activate()
{
    if (active) return;

    RaceManager::addGlobalObjectsToObjectWire(globalObjectList);
    active = true;
}

void Day::deactivate()
{
    if (!active) return;

    RaceManager::removeGlobalObjectsFromObjectWire(globalObjectList);
    active = false;
}
