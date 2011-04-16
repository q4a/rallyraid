
#ifndef ROAD_H
#define ROAD_H

#include <string>
#include <vector>
#include <irrlicht.h>

class RoadType;

class Road
{
public:
    typedef std::vector<irr::core::vector3df> roadPointVector_t;

public:
    Road(const std::string& roadFilename, bool& ret, bool read = false);
    ~Road();

    bool isLoaded(); // inline
    const std::string& getName(); // inline

private:
    bool readHeader();
    bool writeHeader();
    bool readData();
    bool writeData();

private:
    std::string             roadFilename;
    std::string             roadName;
    std::string             roadDataFilename;

    RoadType*               roadType;
    bool                    loaded;

    roadPointVector_t       roadPointVector;


    friend class RaceManager;
    friend class MenuPageEditor;
    friend class MenuPageEditorRoad;
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
