
#include "ItinerManager.h"
#include "ItinerPoint.h"
#include <irrlicht.h>
#include <set>
#include "ConfigDirectory.h"
#include "TheGame.h"
#include "stdafx.h"


ItinerManager* ItinerManager::itinerManager = 0;

/*static */ void ItinerManager::initialize()
{
    if (itinerManager == 0)
    {
        itinerManager = new ItinerManager();
    }
}

/*static */ void ItinerManager::finalize()
{
    if (itinerManager)
    {
        delete itinerManager;
        itinerManager = 0;
    }
}

ItinerManager::ItinerManager()
    : activeItinerPointSet(),
      itinerImageMap()
{
    readItinerImages();
}

ItinerManager::~ItinerManager()
{
    activeItinerPointSet.clear();
    itinerImageMap.clear();
}

bool ItinerManager::update(const irr::core::vector3df& newPos, bool force = false)
{
// activeItinerPointSet
    return false;
}

bool ItinerManager::readItinerImages()
{
    ConfigDirectory::fileList_t fileList;
    
    dprintf(MY_DEBUG_NOTE, "Read itiner images directory:\n");

    bool ret = ConfigDirectory::load(ITINERIMAGES_DIR.c_str(), fileList);
    
    if (!ret)
    {
        dprintf(MY_DEBUG_WARNING, "unable to read itiner images directory\n");
        return false;
    }
    
    for (ConfigDirectory::fileList_t::const_iterator it = fileList.begin();
         it != fileList.end();
         it++)
    {
        std::string itinerImage = it->c_str();
        itinerImageMap[itinerImage] = TheGame::getInstance()->getDriver()->getTexture((ITINERIMAGES_DIR+"/"+itinerImage).c_str());
    }
    return true;
}
