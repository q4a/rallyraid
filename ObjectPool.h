
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
        Grass,
        Tree
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
        const std::string& texture2Filename,
        bool physics, ObjectType objectType,
        const std::string& materialName, const std::string& material2Name,
        unsigned int num, unsigned int category,
        float friction);
    ~ObjectPool();

    OffsetObject* getObject(const irr::core::vector3df& apos);
    void putObject(OffsetObject* object);

    OffsetObject* createNewInstance();
    
    static irr::scene::SAnimatedMesh* readMySimpleObject(const std::string& meshFilename);
    static hkpShape* calculateCollisionMesh(irr::scene::IAnimatedMesh* objectMesh, bool box = false);
    static irr::scene::SAnimatedMesh* generateGrassMesh();

private:
    objectList_t objectList;

    irr::scene::IAnimatedMesh*  objectMesh;
    hkpShape*                   hkShape;
    int                         category;

    irr::video::ITexture*       texture;
    irr::video::ITexture*       texture2;
    ObjectType                  objectType;
    irr::video::E_MATERIAL_TYPE material;
    irr::video::E_MATERIAL_TYPE material2;
    float                       friction;
};

#endif // OBJECTPOOL_H
