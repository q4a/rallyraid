
#include "WayPointManager.h"
#include "WayPoint.h"
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
    : activeWayPointSet()
{
}

WayPointManager::~WayPointManager()
{
    activeWayPointSet.clear();
}

bool WayPointManager::update(const irr::core::vector3df& newPos, bool force)
{
// activeWayPointSet
    return false;
}

/* static */ void WayPointManager::readWayPointList(const std::string& wayPointListFilename, WayPointManager::wayPointList_t& wayPointList)
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
            WayPoint* wayPoint = new WayPoint(apos, num);
            wayPointList.push_back(wayPoint);
            num++;
        }
    }
    //assert(0);
}

/* static */ bool WayPointManager::writeWayPointList(const std::string& wayPointListFilename, const WayPointManager::wayPointList_t& wayPointList)
{
    FILE* f;
    errno_t error = fopen_s(&f, wayPointListFilename.c_str(), "w");
    if (error)
    {
        printf("unable to open file for write %s\n", wayPointListFilename.c_str());
        return false;
    }

    unsigned int id = 1;

    for (wayPointList_t::const_iterator it = wayPointList.begin();
         it != wayPointList.end();
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

/* static */ void WayPointManager::clearWayPointList(WayPointManager::wayPointList_t& wayPointList)
{
    for (wayPointList_t::iterator it = wayPointList.begin();
         it != wayPointList.end();
         it++)
    {
        delete (*it);
    }
    wayPointList.clear();
}

/* static */ void WayPointManager::addWayPointListToObjectWire(const WayPointManager::wayPointList_t& wayPointList)
{
    for (wayPointList_t::const_iterator it = wayPointList.begin();
         it != wayPointList.end();
         it++)
    {
        ObjectWire::getInstance()->addGlobalObject(*it);
    }
}

/* static */ void WayPointManager::removeWayPointListFromObjectWire(const WayPointManager::wayPointList_t& wayPointList)
{
    for (wayPointList_t::const_iterator it = wayPointList.begin();
         it != wayPointList.end();
         it++)
    {
        ObjectWire::getInstance()->removeGlobalObject(*it, false);
    }
}

/* static */ void WayPointManager::editorRenderWayPointList(const WayPointManager::wayPointList_t& wayPointList)
{
    for (wayPointList_t::const_iterator it = wayPointList.begin();
         it != wayPointList.end();
         it++)
    {
        (*it)->editorRender(*it == wayPointList.back());
    }
}
