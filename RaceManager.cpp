
#include "RaceManager.h"
#include "Race.h"
#include "stdafx.h"


RaceManager* RaceManager::raceManager = 0;

void RaceManager::initialize()
{
    if (raceManager == 0)
    {
        raceManager = new RaceManager();        
    }
}

void RaceManager::finalize()
{
    if (raceManager)
    {
        delete raceManager;
        raceManager = 0;
    }
}


RaceManager::RaceManager()
    : raceMap()
{
}

RaceManager::~RaceManager()
{
    for (raceMap_t::const_iterator rit = raceMap.begin();
         rit != raceMap.end();
         rit++)
    {
        delete rit->second;
    }
    raceMap.clear();
}

void RaceManager::read()
{
    ConfigDirectory::fileList_t fileList;
    
    dprintf(MY_DEBUG_NOTE, "Read races directory:\n");

    bool ret = ConfigDirectory::load(RACES_DIR, RACE_CFG, fileList);
    
    if (!ret)
    {
        dprintf(MY_DEBUG_WARNING, "unable to read races directory\n");
        return;
    }
    
    for (ConfigDirectory::fileList_t::const_iterator it = fileList.begin();
         it != fileList.end();
         it++)
    {
        std::string raceName = it->c_str();
        Race* race = new Race(raceName, ret);
        if (!ret)
        {
            delete race;
        }
        else
        {
            raceMap[raceName] = race;
        }
    }
}

