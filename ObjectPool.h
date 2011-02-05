
#ifndef OBJECTPOOL_H
#define OBJECTPOOL_H

#include <string>
#include <list>
#include <irrlicht.h>
#include "hk.h"

class OffsetObject;

class ObjectPool
{
public:
    enum ObjectType
    {
        Standard = 0,
        Vehicle,
        Grass
    };

    enum Category
    {
        All = 0,
        Normal = 1,
        HalfDesert = 2,
        Desert = 4
    };

    typedef std::list<OffsetObject*> objectList_t;

public:
    ObjectPool(const std::string& meshFilename, const std::string& textureFilename,
        bool physics, ObjectType objectType, const std::string& material,
        unsigned int num, unsigned int category);
    ~ObjectPool();

    OffsetObject* getObject();
    void putObject(OffsetObject* object);
    
    static irr::scene::SAnimatedMesh* readMySimpleObject(const std::string& meshFilename);
    static hkpShape* calculateCollisionMesh(irr::scene::IAnimatedMesh* objectMesh, bool box = false);
    static irr::scene::SAnimatedMesh* generateGrassMesh();

private:
    objectList_t objectList;

    irr::scene::IAnimatedMesh* objectMesh;
    hkpShape* hkShape;
    int category;
};

#endif // OBJECTPOOL_H
