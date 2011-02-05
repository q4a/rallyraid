
#include "ObjectPoolManager.h"
#include "ObjectPool.h"
#include "ConfigFile.h"
#include "stdafx.h"
#include "StringConverter.h"

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

ObjectPoolManager* ObjectPoolManager::getInstance()
{
    return objectPoolManager;
}


ObjectPoolManager::ObjectPoolManager()
    : objectPoolMap()
{
    read();
}

ObjectPoolManager::~ObjectPoolManager()
{
    for (objectPoolMap_t::iterator it = objectPoolMap.begin();
         it != objectPoolMap.end();
         it++)
    {
        delete it->second;
    }
}

void ObjectPoolManager::read()
{

    ConfigFile cf;
    cf.load("data/objects/objects.cfg");

    dprintf(MY_DEBUG_NOTE, "Read objects file:\n");
    // Go through all sections & settings in the file
    ConfigFile::SectionIterator seci = cf.getSectionIterator();

    std::string objectName, keyName, valName;
    while (seci.hasMoreElements())
    {
        std::string meshFilename = "";
        std::string textureFilename = "";
        bool physics = false;
        std::string material;
        ObjectPool::ObjectType objectType = ObjectPool::Standard;
        unsigned int category = 0;
        unsigned int num = 1;

        objectName = seci.peekNextKey();
        dprintf(MY_DEBUG_NOTE, "\tObject: %s\n", objectName.c_str());
        ConfigFile::SettingsMultiMap *settings = seci.getNext();
        for (ConfigFile::SettingsMultiMap::iterator i = settings->begin(); i != settings->end(); ++i)
        {
            keyName = i->first;
            valName = i->second;

            if (keyName == "mesh")
            {
                meshFilename = valName;
            } else if (keyName == "texture")
            {
                textureFilename = valName;
            } else if (keyName == "physics")
            {
                physics = StringConverter::parseBool(valName, false);
            } else if (keyName == "material")
            {
                material = valName;
            } else if (keyName == "type")
            {
                if (valName == "vehicle")
                {
                    objectType = ObjectPool::Vehicle;
                }
            } else if (keyName == "category")
            {
                category = StringConverter::parseUnsignedInt(valName, 0);
            } else if (keyName == "num")
            {
                num = StringConverter::parseUnsignedInt(valName, 1);
            }
        }
        
        if (objectName != "")
        {
            objectPoolMap[objectName] = new ObjectPool(meshFilename, textureFilename, physics, objectType, material, num, category);
        }
    }
}
