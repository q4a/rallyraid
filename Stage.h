
#ifndef STAGE_H
#define STAGE_H

#include <string>
#include <map>
#include <irrlicht.h>
#include "RaceManager.h"
#include "RoadManager.h"


class Stage
{
public:
    Stage(Day* parent, const std::string& raceName, const std::string& dayName, const std::string& stageName, bool& ret);
    ~Stage();
    
private:
    bool read();
    bool readCfg();
    void readShortDescription();
    void readGlobalObjects();

    bool write();
    bool writeCfg();
    bool writeShortDescription();
    bool writeGlobalObjects();

public:
    const std::string& getName(); // inline
    const std::string& getLongName(); // inline
    const std::string& getShortDescription(); // inline
    const RoadManager::roadMap_t& getRoadMap(); // inline

    void activate();
    void deactivate();

private:
    Day*                            parent;
    std::string                     raceName;
    std::string                     dayName;
    std::string                     stageName;
    std::string                     stageLongName;
    std::string                     shortDescription;
    RaceManager::globalObjectList_t globalObjectList;
    bool                            active;
    RoadManager::roadMap_t          roadMap;
    

    friend class RaceManager;
    friend class MenuPageEditor;
    friend class MenuPageEditorRace;
    friend class MenuPageEditorDay;
    friend class MenuPageEditorStage;
};

inline const std::string& Stage::getName()
{
    return stageName;
}

inline const std::string& Stage::getLongName()
{
    return stageLongName;
}

inline const std::string& Stage::getShortDescription()
{
    return shortDescription;
}

inline const RoadManager::roadMap_t& Stage::getRoadMap()
{
    return roadMap;
}

#endif // STAGE_H
