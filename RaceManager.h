
#ifndef RACEMANAGER_H
#define RACEMANAGER_H

#include <string>
#include <map>
#include <irrlicht.h>


#define RACES_DIR                   (std::string("data/races"))

#define RACE_DIR(RACE)              (RACES_DIR + std::string("/") + RACE)
#define RACE_CFG                    (std::string("race.cfg"))

#define DAY_DIR(RACE, DAY)          (RACE_DIR(RACE) + std::string("/") + DAY) 
#define DAY_CFG                     (std::string("day.cfg"))

#define STAGE_DIR(RACE, DAY, STAGE) (DAY_DIR(RACE, DAY) + std::string("/") + STAGE) 
#define STAGE_CFG                   (std::string("stage.cfg"))

class Race;

class RaceManager
{
public:
    static void initialize();
    static void finalize();
    
    static RaceManager* getInstance() {return raceManager;}

    typedef std::map<std::string, Race*> raceMap_t;

private:
    static RaceManager* raceManager;

private:
    RaceManager();
    ~RaceManager();

    void read();

public:
    Race* getRace(const std::string& raceName); // inline
    const raceMap_t& getRaceMap(); // inline
    

    static void readShortDescription(const std::string& fileName, std::string& shortDescription);

private:
    raceMap_t raceMap;
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

#endif // RACEMANAGER_H
