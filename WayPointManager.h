
#ifndef WAYPOINTMANAGER_H
#define WAYPOINTMANAGER_H

#include <irrlicht.h>
#include <set>
#include <map>
#include <string>
#include <list>


class WayPoint;

class WayPointManager
{
public:
    typedef std::list<WayPoint*> wayPointList_t;

    static void initialize();
    static void finalize();
    static WayPointManager* getInstance() {return wayPointManager;}
private:
    static WayPointManager* wayPointManager;

private:
    WayPointManager();
    ~WayPointManager();
    
public:
    bool update(const irr::core::vector3df& newPos, bool force = false);
    
    void addActiveWayPoint(WayPoint* WayPoint);    // inline
    void removeActiveWayPoint(WayPoint* WayPoint); // inline

    static void readWayPointList(const std::string& WPListFilename, wayPointList_t& wayPointList);
    static bool writeWayPointList(const std::string& WPListFilename, const wayPointList_t& wayPointList);
    static void clearWayPointList(wayPointList_t& wayPointList);

    static void addWayPointListToObjectWire(const wayPointList_t& wayPointList);
    static void removeWayPointListFromObjectWire(const wayPointList_t& wayPointList);

private:
    static void editorRenderWayPointList(const wayPointList_t& wayPointList);
    
private:
    typedef std::set<WayPoint*> wayPointSet_t;

    wayPointSet_t       activeWayPointSet;


    friend class MenuPageEditor;
    friend class MenuPageEditorDay;
    friend class MenuPageEditorStage;
};


inline void WayPointManager::addActiveWayPoint(WayPoint* wayPoint)
{
    activeWayPointSet.insert(wayPoint);
}

inline void WayPointManager::removeActiveWayPoint(WayPoint* wayPoint)
{
    activeWayPointSet.erase(wayPoint);
}

#endif // WAYPOINTMANAGER_H
