
#ifndef OBJECTWIRE_H
#define OBJECTWIRE_H

#include <irrlicht.h>
#include <map>
#include <set>
#include <string>
#include <list>

class OffsetObject;
class ObjectWireGlobalObject;

class ObjectWireTile
{
private:
    ObjectWireTile(const irr::core::vector2df& apos, const irr::core::vector2di& rpos);
    ~ObjectWireTile();

private:
    irr::core::vector2df        apos;
    irr::core::vector2di        rpos;
    std::list<OffsetObject*>    objectList;


    friend class ObjectWire;
    friend class MenuPageEditor;
};

class ObjectWire
{
public:
    static void initialize();
    static void finalize();
    static ObjectWire* getInstance() {return objectWire;}
private:
    static ObjectWire* objectWire;

public:
    ObjectWire();
    ~ObjectWire();
    
    // called by TheGame::loop()
    bool update(const irr::core::vector3df& newPos, bool force = false);
    
    // called by GamePlay::startGame()
    void reset();

    ObjectWireGlobalObject* addGlobalObject(const std::string& objectPoolName,
        const irr::core::vector3df& apos,
        const irr::core::vector3df& rot = irr::core::vector3df(0.0f, 0.0f, 0.0f),
        const irr::core::vector3df& scale = irr::core::vector3df(1.0f, 1.0f, 1.0f));
        
    ObjectWireGlobalObject* addGlobalObject(ObjectWireGlobalObject* globalObject);

    void removeGlobalObject(ObjectWireGlobalObject* globalObject, bool deleteObject = true);
    
private:
    typedef std::set<ObjectWireGlobalObject*> globalObjectSet_t;
    typedef std::map<int, globalObjectSet_t> globalObjectWire_t;

    ObjectWireTile**        tiles;
    irr::core::vector2di    lastWireCenter;
   // devided by objectWireSize
    globalObjectWire_t      globalObjectWire;


    friend class MenuPageEditor;
};

#endif // OBJECTWIRE_H
