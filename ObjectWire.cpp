
#include "ObjectWire.h"
#include "OffsetObject.h"
#include "ObjectPoolManager.h"
#include "ObjectPool.h"
#include "Settings.h"
#include "TheEarth.h"
#include "Terrain_defs.h"
#include "stdafx.h"
#include <list>
#include <assert.h>
#include <string.h>
#include <math.h>



class ObjectWireTile
{
private:
    ObjectWireTile(const irr::core::vector2df& apos, const irr::core::vector2di& rpos)
        : apos(apos),
          rpos(rpos),
          objectList()
    {
        const unsigned int objectWireNum = Settings::getInstance()->objectWireNum;
        const unsigned int objectWireSize = Settings::getInstance()->objectWireSize;
        // TODO: we need more detailed density maps.
        irr::video::SColor density = TheEarth::getInstance()->getEarthDensity(
            (unsigned int)abs((int)apos.X/TILE_SIZE), (unsigned int)abs((int)apos.Y/TILE_SIZE));

        const ObjectPoolManager::objectPoolMap_t& objectPoolMap = ObjectPoolManager::getInstance()->getObjectPoolMap();
        
        for (ObjectPoolManager::objectPoolMap_t::const_iterator opIt = objectPoolMap.begin();
             opIt != objectPoolMap.end();
             opIt++)
        {
            ObjectPool* objectPool = opIt->second;
            unsigned int num = objectPool->getNum();
            unsigned int category = objectPool->getCategory();
            
            unsigned int rep = 0;
            
            if ((category & 1) == 1)
            {
                rep += density.getRed();
            }
            if ((category & 2) == 2)
            {
                rep += density.getGreen();
            }
            if ((category & 4) == 4)
            {
                rep += density.getBlue();
            }
            if (rep > 255) rep = 255;
            
            unsigned int cnt = (rep * num * Settings::getInstance()->objectDensity) / (255*100);
            
            for (unsigned int i = 0; i < cnt; i++)
            {
                irr::core::vector3df objectPos = irr::core::vector3df(
                    (float)(rand()%(objectWireSize*10)) / 10.0f + apos.X,
                    -50.f,
                    (float)(rand()%(objectWireSize*10)) / 10.0f + apos.Y);
                
                objectPos.Y = TheEarth::getInstance()->getHeight(objectPos);
                
                if (objectPos.Y > 0.f)
                {                
                    OffsetObject* oo = objectPool->getObject(objectPos);
                    if (oo)
                    {
                        objectList.push_back(oo);
                    }
                }
            }
        }
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
    const unsigned int objectWireNum = Settings::getInstance()->objectWireNum;
    const unsigned int objectWireSize = Settings::getInstance()->objectWireSize;
    assert(objectWireNum);
    
    irr::core::vector2di newWireCenter = irr::core::vector2di((int)newPos.X/objectWireSize, (int)newPos.Z/objectWireSize);
    
    if (newWireCenter != lastWireCenter)
    {
        dprintf(MY_DEBUG_NOTE, "ObjectWire::update(): (%d, %d) -> (%d, %d)\n",
            lastWireCenter.X, lastWireCenter.Y, newWireCenter.X, newWireCenter.Y);
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
