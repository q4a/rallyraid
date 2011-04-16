
#include "Road.h"
#include "ConfigFile.h"
#include "RoadType.h"
#include "RoadTypeManager.h"

#include "stdafx.h"
#include <assert.h>


Road::Road(const std::string& roadFilename, bool& ret, bool read)
    : roadFilename(roadFilename),
      roadName(),
      roadDataFilename(),
      roadType(0),
      roadPointVector()
{
    ret = readHeader();
    if (ret && read)
    {
        readData();
    }
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
    unsigned long pointNum = 0;

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
        ret = fscanf_s(f, "%f %f %f\n", &f1, &f2, &f3);
        if (ret < 3 )
        {
            break;
        }
        assert(i < pointNum);
        roadPointVector[i] = irr::core::vector3df(f1, f2, f3);
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
        ret = fprintf(f, "%f %f %f\n", roadPointVector[i].X, roadPointVector[i].Y, roadPointVector[i].Z);
    }

    fclose(f);
    return true;
}
