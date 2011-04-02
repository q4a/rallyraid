
#include "RaceManager.h"
#include "Race.h"
#include "stdafx.h"
#include "ConfigDirectory.h"


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

    bool ret = ConfigDirectory::load(RACES_DIR.c_str(), RACE_CFG.c_str(), fileList);
    
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

/* static */ void RaceManager::readShortDescription(const std::string& fileName, std::string& shortDescription)
{
    FILE* f;
    const unsigned int sdSize = 4096;
    char* tmp = new char[sdSize];
    memset(tmp, 0, sdSize);
    
    errno_t error = fopen_s(&f, fileName.c_str(), "rb");
    if (error)
    {
        printf("unable to open file for read %s\n", fileName.c_str());
        delete tmp;
        return;
    }

    size_t ret = fread(tmp, sizeof(char), sdSize - 1, f);
    if (ret)
    {
        shortDescription = tmp;
    }
    fclose(f);
    delete tmp;
}
