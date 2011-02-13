#include "OffsetManager.h"
#include "OffsetObject.h"
#include "ObjectPoolManager.h"
//#include "TerrainPool.h"
//#include "gameplay.h"
#include <stdio.h>

OffsetManager* OffsetManager::offsetManager = 0;

void OffsetManager::initialize()
{
    if (offsetManager == 0)
    {
        offsetManager = new OffsetManager();
    }
}

void OffsetManager::finalize()
{
    if (offsetManager)
    {
        delete offsetManager;
        offsetManager = 0;
    }
}

OffsetManager::OffsetManager()
    : objects(), offset(), last()
{
}

OffsetManager::~OffsetManager()
{
    while (objects.size() > 0)
    {
        ObjectPoolManager::getInstance()->putObject(*objects.begin());
    }
}

void OffsetManager::addObject(OffsetObject* object)
{
    objects.push_front(object);
    object->iterator = objects.begin();
    object->update(offset, offset);
}

void OffsetManager::removeObject(OffsetObject* object)
{
    if (object->iterator != offsetObjectList_t::Iterator())
    {
        objects.erase(object->iterator);
        object->iterator = offsetObjectList_t::Iterator();
    }
}

//#warning use proper value for the offset unit
#define SMALLTERRAIN_SIZE 2048
bool OffsetManager::update(const irr::core::vector3df& newPos, bool force)
{
    const irr::core::vector3di new_((irr::s32)(newPos.X/SMALLTERRAIN_SIZE), 0, (irr::s32)(newPos.Z/SMALLTERRAIN_SIZE));
    
    if (last.X != new_.X || last.Z != new_.Z || force)
    {
        irr::core::vector3df loffset((float)(new_.X-last.X)*SMALLTERRAIN_SIZE, 0.f, (float)(new_.Z-last.Z)*SMALLTERRAIN_SIZE);
        last.X = new_.X;
        last.Z = new_.Z;
        offset.X = (float)last.X * SMALLTERRAIN_SIZE;
        offset.Z = (float)last.Z * SMALLTERRAIN_SIZE;
        unsigned int updated = 0;
        for (offsetObjectList_t::Iterator it = objects.begin(); it != objects.end(); it++)
        {
            (*it)->update(offset, loffset);
            updated++;
        }
        return true;
    }
    else
    {
        return false;
    }
}

void OffsetManager::reset()
{
    unsigned int reseted = 0;
    for (offsetObjectList_t::Iterator it = objects.begin(); it != objects.end();)
    {
        delete (*it);
        it = objects.erase(it);
        reseted++;
    }
    last = irr::core::vector3di();
    offset = irr::core::vector3df();
}

