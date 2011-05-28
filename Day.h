
#ifndef DAY_H
#define DAY_H

#include <string>
#include <map>
#include <irrlicht.h>
#include "RaceManager.h"
#include "RoadManager.h"


class Stage;

class Day
{
public:
    Day(Race* parent, const std::string& raceName, const std::string& dayName, bool& ret);
    ~Day();
    
    typedef std::map<std::string, Stage*> stageMap_t;

private:
    bool read();
    bool readCfg();
    bool readStages();
    void readShortDescription();
    void readGlobalObjects();

    bool write();
    bool writeCfg();
    bool writeShortDescription();
    bool writeGlobalObjects();

public:
    Stage* getStage(const std::string& stageName); // inline
    const stageMap_t& getStageMap(); // inline
    const std::string& getName(); // inline
    const std::string& getLongName(); // inline
    const std::string& getShortDescription(); // inline
    const RoadManager::roadMap_t& getRoadMap(); // inline

    void activate();
    void deactivate();

private:
    Race*                           parent;
    std::string                     raceName;
    std::string                     dayName;
    std::string                     dayLongName;
    std::string                     shortDescription;
    stageMap_t                      stageMap;
    RaceManager::globalObjectList_t globalObjectList;
    bool                            active;
    RoadManager::roadMap_t          roadMap;
    

    friend class RaceManager;
    friend class MenuPageEditor;
    friend class MenuPageEditorRace;
    friend class MenuPageEditorDay;
    friend class MenuPageEditorStage;
};


inline Stage* Day::getStage(const std::string& stageName)
{
    stageMap_t::const_iterator sit = stageMap.find(stageName);
    if (sit == stageMap.end())
    {
        return 0;
    }
    return sit->second;
}

inline const Day::stageMap_t& Day::getStageMap()
{
    return stageMap;
}

inline const std::string& Day::getName()
{
    return dayName;
}

inline const std::string& Day::getLongName()
{
    return dayLongName;
}

inline const std::string& Day::getShortDescription()
{
    return shortDescription;
}

inline const RoadManager::roadMap_t& Day::getRoadMap()
{
    return roadMap;
}

#endif // DAY_H
