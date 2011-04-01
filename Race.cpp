
#include "Race.h"
#include "RaceManager.h"
#include "Day.h"
#include "ConfigDirectory.h"


Race::Race(const std::string& raceName, bool& ret)
    : raceName(raceName),
      shortDescription(),
      dayMap()
{
    ret = read();
}

Race::~Race()
{
    for (dayMap_t::const_iterator dit = dayMap.begin();
         dit != dayMap.end();
         dit++)
    {
        delete dit->second;
    }
    dayMap.clear();
}
    
bool Race::read()
{
    bool ret = readCfg();
    if (ret)
    {
        ret = readDays();
        if (ret)
        {
            readShortDescription();
        }
    }
    return ret;
}

bool Race::readCfg()
{
    return true;
}

bool Race::readDays()
{
    ConfigDirectory::fileList_t fileList;
    
    dprintf(MY_DEBUG_NOTE, "Read race (%s) directory:\n", raceName.c_str());

    bool ret = ConfigDirectory::load(RACE_DIR(raceName), DAY_CFG, fileList);
    
    if (!ret)
    {
        dprintf(MY_DEBUG_WARNING, "unable to read race (%s) directory\n", raceName.c_str());
        return;
    }
    
    for (ConfigDirectory::fileList_t::const_iterator it = fileList.begin();
         it != fileList.end();
         it++)
    {
        std::string dayName = it->c_str();
        Day* day = new Race(raceName, dayName, ret);
        if (!ret)
        {
            delete day;
        }
        else
        {
            dayMap[dayName] = day;
        }
    }
    return true;
}

void Race::readShortDescription()
{
    RaceManager::readShortDescription(RACE_DIR + "/description.txt", shortDescription);
}
