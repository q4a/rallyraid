
#ifndef OBJECTWIRE_H
#define OBJECTWIRE_H

#include <irrlicht.h>
#include <map>
#include <set>
#include <string>

class ObjectWireTile;
class ObjectWireGlobalObject;

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
    bool update(const irr::core::vector3df& newPos, bool force = false);

    ObjectWireGlobalObject* addGlobalObject(const std::string& objectPoolName,
        const irr::core::vector3df& apos,
        const irr::core::vector3df& rot = irr::core::vector3df(0.0f, 0.0f, 0.0f),
        const irr::core::vector3df& scale = irr::core::vector3df(1.0f, 1.0f, 1.0f));
        
    void removeGlobalObject(ObjectWireGlobalObject* globalObject, bool deleteObject = true);
    
private:
    typedef std::set<ObjectWireGlobalObject*> globalObjectSet_t;
    typedef std::map<int, globalObjectSet_t> globalObjectWire_t;

    ObjectWireTile**        tiles;
    irr::core::vector2di    lastWireCenter;
    globalObjectWire_t      globalObjectWire;
};

#endif // OBJECTWIRE_H
