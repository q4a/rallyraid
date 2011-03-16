
#ifndef OBJECTWIRE_H
#define OBJECTWIRE_H

#include <irrlicht.h>

class ObjectWireTile;

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

private:
    ObjectWireTile*         tiles;
    irr::core::vector2di    lastWireCenter;
};

#endif // OBJECTWIRE_H
