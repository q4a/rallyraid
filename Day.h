
#ifndef DAY_H
#define DAY_H

#include <string>
#include <map>
#include <irrlicht.h>

class Stage;

class Day
{
public:
    Day(const std::string& raceName, const std::string& dayName, bool& ret);
    ~Day();
    
    typedef std::map<std::string, Stage*> stageMap_t;

private:
    bool read();
    bool readCfg();
    bool readStages();
    void readShortDescription();

public:
    Stage* getStage(const std::string& stageName); // inline
    const stageMap_t& getStageMap(); // inline
    const std::string& getName(); // inline
    const std::string& getLongName(); // inline
    const std::string& getShortDescription(); // inline

private:
    std::string     raceName;
    std::string     dayName;
    std::string     dayLongName;
    std::string     shortDescription;
    stageMap_t      stageMap;
    
    friend class RaceManager;
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

inline const std::string& Day::getLongName()
{
    return dayLongName;
}

inline const std::string& Day::getShortDescription()
{
    return shortDescription;
}

#endif // DAY_H
