
#ifndef ITINERMANAGER_H
#define ITINERMANAGER_H

#include <irrlicht.h>
#include <set>
#include <map>
#include <string>
#include <list>


#define ITINERIMAGES_DIR    (std::string("data/itiner_images"))

class ItinerPoint;

class ItinerManager
{
public:
    typedef std::map<std::string, irr::video::ITexture*> itinerImageMap_t;
    typedef std::list<ItinerPoint*> itinerPointList_t;

    static void initialize();
    static void finalize();
    static ItinerManager* getInstance() {return itinerManager;}
private:
    static ItinerManager* itinerManager;

public:
    ItinerManager();
    ~ItinerManager();
    bool update(const irr::core::vector3df& newPos, bool force = false);
    
    void addActiveItinerPoint(ItinerPoint* itinerPoint);    // inline
    void removeActiveItinerPoint(ItinerPoint* itinerPoint); // inline
    const itinerImageMap_t& getItinerImageMap();            // inline
    irr::video::ITexture* getItinerImage(const std::string& itinerImageName); // inline

    static void readItinerPointList(const std::string& itinerListFilename, itinerPointList_t& itinerPointList);
    static bool writeItinerPointList(const std::string& itinerListFilename, const itinerPointList_t& itinerPointList);
    static void clearItinerPointList(itinerPointList_t& itinerPointList);

    static void addItinerPointListToObjectWire(const itinerPointList_t& itinerPointList);
    static void removeItinerPointListFromObjectWire(const itinerPointList_t& itinerPointList);

private:
    bool readItinerImages();
    static void editorRenderItinerPointList(const itinerPointList_t& itinerPointList);
    
private:
    typedef std::set<ItinerPoint*> itinerPointSet_t;

    itinerPointSet_t    activeItinerPointSet;
    itinerImageMap_t    itinerImageMap;

    float               editorGlobalDistance;
    float               editorLocalDistance;
    std::string         editorItinerImageName;
    std::string         editorDescription;


    friend class MenuPageEditor;
    friend class MenuPageEditorItiner;
};


inline void ItinerManager::addActiveItinerPoint(ItinerPoint* itinerPoint)
{
    activeItinerPointSet.insert(itinerPoint);
}

inline void ItinerManager::removeActiveItinerPoint(ItinerPoint* itinerPoint)
{
    activeItinerPointSet.erase(itinerPoint);
}

inline const ItinerManager::itinerImageMap_t& ItinerManager::getItinerImageMap()
{
    return itinerImageMap;
}

inline irr::video::ITexture* ItinerManager::getItinerImage(const std::string& itinerImageName)
{
    itinerImageMap_t::const_iterator it = itinerImageMap.find(itinerImageName);
    if (it == itinerImageMap.end())
    {
        return 0;
    }
    return it->second;
}

#endif // ITINERMANAGER_H
