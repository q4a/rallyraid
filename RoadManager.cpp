
#include "RoadManager.h"
#include "Road.h"
#include "stdafx.h"
#include "ConfigDirectory.h"
#include "ConfigFile.h"
#include "StringConverter.h"
#include "Terrain_defs.h"
#include "TheEarth.h"

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
      visibleRoadChunkSet(),
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

void RoadManager::addStageRoad(Road* road)
{
    unsigned long ps = road->roadPointVector.size();
    if (ps == 0) return;

    RoadRoadChunk currentChunk;
    currentChunk.road = road;
    currentChunk.roadChunk = roadChunk_t(0, 0);
    unsigned int currentTile = TheEarth::getInstance()->calculateTileNum(
        (unsigned int)abs((int)road->roadPointVector[0].X), (unsigned int)abs((int)road->roadPointVector[0].Z));

    for (unsigned int i = 1; i < ps; i++)
    {
        unsigned int newTile = TheEarth::getInstance()->calculateTileNum(
            (unsigned int)abs((int)road->roadPointVector[i].X), (unsigned int)abs((int)road->roadPointVector[i].Z));
        if (newTile != currentTile)
        {
            currentChunk.roadChunk.second = i - 1;
            stageRoadChunkListMap[currentTile].push_back(currentChunk);

            currentChunk.roadChunk = roadChunk_t(i, i);
            currentTile = newTile;
        }
    }
    currentChunk.roadChunk.second = ps - 1;
    stageRoadChunkListMap[currentTile].push_back(currentChunk);
}

void RoadManager::clearStageRoads()
{
    stageRoadChunkListMap.clear();
}

void RoadManager::clearVisible()
{
    visibleRoadChunkSet.clear();
}

void RoadManager::addChunkListToVisible(const roadRoadChunkList_t& roadRoadChunkList)
{
    for (roadRoadChunkList_t::const_iterator rcit = roadRoadChunkList.begin();
         rcit != roadRoadChunkList.end();
         rcit++)
    {
        visibleRoadChunkSet.insert(*rcit);
    }
}

void RoadManager::setVisibleStageRoad(unsigned int tileNum)
{
    stageRoadChunkListMap_t::const_iterator srit = stageRoadChunkListMap.find(tileNum);
    if (srit != stageRoadChunkListMap.end())
    {
        addChunkListToVisible(srit->second);
    }
}
