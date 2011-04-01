
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

private:
    std::string     raceName;
    std::string     shortDescription;
    dayMap_t        dayMap;
};


inline Day* Race::getDay(const std::string& dayName)
{
    dayMap_t dit = dayMap.find(dayName);
    if (dit == dayMap.end())
    {
        return 0;
    }
    return dit->second;
}

inline const dayMap_t& Race::getDayMap()
{
    return dayMap;
}

#endif // RACE_H
