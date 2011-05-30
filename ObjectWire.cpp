
#include "ObjectWire.h"
#include "OffsetObject.h"
#include "OffsetManager.h"
#include "ObjectPoolManager.h"
#include "ObjectPool.h"
#include "ObjectWireGlobalObject.h"
#include "Settings.h"
#include "TheEarth.h"
#include "Terrain_defs.h"
#include "stdafx.h"
#include <assert.h>
#include <string.h>
#include <math.h>



ObjectWireTile::ObjectWireTile(const irr::core::vector2df& apos, const irr::core::vector2di& rpos)
    : apos(apos),
        rpos(rpos),
        objectList()
{
    //printf("ObjectWireTile::ObjectWireTile()\n");
    const unsigned int objectWireNum = Settings::getInstance()->objectWireNum;
    const unsigned int objectWireSize = Settings::getInstance()->objectWireSize;
    const unsigned int objectDensity = Settings::getInstance()->objectDensity;
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
            
        //unsigned int cnt = (rep * num /* Settings::getInstance()->objectDensity*/) / (255/*100*/);

        //printf("%s: %u\n", objectPool->getName().c_str(), cnt);
        const unsigned int req = rep * objectDensity;

        if (req == 0) continue;
            
        for (unsigned int i = 0; i < num; i++)
        {
            irr::core::vector3df objectPos = irr::core::vector3df(
                    (float)(rand()%(objectWireSize*10)) / 10.0f + apos.X,
                    -50.f,
                    (float)(rand()%(objectWireSize*10)) / 10.0f + apos.Y);
                
            irr::video::SColor fineDensity = TheEarth::getInstance()->getTileFineDensity(
                (unsigned int)abs((int)objectPos.X/TILE_FINE_SCALE), (unsigned int)abs((int)objectPos.Z/TILE_FINE_SCALE));
                
            unsigned int newRep = 0;
                
            if ((category & 1) == 1)
            {
                newRep += fineDensity.getRed();
            }
            if ((category & 2) == 2)
            {
                newRep += fineDensity.getGreen();
            }
            if ((category & 4) == 4)
            {
                newRep += fineDensity.getBlue();
            }
            if (rep > 255) rep = 255;
                
            const unsigned int newReq = newRep * objectDensity;
            if (newReq >= 255*100 || (unsigned int)(rand()%(255*100)) < newReq)
            {
                
                objectPos.Y = TheEarth::getInstance()->getHeight(objectPos-OffsetManager::getInstance()->getOffset());
                //printf("opos: %f %f %f\n", objectPos.X, objectPos.Y, objectPos.Z);
                
                if (objectPos.Y > 10.f)
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
}
    
ObjectWireTile::~ObjectWireTile()
{
    for (std::list<OffsetObject*>::iterator it = objectList.begin();
            it != objectList.end();
            it++)
    {
        ObjectPoolManager::getInstance()->putObject(*it);
    }
    objectList.clear();
}



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
      lastWireCenter(),
      globalObjectWire()
{
    const unsigned int objectWireNum = Settings::getInstance()->objectWireNum;
    assert(objectWireNum);
    
    tiles = new ObjectWireTile*[objectWireNum*objectWireNum];
    memset(tiles, 0, objectWireNum*objectWireNum*sizeof(ObjectWireTile*));
}

ObjectWire::~ObjectWire()
{
    if (tiles)
    {
        delete tiles;
        tiles = 0;
    }
     // clear globals
     for (globalObjectWire_t::iterator it = globalObjectWire.begin();
          it != globalObjectWire.end();
          it++)
     {
         for (globalObjectSet_t::const_iterator oit = it->second.begin();
              oit != it->second.end();
              oit++)
         {
             delete (*oit);
         }
         it->second.clear();
     }
     globalObjectWire.clear();
}

bool ObjectWire::update(const irr::core::vector3df& newPos, bool force)
{
    const unsigned int objectWireNum = Settings::getInstance()->objectWireNum;
    const unsigned int objectWireSize = Settings::getInstance()->objectWireSize;
    assert(objectWireNum);
    
    irr::core::vector2di newWireCenter = irr::core::vector2di((int)newPos.X/(int)objectWireSize, (int)newPos.Z/(int)objectWireSize);
    
    if (newWireCenter != lastWireCenter)
    {
        dprintf(MY_DEBUG_NOTE, "ObjectWire::update(): (%d, %d) -> (%d, %d)\n",
            lastWireCenter.X, lastWireCenter.Y, newWireCenter.X, newWireCenter.Y);
        irr::core::vector2di offset = newWireCenter - lastWireCenter;
        ObjectWireTile** newTiles = new ObjectWireTile*[objectWireNum*objectWireNum];
        memset(newTiles, 0, objectWireNum*objectWireNum*sizeof(ObjectWireTile*));
        
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
                    newTiles[newX + objectWireNum*newY]->rpos = irr::core::vector2di(newX, newY);
                }
                else
                {
                    if (tiles[x + objectWireNum*y])
                    {
                        delete tiles[x + objectWireNum*y];
                        tiles[x + objectWireNum*y] = 0;
                        
                        // check globals
                        globalObjectWire_t::const_iterator it = globalObjectWire.find((lastWireCenter.X-(int)(objectWireNum/2)+(int)x) +
                                               (TheEarth::getInstance()->getSizeX() * (lastWireCenter.Y+(int)(objectWireNum/2)-(int)y)));
                        if (it != globalObjectWire.end())
                        {
                            for (globalObjectSet_t::const_iterator oit = it->second.begin();
                                 oit != it->second.end();
                                 oit++)
                            {
                                (*oit)->setVisible(false);
                            }
                        }
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
                            (float)((lastWireCenter.X-(int)(objectWireNum/2)+(int)x)*(int)objectWireSize),
                            (float)((lastWireCenter.Y-1+(int)(objectWireNum/2)-(int)y)*(int)objectWireSize)),
                        irr::core::vector2di(x, y));

                    // check globals
                    globalObjectWire_t::const_iterator it = globalObjectWire.find((lastWireCenter.X-(int)(objectWireNum/2)+(int)x) +
                                           (TheEarth::getInstance()->getSizeX() * (lastWireCenter.Y+(int)(objectWireNum/2)-(int)y)));
                    //printf("check global in %d\n", (lastWireCenter.X-(int)(objectWireNum/2)+(int)x) +
                    //                       (TheEarth::getInstance()->getSizeX() * (lastWireCenter.Y+(int)(objectWireNum/2)-(int)y)));
                    if (it != globalObjectWire.end())
                    {
                        for (globalObjectSet_t::const_iterator oit = it->second.begin();
                             oit != it->second.end();
                             oit++)
                        {
                            (*oit)->setVisible(true);
                            //printf("set visible global object: %p\n", *oit);
                        }
                    }
                }
            }
        }
        return true;
    }
    return false;
}

ObjectWireGlobalObject* ObjectWire::addGlobalObject(const std::string& objectPoolName,
    const irr::core::vector3df& apos,
    const irr::core::vector3df& rot,
    const irr::core::vector3df& scale)
{
    ObjectPool* objectPool = 0;
    ObjectPoolManager::objectPoolMap_t::const_iterator poolIt = ObjectPoolManager::getInstance()->getObjectPoolMap().find(objectPoolName);
    if (poolIt != ObjectPoolManager::getInstance()->getObjectPoolMap().end())
    {
        objectPool = poolIt->second;
    }
    else
    {
        dprintf(MY_DEBUG_WARNING, "no object pool for %s\n", objectPoolName.c_str());
    }
    //assert(objectPool);
    ObjectWireGlobalObject* globalObject = new ObjectWireGlobalObject(objectPool, apos, rot, scale);
    //globalObjectWire[x + (TheEarth::getInstance()->getSizeX() * y)].insert(globalObject);
    //return globalObject;
    return addGlobalObject(globalObject);
}
    
ObjectWireGlobalObject* ObjectWire::addGlobalObject(ObjectWireGlobalObject* globalObject)
{
    const unsigned int objectWireNum2 = Settings::getInstance()->objectWireNum/2;
    const unsigned int objectWireSize = Settings::getInstance()->objectWireSize;
    int x = (int)globalObject->getPos().X / (int)objectWireSize;
    int y = (int)globalObject->getPos().Z / (int)objectWireSize;
    
    globalObjectWire[x + (TheEarth::getInstance()->getSizeX() * y)].insert(globalObject);
    //printf("add global object: %d - %p\n", x + (TheEarth::getInstance()->getSizeX() * y), globalObject);
    if (abs(x-lastWireCenter.X) < objectWireNum2 && abs(y-lastWireCenter.Y) < objectWireNum2)
    {
        globalObject->setVisible(true);
    }
    return globalObject;
}

void ObjectWire::removeGlobalObject(ObjectWireGlobalObject* globalObject, bool deleteObject)
{
    if (globalObject)
    {
        const unsigned int objectWireNum = Settings::getInstance()->objectWireNum;
        const unsigned int objectWireSize = Settings::getInstance()->objectWireSize;
        int x = (int)globalObject->apos.X / (int)objectWireSize;
        int y = (int)globalObject->apos.Z / (int)objectWireSize;
        
        
        globalObjectWire_t::iterator it = globalObjectWire.find(x + (TheEarth::getInstance()->getSizeX() * y));
        if (it != globalObjectWire.end())
        {
            it->second.erase(globalObject);
            if (it->second.empty())
            {
                globalObjectWire.erase(it);
            }
        }
        
        if (deleteObject)
        {
            delete globalObject;
        }
        else
        {
            globalObject->setVisible(false);
        }
    }
}
