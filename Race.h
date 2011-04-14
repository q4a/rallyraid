
#ifndef RACE_H
#define RACE_H

#include <string>
#include <map>
#include <irrlicht.h>
#include "RaceManager.h"


class Day;
class Competitor;

class Race
{
public:
    Race(const std::string& raceName, bool& ret);
    ~Race();
    
    typedef std::map<std::string, Day*> dayMap_t;
    typedef std::map<unsigned int, Competitor*> competitorMap_t;

private:
    bool read();
    bool readCfg();
    bool readDays();
    void readShortDescription();
    bool readCompetitors();
    void readGlobalObjects();
    
    bool write();
    bool writeCfg();
    bool writeShortDescription();
    bool writeGlobalObjects();

public:
    Day* getDay(const std::string& dayName); // inline
    const dayMap_t& getDayMap(); // inline
    const std::string& getName(); // inline
    const std::string& getLongName(); // inline
    const std::string& getShortDescription(); // inline
    const competitorMap_t& getCompetitorMap(); // inline

    void activate();
    void deactivate();

private:
    std::string                     raceName;
    std::string                     raceLongName;
    std::string                     shortDescription;
    dayMap_t                        dayMap;
    competitorMap_t                 competitorMap;
    RaceManager::globalObjectList_t globalObjectList;
    bool                            active;

    
    friend class RaceManager;
    friend class MenuPageEditor;
    friend class MenuPageEditorRace;
    friend class MenuPageEditorDay;
    friend class MenuPageEditorStage;
};


inline Day* Race::getDay(const std::string& dayName)
{
    dayMap_t::const_iterator dit = dayMap.find(dayName);
    if (dit == dayMap.end())
    {
        return 0;
    }
    return dit->second;
}

inline const Race::dayMap_t& Race::getDayMap()
{
    return dayMap;
}

inline const std::string& Race::getLongName()
{
    return raceLongName;
}

inline const std::string& Race::getShortDescription()
{
    return shortDescription;
}

inline const Race::competitorMap_t& Race::getCompetitorMap()
{
    return competitorMap;
}

#endif // RACE_H
