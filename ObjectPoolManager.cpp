
#include "ObjectPoolManager.h"

ObjectPoolManager* ObjectPoolManager::objectPoolManager = 0;

void ObjectPoolManager::initialize()
{
    if (objectPoolManager == 0)
    {
        objectPoolManager = new ObjectPoolManager();
    }
}

void ObjectPoolManager::finalize()
{
    if (objectPoolManager)
    {
        delete objectPoolManager;
        objectPoolManager = 0;
    }
}

ObjectPoolManager* ObjectPoolManagergetInstance()
{
    return objectPoolManager;
}


ObjectPoolManager::ObjectPoolManager()
{
}

ObjectPoolManager::~ObjectPoolManager()
{
}
