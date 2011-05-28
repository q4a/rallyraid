
#ifndef ROAD_H
#define ROAD_H

#include <string>
#include <vector>
#include <irrlicht.h>

class RoadType;

struct RoadPoint
{
    irr::core::vector3df    p;
    int                     radius;
    irr::video::SColor      color;
};

class Road
{
public:
    typedef std::vector<RoadPoint> roadPointVector_t;

public:
    Road(const std::string& roadFilename, bool& ret, bool global, bool read = false);
    Road(const std::string& roadFilename,
         const std::string& roadName,
         const std::string& roadDataFilename,
         RoadType*          roadType,
         bool               global);
    ~Road();

    bool isLoaded(); // inline
    const std::string& getName(); // inline

private:
    bool readHeader();
    bool writeHeader();
    bool readData();
    bool writeData();

    // only called by the editor
    void addRoadFarPoint(const irr::core::vector3df& pos);
    void addRoadFarPointBegin(const irr::core::vector3df& pos);
    void removeRoadPoint();
    void removeRoadPointBegin();
    
private:
    // only called by the addFarPoint
    void addRoadPoint(const irr::core::vector3df& pos);
    void addRoadPointBegin(const irr::core::vector3df& pos);

private:
    std::string             roadFilename;
    std::string             roadName;
    std::string             roadDataFilename;

    RoadType*               roadType;
    bool                    loaded;
    bool                    global;

    roadPointVector_t       roadPointVector;


    friend class RoadManager;
    friend class MenuPageEditor;
    friend class MenuPageEditorRoad;
    friend class MenuPageEditorRace;
    friend class MenuPageEditorDay;
    friend class MenuPageEditorStage;
};

inline bool Road::isLoaded()
{
    return loaded;
}

inline const std::string& Road::getName()
{
    return roadName;
}

#endif // ROAD_H
