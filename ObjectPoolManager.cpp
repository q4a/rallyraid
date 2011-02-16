
#include "ObjectPoolManager.h"
#include "ObjectPool.h"
#include "OffsetObject.h"
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
    objectPoolMap.clear();
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
        std::string texture2Filename = "";
        bool physics = false;
        std::string material = "";
        std::string material2 = "";
        ObjectPool::ObjectType objectType = ObjectPool::Standard;
        unsigned int category = 0;
        unsigned int num = 1;
        float friction = 0.5f;
        float mass = 0.0f;

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
            } else if (keyName == "texture2")
            {
                texture2Filename = valName;
            } else if (keyName == "physics")
            {
                physics = StringConverter::parseBool(valName, false);
            } else if (keyName == "material")
            {
                material = valName;
            } else if (keyName == "material2")
            {
                material2 = valName;
            } else if (keyName == "type")
            {
                if (valName == "vehicle")
                {
                    objectType = ObjectPool::Vehicle;
                } else if (valName == "grass")
                {
                    objectType = ObjectPool::Grass;
                } else if (valName == "tree")
                {
                    objectType = ObjectPool::Tree;
                }
            } else if (keyName == "category")
            {
                category = StringConverter::parseUnsignedInt(valName, 0);
            } else if (keyName == "num")
            {
                num = StringConverter::parseUnsignedInt(valName, 1);
            } else if (keyName == "friction")
            {
                friction = StringConverter::parseFloat(valName, 0.5f);
            } else if (keyName == "mass")
            {
                mass = StringConverter::parseFloat(valName, 0.0f);
            }
        }
        
        if (objectName != "")
        {
            objectPoolMap[objectName] = new ObjectPool(meshFilename, textureFilename, texture2Filename,
                physics, objectType, material, material2, num, category, friction, mass);
        }
    }
}

OffsetObject* ObjectPoolManager::getObject(const std::string& objectPoolName, const irr::core::vector3df& apos)
{
    return objectPoolMap[objectPoolName]->getObject(apos);
}

void ObjectPoolManager::putObject(OffsetObject* offsetObject)
{
    offsetObject->getPool()->putObject(offsetObject);
}