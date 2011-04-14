
#ifndef RACEMANAGER_H
#define RACEMANAGER_H

#include <string>
#include <map>
#include <list>
#include <irrlicht.h>
#include "ObjectWire.h"


#define RACES_DIR                   (std::string("data/races"))

#define RACE_DIR(RACE)              (RACES_DIR + std::string("/") + RACE)
#define RACE_CFG                    (std::string("race.cfg"))

#define DAY_DIR(RACE, DAY)          (RACE_DIR(RACE) + std::string("/") + DAY) 
#define DAY_CFG                     (std::string("day.cfg"))

#define STAGE_DIR(RACE, DAY, STAGE) (DAY_DIR(RACE, DAY) + std::string("/") + STAGE) 
#define STAGE_CFG                   (std::string("stage.cfg"))

#define DESCRIPTION_TXT             (std::string("description.txt"))
#define COMPETITORS_CFG             (std::string("competitors.cfg"))
#define OBJECTS_CFG                 (std::string("objects.cfg"))

class Race;
class Day;
class Stage;
class ObjectWireGlobalObject;

class RaceManager
{
public:
    static void initialize();
    static void finalize();
    
    static RaceManager* getInstance() {return raceManager;}

    typedef std::map<std::string, Race*> raceMap_t;
    typedef std::list<ObjectWireGlobalObject*> globalObjectList_t;

private:
    static RaceManager* raceManager;

private:
    RaceManager();
    ~RaceManager();

    void read();

public:
    Race* getRace(const std::string& raceName); // inline
    const raceMap_t& getRaceMap(); // inline
    const std::string& getCurrentRaceName(); // inline
    const std::string& getCurrentDayName(); // inline
    const std::string& getCurrentStageName(); // inline
    Race* getCurrentRace(); // inline
    Day* getCurrentDay(); // inline
    Stage* getCurrentStage(); // inline


    static void readShortDescription(const std::string& fileName, std::string& shortDescription);
    static bool writeShortDescription(const std::string& fileName, const std::string& shortDescription);
    
    static void readGlobalObjects(const std::string& fileName, globalObjectList_t& globalObjectList);
    static bool writeGlobalObjects(const std::string& fileName, const globalObjectList_t& globalObjectList);
    static void clearGlobalObjects(globalObjectList_t& globalObjectList);
    
    static void addGlobalObjectsToObjectWire(const globalObjectList_t& globalObjectList);
    static void removeGlobalObjectsFromObjectWire(const globalObjectList_t& globalObjectList);

private:
    raceMap_t   raceMap;

    std::string currentRaceName;
    std::string currentDayName;
    std::string currentStageName;
    Race*       currentRace;
    Day*        currentDay;
    Stage*      currentStage;

    Race*       editorRace;
    Day*        editorDay;
    Stage*      editorStage;


    friend class MenuPageEditor;
    friend class MenuPageEditorRace;
    friend class MenuPageEditorDay;
    friend class MenuPageEditorStage;
};


inline Race* RaceManager::getRace(const std::string& raceName)
{
    raceMap_t::const_iterator rit = raceMap.find(raceName);
    if (rit == raceMap.end())
    {
        return 0;
    }
    return rit->second;
}

inline const RaceManager::raceMap_t& RaceManager::getRaceMap()
{
    return raceMap;
}

inline const std::string& RaceManager::getCurrentRaceName()
{
    return currentRaceName;
}

inline const std::string& RaceManager::getCurrentDayName()
{
    return currentDayName;
}

inline const std::string& RaceManager::getCurrentStageName()
{
    return currentStageName;
}

inline Race* RaceManager::getCurrentRace()
{
    return currentRace;
}

inline Day* RaceManager::getCurrentDay()
{
    return currentDay;
}

inline Stage* RaceManager::getCurrentStage()
{
    return currentStage;
}

#endif // RACEMANAGER_H
