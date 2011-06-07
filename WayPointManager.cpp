
#include "WayPointManager.h"
#include "WayPointPoint.h"
#include <irrlicht.h>
#include <set>
#include "ConfigFile.h"
#include "TheGame.h"
#include "stdafx.h"
#include "StringConverter.h"
#include "ObjectWire.h"


WayPointManager* WayPointManager::wayPointManager = 0;

/*static */ void WayPointManager::initialize()
{
    if (wayPointManager == 0)
    {
        wayPointManager = new WayPointManager();
    }
}

/*static */ void WayPointManager::finalize()
{
    if (wayPointManager)
    {
        delete wayPointManager;
        wayPointManager = 0;
    }
}

WayPointManager::WayPointManager()
    : activeWayPointPointSet()
{
}

WayPointManager::~WayPointManager()
{
    activeWayPointPointSet.clear();
}

bool WayPointManager::update(const irr::core::vector3df& newPos, bool force)
{
// activeWayPointPointSet
    return false;
}

/* static */ void WayPointManager::readWayPointPointList(const std::string& wayPointListFilename, WayPointManager::wayPointPointList_t& wayPointPointList)
{
    ConfigFile cf;
    cf.load(wayPointListFilename);

    dprintf(MY_DEBUG_NOTE, "Read wayPoints cfg file: %s\n", wayPointListFilename.c_str());
    // Go through all sections & settings in the file
    ConfigFile::SectionIterator seci = cf.getSectionIterator();

    float globalDistance = 0.f;
    std::string secName, keyName, valName;
    while (seci.hasMoreElements())
    {
        irr::core::vector3df apos;
        unsigned int num = 1;

        secName = seci.peekNextKey();
        dprintf(MY_DEBUG_NOTE, "\tSection: %s\n", secName.c_str());
        ConfigFile::SettingsMultiMap *settings = seci.getNext();
        ConfigFile::SettingsMultiMap::iterator i;
        for (i = settings->begin(); i != settings->end(); ++i)
        {
            keyName = i->first;
            valName = i->second;
            dprintf(MY_DEBUG_NOTE, "\t\tkey: %s, value: %s\n", keyName.c_str(), valName.c_str());

            if (keyName == "pos")
            {
                StringConverter::parseFloat3(valName, apos.X, apos.Y, apos.Z);
            }
        }

        if (!secName.empty())
        {
            WayPointPoint* wayPointPoint = new WayPointPoint(apos, num);
            wayPointPointList.push_back(wayPointPoint);
            num++;
        }
    }
    //assert(0);
}

/* static */ bool WayPointManager::writeWayPointPointList(const std::string& wayPointListFilename, const WayPointManager::wayPointPointList_t& wayPointPointList)
{
    FILE* f;
    errno_t error = fopen_s(&f, wayPointListFilename.c_str(), "w");
    if (error)
    {
        printf("unable to open file for write %s\n", wayPointListFilename.c_str());
        return false;
    }

    unsigned int id = 1;

    for (wayPointPointList_t::const_iterator it = wayPointPointList.begin();
         it != wayPointPointList.end();
         it++)
    {
        if (id != 1)
        {
            fprintf_s(f, "\n");
        }
        fprintf_s(f, "[%u]\n", id);

        fprintf_s(f, "pos=%f %f %f\n", (*it)->getPos().X, (*it)->getPos().Y, (*it)->getPos().Z);

        id++;
    }

    fclose(f);
    return true;
}

/* static */ void WayPointManager::clearWayPointPointList(WayPointManager::wayPointPointList_t& wayPointPointList)
{
    for (wayPointPointList_t::iterator it = wayPointPointList.begin();
         it != wayPointPointList.end();
         it++)
    {
        delete (*it);
    }
    wayPointPointList.clear();
}

/* static */ void WayPointManager::addWayPointPointListToObjectWire(const WayPointManager::wayPointPointList_t& wayPointPointList)
{
    for (wayPointPointList_t::const_iterator it = wayPointPointList.begin();
         it != wayPointPointList.end();
         it++)
    {
        ObjectWire::getInstance()->addGlobalObject(*it);
    }
}

/* static */ void WayPointManager::removeWayPointPointListFromObjectWire(const WayPointManager::wayPointPointList_t& wayPointPointList)
{
    for (wayPointPointList_t::const_iterator it = wayPointPointList.begin();
         it != wayPointPointList.end();
         it++)
    {
        ObjectWire::getInstance()->removeGlobalObject(*it, false);
    }
}

/* static */ void WayPointManager::editorRenderWayPointPointList(const WayPointManager::wayPointPointList_t& wayPointPointList)
{
    for (wayPointPointList_t::const_iterator it = wayPointPointList.begin();
         it != wayPointPointList.end();
         it++)
    {
        (*it)->editorRender(*it == wayPointPointList.back());
    }
}
