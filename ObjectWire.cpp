
#include "ObjectWire.h"
#include "OffsetObject.h"
#include "ObjectPoolManager.h"
#include "Settings.h"
#include <list>
#include <assert.h>
#include <string.h>



class ObjectWireTile
{
private:
    ObjectWireTile(const irr::core::vector2df& apos, const irr::core::vector2di& rpos)
        : apos(apos),
          rpos(rpos),
          objectList()
    {
    
    }
    
    ~ObjectWireTile()
    {
        for (std::list<OffsetObject*>::iterator it = objectList.begin();
             it != objectList.end();
             it++)
        {
            ObjectPoolManager::getInstance()->putObject(*it);
        }
        objectList.clear();
    }

private:
    irr::core::vector2df        apos;
    irr::core::vector2di        rpos;
    std::list<OffsetObject*>    objectList;

private:
    friend class ObjectWire;
};


ObjectWire* ObjectWire::objectWire = 0;

void ObjectWire::initialize()
{
    if (objectWire == 0)
    {
        objectWire = new ObjectWire();
    }
}

void ObjectWire::finalize()
{
    if (objectWire)
    {
        delete objectWire;
        objectWire = 0;
    }
}


ObjectWire::ObjectWire()
    : tiles(0),
      lastWireCenter()
{
    unsigned int objectWireNum = Settings::getInstance()->objectWireNum;
    assert(objectWireNum);
    
    tiles = new ObjectWireTile[objectWireNum*objectWireNum];
    memset(tiles, 0, objectWireNum*objectWireNum*sizeof(ObjectWireTile*))
}

ObjectWire::~ObjectWire()
{
    if (tiles)
    {
        delete tiles;
        tiles = 0;
    }
}

bool ObjectWire::update(const irr::core::vector3df& newPos, bool force = false)
{
    unsigned int objectWireNum = Settings::getInstance()->objectWireNum;
    unsigned int objectWireSize = Settings::getInstance()->objectWireSize;
    assert(objectWireNum);
    
    irr::core::vector2di newWireCenter = irr::core::vector2di((int)newPos.X/objectWireSize, (int)newPos.Z/objectWireSize);
    
    if (newWireCenter != lastWireCenter)
    {
        irr::core::vector2di offset = newWireCenter - lastWireCenter;
        ObjectWireTile* newTiles = new ObjectWireTile[objectWireNum*objectWireNum];
        memset(newTiles, 0, objectWireNum*objectWireNum*sizeof(ObjectWireTile*))
        
        // search for overlap in old tiles, if no overlap delete
        for (unsigned int x = 0; x < objectWireNum; x++)
        {
            for (unsigned int y = 0; y < objectWireNum; y++)
            {
                unsigned int newX = x - offset.X;
                unsigned int newY = y + offset.Y;
                if (newX < objectWireNum && newY < objectWireNum && tiles[x + objectWireNum*y])
                {
                    // overlap
                    newTiles[newX + objectWireNum*newY] = tiles[x + objectWireNum*y];
                    newTiles[newX + objectWireNum*newY].rpos = irr::core::vector2di(newX, newY);
                }
                else
                {
                    if (tiles[x + objectWireNum*y])
                    {
                        delete tiles[x + objectWireNum*y];
                        tiles[x + objectWireNum*y] = 0;
                    }
                }
            }
        }
        
        delete [] tiles;
        tiles = newTiles;
        lastWireCenter = newWireCenter;
        
        // search for non-filed newTiles, create new
        for (unsigned int x = 0; x < objectWireNum; x++)
        {
            for (unsigned int y = 0; y < objectWireNum; y++)
            {
                if (tiles[x + objectWireNum*y] == 0)
                {
                    tiles[x + objectWireNum*y] = new ObjectWireTile(
                        irr::core::vector2df(
                            (float)((lastWireCenter.X-objectWireNum/2+x)*objectWireSize),
                            (float)((lastWireCenter.Y+objectWireNum/2-y)*objectWireSize)),
                        irr::core::vector2di(x, y));
                }
            }
        }
    }
}
