
#include "RoadManager.h"
#include "Road.h"
#include "stdafx.h"
#include "ConfigDirectory.h"
#include "ConfigFile.h"
#include "StringConverter.h"


RoadManager* RoadManager::roadManager = 0;

void RoadManager::initialize()
{
    if (roadManager == 0)
    {
        roadManager = new RoadManager();
    }
}

void RoadManager::finalize()
{
    if (roadManager)
    {
        delete roadManager;
        roadManager = 0;
    }
}


RoadManager::RoadManager()
    : roadMap(),
      stageRoadChunkListMap(),
      visibleRoadChunkListSet(),
      editorRoad(0)
{
    read();
}

RoadManager::~RoadManager()
{
    for (roadMap_t::const_iterator rit = roadMap.begin();
         rit != roadMap.end();
         rit++)
    {
        delete rit->second;
    }
    roadMap.clear();
}

void RoadManager::read()
{
    readRoads("data/roads", roadMap, false);
}

/* static */
void RoadManager::readRoads(const std::string& dirName, RoadManager::roadMap_t& roadMap, bool doRead)
{
    ConfigDirectory::fileList_t fileList;
    
    dprintf(MY_DEBUG_NOTE, "Read roads directory:\n");

    bool ret = ConfigDirectory::load(dirName.c_str(), fileList);
    
    if (!ret)
    {
        dprintf(MY_DEBUG_WARNING, "unable to read roads directory\n");
        return;
    }
    
    for (ConfigDirectory::fileList_t::const_iterator it = fileList.begin();
         it != fileList.end();
         it++)
    {
        std::string roadFilename = it->c_str();
        bool ret = false;
        Road* road = new Road(roadFilename, ret, doRead);
        if (ret)
        {
            roadMap[road->getName()] = road;
        }
        else
        {
            dprintf(MY_DEBUG_ERROR, "road file is invalid: %s\n", roadFilename.c_str());
            delete road;
        }
    }
}
