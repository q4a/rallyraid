
#ifndef RACE_H
#define RACE_H

#include <string>
#include <map>
#include <irrlicht.h>

class Day;

class Race
{
public:
    Race(const std::string& raceName, bool& ret);
    ~Race();
    
    typedef std::map<std::string, Day*> dayMap_t;

private:
    bool read();
    bool readCfg();
    bool readDays();
    void readShortDescription();

public:
    Day* getDay(const std::string& dayName); // inline
    const dayMap_t& getDayMap(); // inline
    const std::string& getName(); // inline
    const std::string& getLongName(); // inline
    const std::string& getShortDescription(); // inline

private:
    std::string     raceName;
    std::string     raceLongName;
    std::string     shortDescription;
    dayMap_t        dayMap;
    
    friend class RaceManager;
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

#endif // RACE_H
