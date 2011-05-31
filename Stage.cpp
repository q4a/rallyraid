
#include "Race.h"
#include "Day.h"
#include "Stage.h"
#include "ConfigFile.h"
#include "stdafx.h"


Stage::Stage(Day* parent, const std::string& raceName, const std::string& dayName, const std::string& stageName, bool& ret)
    : parent(parent),
      raceName(raceName),
      dayName(dayName),
      stageName(stageName),
      stageLongName(),
      shortDescription(),
      globalObjectList(),
      itinerPointList(),
      active(false),
      roadMap()
{
    ret = read();
}

Stage::~Stage()
{
    deactivate();
    RaceManager::clearGlobalObjects(globalObjectList);
    RoadManager::clearRoadMap(roadMap);
    ItinerManager::clearItinerPointList(itinerPointList);
}

bool Stage::read()
{
    bool ret = readCfg();
    if (ret)
    {
        readShortDescription();
        readGlobalObjects();
        readItinerPointList();
        RoadManager::readRoads(STAGE_ROADS(raceName, dayName, stageName), roadMap, false, true);
    }
    return ret;
}

bool Stage::readCfg()
{
    ConfigFile cf;
    cf.load(STAGE_DIR(raceName, dayName, stageName)+"/"+STAGE_CFG);

    dprintf(MY_DEBUG_NOTE, "Read stage cfg file: %s / %s / %s\n", raceName.c_str(), dayName.c_str(), stageName.c_str());
    // Go through all sections & settings in the file
    ConfigFile::SectionIterator seci = cf.getSectionIterator();

    std::string secName, keyName, valName;
    while (seci.hasMoreElements())
    {
        secName = seci.peekNextKey();
        dprintf(MY_DEBUG_NOTE, "\tSection s: %s\n", secName.c_str());
        ConfigFile::SettingsMultiMap *settings = seci.getNext();
        ConfigFile::SettingsMultiMap::iterator i;
        for (i = settings->begin(); i != settings->end(); ++i)
        {
            keyName = i->first;
            valName = i->second;
            dprintf(MY_DEBUG_NOTE, "\t\tkey: %s, value: %s\n", keyName.c_str(), valName.c_str());

            if (keyName == "long_name")
            {
                stageLongName = valName;
            }/* else if (keyName == "cache_objects")
            {
                cacheObjects = StringConverter::parseBool(valName, true);
            }*/
        }
    }
    return true;
}

void Stage::readShortDescription()
{
    RaceManager::readShortDescription(STAGE_DIR(raceName, dayName, stageName) + "/" + DESCRIPTION_TXT, shortDescription);
}

void Stage::readGlobalObjects()
{
    RaceManager::readGlobalObjects(STAGE_DIR(raceName, dayName, stageName) + "/" + OBJECTS_CFG, globalObjectList);
}

void Stage::readItinerPointList()
{
    ItinerManager::readItinerPointList(STAGE_DIR(raceName, dayName, stageName) + "/" + ITINER_CFG, itinerPointList);
}

bool Stage::write()
{
    bool ret = writeCfg();
    ret &= writeShortDescription();
    ret &= writeGlobalObjects();
    ret &= writeItinerPointList();
    return ret;
}

bool Stage::writeCfg()
{
    FILE* f;
    int ret = 0;
    std::string fileName = STAGE_DIR(raceName, dayName, stageName)+"/"+STAGE_CFG;
    errno_t error = fopen_s(&f, fileName.c_str(), "w");
    if (error)
    {
        printf("unable to open file for write %s\n", fileName.c_str());
        return false;
    }

    ret = fprintf(f, "long_name=%s\n", stageLongName.c_str());

    fclose(f);
    return true;
}

bool Stage::writeShortDescription()
{
    return RaceManager::writeShortDescription(STAGE_DIR(raceName, dayName, stageName) + "/" + DESCRIPTION_TXT, shortDescription);
}

bool Stage::writeGlobalObjects()
{
    return RaceManager::writeGlobalObjects(STAGE_DIR(raceName, dayName, stageName) + "/" + OBJECTS_CFG, globalObjectList);
}

bool Stage::writeItinerPointList()
{
    return ItinerManager::writeItinerPointList(STAGE_DIR(raceName, dayName, stageName) + "/" + ITINER_CFG, itinerPointList);
}

void Stage::activate()
{
    RoadManager::getInstance()->addStageRoad(roadMap);
    
    if (active) return;

    RaceManager::addGlobalObjectsToObjectWire(globalObjectList);
    ItinerManager::addItinerPointListToObjectWire(itinerPointList);
    active = true;
}

void Stage::deactivate()
{
    if (!active) return;

    RaceManager::removeGlobalObjectsFromObjectWire(globalObjectList);
    ItinerManager::removeItinerPointListFromObjectWire(itinerPointList);
    active = false;
}
