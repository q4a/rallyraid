
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
      editorRoad(0),
      editorRadius(0),
      editorColor(0)
{
    read();
}

RoadManager::~RoadManager()
{
    clearRoadMap(roadMap);
}

void RoadManager::read()
{
    readRoads("data/roads", roadMap, true, false);
}

/* static */
void RoadManager::readRoads(const std::string& dirName, RoadManager::roadMap_t& roadMap, bool global, bool doRead)
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
        Road* road = new Road(dirName+"/"+roadFilename, ret, global, doRead);
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
        (unsigned int)abs((int)road->roadPointVector[0].p.X), (unsigned int)abs((int)road->roadPointVector[0].p.Z));

    for (unsigned int i = 1; i < ps; i++)
    {
        unsigned int newTile = TheEarth::getInstance()->calculateTileNum(
            (unsigned int)abs((int)road->roadPointVector[i].p.X), (unsigned int)abs((int)road->roadPointVector[i].p.Z));
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

void RoadManager::addStageRoad(const RoadManager::roadMap_t& p_roadMap)
{
    for (roadMap_t::const_iterator it = p_roadMap.begin();
         it != p_roadMap.end();
         it++)
    {
        addStageRoad(it->second);
    }
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

/* static */
bool RoadManager::readRoadRoadChunk(const std::string& fileName, roadRoadChunkList_t& roadRoadChunkList, const RoadManager::roadMap_t& roadMap)
{
    FILE* f;
    int ret = 0;
    char roadName[256];
    unsigned int first, second;

    if (!roadRoadChunkList.empty())
    {
        dprintf(MY_DEBUG_WARNING, "road roadChunk list already exists for tile %s\n", fileName.c_str());
        return true;
    }

    errno_t error = fopen_s(&f, fileName.c_str(), "r");
    if (error)
    {
        printf("road roadChunk file unable to open: %s\n", fileName.c_str());
        return false;
    }
    
    while (true)
    {
        ret = fscanf_s(f, "%s %u %u\n", roadName, &first, &second);
        if (ret < 3 )
        {
            break;
        }
        RoadRoadChunk rrc;
        roadMap_t::const_iterator rit = roadMap.find(roadName);
        //rrc.road = getRoad(roadName);
        if (rit != roadMap.end())
        {
            rrc.road = rit->second;
            if (rrc.road)
            {
                rrc.roadChunk.first = first;
                rrc.roadChunk.second = second;
                roadRoadChunkList.push_back(rrc);
            }
        }
    }
    fclose(f);
    return true;
}

/* static */
bool RoadManager::writeRoadRoadChunk(const std::string& fileName, const roadRoadChunkList_t& roadRoadChunkList)
{
    FILE* f;
    int ret = 0;
    errno_t error = fopen_s(&f, fileName.c_str(), "w");
    if (error)
    {
        printf("unable to open road roadChunk file for write %s\n", fileName.c_str());
        return false;
    }

    for (roadRoadChunkList_t::const_iterator it = roadRoadChunkList.begin();
         it != roadRoadChunkList.end();
         it++)
    {
        ret = fprintf(f, "%s %u %u\n", it->road->getName().c_str(), it->roadChunk.first, it->roadChunk.second);
    }

    fclose(f);
    return true;
}

/* static */
void RoadManager::clearRoadMap(RoadManager::roadMap_t& roadMap)
{
    for (roadMap_t::const_iterator rit = roadMap.begin();
         rit != roadMap.end();
         rit++)
    {
        delete rit->second;
    }
    roadMap.clear();
}

/* static */ void RoadManager::editorRenderRoads(const RoadManager::roadMap_t& roadMap)
{
    for (roadMap_t::const_iterator rit = roadMap.begin();
         rit != roadMap.end();
         rit++)
    {
        if (rit->second != RoadManager::getInstance()->editorRoad)
        {
            rit->second->editorRender();
        }
    }
}
