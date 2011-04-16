
#ifndef ROADMANAGER_H
#define ROADMANAGER_H

#include <string>
#include <map>
#include <list>
#include <set>
#include <utility>
#include <irrlicht.h>

class Road;

typedef std::pair<unsigned int, unsigned int> roadChunk_t;

struct RoadRoadChunk
{
    Road*       road;
    roadChunk_t roadChunk;

    bool operator<(const RoadRoadChunk& other) const
    {
        return road < other.road ||
            (road == other.road && roadChunk.first < other.roadChunk.first);
    }
};
typedef std::list<RoadRoadChunk> roadRoadChunkList_t;   // store data for stages road chunks, and for tile chunks
typedef std::set<RoadRoadChunk> roadRoadChunkSet_t;    // maintain visible parts

class RoadManager
{
public:
    static void initialize();
    static void finalize();
    
    static RoadManager* getInstance() {return roadManager;}

    typedef std::map<std::string, Road*>                roadMap_t;
    typedef std::map<unsigned int, roadRoadChunkList_t> stageRoadChunkListMap_t;

private:
    static RoadManager* roadManager;

private:
    RoadManager();
    ~RoadManager();

    void read();

public:
    Road* getRoad(const std::string& roadName); // inline
    const roadMap_t& getRoadMap(); // inline

    void addStageRoad(Road* road);  // call when start a new stage: add race, day and stage roads
    void clearStageRoads();         // call when a stage is ended

    // these stuffs are called from the earth visible part builder thread!
    void clearVisible();                                                        // call when start to build the new visible stuff
    void addChunkListToVisible(const roadRoadChunkList_t& roadRoadChunkList);   // call when a tile become visible
    void setVisibleStageRoad(unsigned int tileNum);                             // call from the tile which become visible

    static void readRoads(const std::string& dirName, roadMap_t& roadMap, bool doRead = false);
    //static bool writeGlobalObjects(const std::string& fileName, const globalObjectList_t& globalObjectList);

private:
    roadMap_t               roadMap;
    stageRoadChunkListMap_t stageRoadChunkListMap;
    roadRoadChunkSet_t      visibleRoadChunkSet;

    Road*                   editorRoad;


    friend class MenuPageEditor;
    friend class MenuPageEditorRoad;
};


inline Road* RoadManager::getRoad(const std::string& roadName)
{
    roadMap_t::const_iterator rit = roadMap.find(roadName);
    if (rit == roadMap.end())
    {
        return 0;
    }
    return rit->second;
}

inline const RoadManager::roadMap_t& RoadManager::getRoadMap()
{
    return roadMap;
}

#endif // ROADMANAGER_H
