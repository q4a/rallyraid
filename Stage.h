
#ifndef STAGE_H
#define STAGE_H

#include <string>
#include <map>
#include <irrlicht.h>
#include "RaceManager.h"


class Stage
{
public:
    Stage(const std::string& raceName, const std::string& dayName, const std::string& stageName, bool& ret);
    ~Stage();
    
private:
    bool read();
    bool readCfg();
    void readShortDescription();
    void readGlobalObjects();

public:
    const std::string& getName(); // inline
    const std::string& getLongName(); // inline
    const std::string& getShortDescription(); // inline

    void activate();
    void deactivate();

private:
    std::string                     raceName;
    std::string                     dayName;
    std::string                     stageName;
    std::string                     stageLongName;
    std::string                     shortDescription;
    RaceManager::globalObjectList_t globalObjectList;
    bool                            active;
    
    friend class RaceManager;
};

inline const std::string& Stage::getLongName()
{
    return stageLongName;
}

inline const std::string& Stage::getShortDescription()
{
    return shortDescription;
}


#endif // STAGE_H
