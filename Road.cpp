
#include "Road.h"
#include "ConfigFile.h"
#include "RoadType.h"
#include "RoadTypeManager.h"
#include "RoadManager.h"

#include "OffsetManager.h"
#include "TheEarth.h"

#include "stdafx.h"
#include <assert.h>


Road::Road(const std::string& roadFilename, bool& ret, bool global, bool read)
    : roadFilename(roadFilename),
      roadName(),
      roadDataFilename(),
      roadType(0),
      loaded(false),
      global(global),
      roadPointVector()
{
    ret = readHeader();
    if (ret && read)
    {
        readData();
    }
}

Road::Road(const std::string&   roadFilename,
           const std::string&   roadName,
           const std::string&   roadDataFilename,
           RoadType*            roadType,
           bool                 global)
    : roadFilename(roadFilename),
      roadName(roadName),
      roadDataFilename(roadDataFilename),
      roadType(roadType),
      global(global),
      roadPointVector()
{
}

Road::~Road()
{
}

bool Road::readHeader()
{
    ConfigFile cf;
    cf.load(roadFilename);

    dprintf(MY_DEBUG_NOTE, "Read road cfg file: %s\n", roadFilename.c_str());
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

            if (keyName == "name")
            {
                roadName = valName;
            } else if (keyName == "type")
            {
                roadType = RoadTypeManager::getInstance()->getRoadType(valName);
            } else if (keyName == "data")
            {
                roadDataFilename = valName;
            }
        }
    }
    return roadType != 0 && roadName.size() > 0;
}

bool Road::writeHeader()
{
    if (roadType == 0)
    {
        printf("unable to save road cfg, no road type is given");
        return false;
    }

    FILE* f;
    int ret = 0;
    errno_t error = fopen_s(&f, roadFilename.c_str(), "w");
    if (error)
    {
        printf("unable to open road cfg file for write %s\n", roadFilename.c_str());
        return false;
    }

    ret = fprintf(f, "name=%s\n", roadName.c_str());
    ret = fprintf(f, "type=%s\n", roadType->getName().c_str());
    ret = fprintf(f, "data=%s\n", roadDataFilename.c_str());

    fclose(f);
    return true;
}

bool Road::readData()
{
    FILE* f;
    int ret = 0;
    float f1, f2, f3;
    int radius;
    unsigned int color;
    unsigned long pointNum = 0;

    loaded = true;

    if (!roadPointVector.empty())
    {
        dprintf(MY_DEBUG_WARNING, "road data already exists for road %s\n", roadName.c_str());
        return true;
    }

    errno_t error = fopen_s(&f, roadDataFilename.c_str(), "r");
    if (error)
    {
        printf("road data file unable to open: %s\n", roadDataFilename.c_str());
        return false;
    }
    
    ret = fscanf_s(f, "%lu\n", &pointNum);
    if (ret < 1)
    {
        printf("road data unable to read number of points: %s\n", roadDataFilename.c_str());
        fclose(f);
        return false;
    }

    roadPointVector.resize(pointNum);
    unsigned int i = 0;
    while (true)
    {
        ret = fscanf_s(f, "%f %f %f %d %u\n", &f1, &f2, &f3, &radius, &color);
        if (ret < 5 )
        {
            break;
        }
        assert(i < pointNum);
        roadPointVector[i].p = irr::core::vector3df(f1, f2, f3);
        roadPointVector[i].radius = radius;
        roadPointVector[i].color.color = color;
        i++;
    }
    fclose(f);
    return true;
}

bool Road::writeData()
{
    FILE* f;
    int ret = 0;
    errno_t error = fopen_s(&f, roadDataFilename.c_str(), "w");
    if (error)
    {
        printf("unable to open road data file for write %s\n", roadDataFilename.c_str());
        return false;
    }

    ret = fprintf(f, "%lu\n", roadPointVector.size());
    for (unsigned int i = 0; i < roadPointVector.size(); i++)
    {
        ret = fprintf(f, "%f %f %f %d %u\n",
            roadPointVector[i].p.X, roadPointVector[i].p.Y, roadPointVector[i].p.Z, roadPointVector[i].radius, roadPointVector[i].color.color);
    }

    fclose(f);
    return true;
}

void Road::addRoadFarPoint(const irr::core::vector3df& pos)
{
    irr::core::vector3df apos = pos + OffsetManager::getInstance()->getOffset();
    if (!roadPointVector.empty())
    {
        irr::core::vector3df bp = roadPointVector.back().p;
        irr::core::vector3df dir = apos - bp;
        irr::core::vector3df tmpp;
        float dist = dir.getLength();
        while (dist > 4.f)
        {
            dir.setLength(4.f);
            tmpp = bp + dir;
            
            tmpp.Y = TheEarth::getInstance()->getHeight(tmpp - OffsetManager::getInstance()->getOffset());
            addRoadPoint(tmpp);
            
            bp = tmpp;
            dir = apos - bp;
            dist = dir.getLength();
        }
    }
    addRoadPoint(apos);
}

void Road::addRoadFarPointBegin(const irr::core::vector3df& pos)
{
    irr::core::vector3df apos = pos + OffsetManager::getInstance()->getOffset();
    if (!roadPointVector.empty())
    {
        irr::core::vector3df bp = roadPointVector.front().p;
        irr::core::vector3df dir = apos - bp;
        irr::core::vector3df tmpp;
        float dist = dir.getLength();
        while (dist > 4.f)
        {
            dir.setLength(4.f);
            tmpp = bp + dir;
            
            tmpp.Y = TheEarth::getInstance()->getHeight(tmpp - OffsetManager::getInstance()->getOffset());
            addRoadPointBegin(tmpp);
            
            bp = tmpp;
            dir = apos - bp;
            dist = dir.getLength();
        }
    }
    addRoadPointBegin(apos);
}

void Road::removeRoadPoint()
{
    if (!loaded)
    {
        readData();
    }

    if (!roadPointVector.empty())
    {
        roadPointVector.pop_back();
    }
}

void Road::removeRoadPointBegin()
{
    if (!loaded)
    {
        readData();
    }

    if (!roadPointVector.empty())
    {
        roadPointVector.erase(roadPointVector.begin());
    }
}


void Road::addRoadPoint(const irr::core::vector3df& pos)
{
    if (!loaded)
    {
        readData();
    }

    RoadPoint roadPoint;

    roadPoint.p = pos;
    roadPoint.color = RoadManager::getInstance()->editorColor;
    roadPoint.radius = RoadManager::getInstance()->editorRadius;

    roadPointVector.push_back(roadPoint);
}

void Road::addRoadPointBegin(const irr::core::vector3df& pos)
{
    if (!loaded)
    {
        readData();
    }

    RoadPoint roadPoint;

    roadPoint.p = pos;
    roadPoint.color = RoadManager::getInstance()->editorColor;
    roadPoint.radius = RoadManager::getInstance()->editorRadius;

    roadPointVector.insert(roadPointVector.begin(), roadPoint);
}
