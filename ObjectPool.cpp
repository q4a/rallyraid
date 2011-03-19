
#include "ObjectPool.h"
#include "OffsetObject.h"
#include "TheGame.h"
#include "Settings.h"
#include "Shaders.h"

/*
    enum ObjectType
    {
        Standard = 0,
        Vehicle
    };

    enum Category
    {
        All = 0,
        Normal = 1,
        HalfDesert = 2,
        Desert = 4
    };

    typedef std::list<OffsetObject*> objectList_t;
*/

ObjectPool::ObjectPool(const std::string& meshFilename, const std::string& textureFilename,
                       const std::string& texture2Filename,
                       bool physics, ObjectType objectType,
                       const std::string& materialName, const std::string& material2Name,
                       unsigned int num, unsigned int category, float friction, float mass,
                       const irr::core::vector3df& center)
    : objectList(),
      objectMesh(0),
      hkShape(0),
      category(category),
      texture(0),
      texture2(0),
      objectType(objectType),
      material(irr::video::EMT_SOLID),
      material2(irr::video::EMT_SOLID),
      friction(friction),
      mass(mass),
      center(center),
      num(num)
{
    if (meshFilename == "" && objectType == Grass)
    {
        objectMesh = generateGrassMesh();
    } else if (meshFilename.rfind(".mso") != std::string::npos)
    {
        objectMesh = readMySimpleObject(meshFilename);
    }
    else
    {
        objectMesh = TheGame::getInstance()->getSmgr()->getMesh(meshFilename.c_str());
    }

    if (physics)
    {
        hkShape = calculateCollisionMesh(objectMesh);
    }

    if (textureFilename != "")
    {
        texture = TheGame::getInstance()->getDriver()->getTexture(textureFilename.c_str());
    }
    if (texture2Filename != "")
    {
        texture2 = TheGame::getInstance()->getDriver()->getTexture(texture2Filename.c_str());
    }
    if (materialName != "")
    {
        material = TheGame::getInstance()->getShaders()->materialMap[materialName];
    }
    if (material2Name != "")
    {
        material2 = TheGame::getInstance()->getShaders()->materialMap[material2Name];
    }

    if (Settings::getInstance()->preloadObjects)
    {
        for (unsigned int i = 0; i < num; i++)
        {
            OffsetObject* offsetObject = createNewInstance();
            objectList.push_back(offsetObject);
        }
    }
}

ObjectPool::~ObjectPool()
{
    for (objectList_t::iterator it = objectList.begin();
         it != objectList.end();
         it++)
    {
        //printf("drop node: %p\n", (*it)->getNode());
        //(*it)->getNode()->drop();
        (*it)->getNode()->remove();
        delete *it;
    }
    objectList.clear();
    if (hkShape)
    {
        hk::lock();
        hkShape->removeReference();
        hk::unlock();
        hkShape = 0;
    }
}

OffsetObject* ObjectPool::getObject(const irr::core::vector3df& apos, const irr::core::vector3df& scale, bool addToOffsetManager)
{
    OffsetObject* offsetObject = 0;
    if (objectList.size() > 0)
    {
        offsetObject = *objectList.begin();
        objectList.erase(objectList.begin());
    }
    else
    {
        offsetObject = createNewInstance();
    }

    //offsetObject->setPos(apos);
    offsetObject->getNode()->setPosition(apos);
    offsetObject->getNode()->setScale(scale);
    offsetObject->getNode()->setMaterialType(material);
    if (TheGame::getInstance()->getShaders()->getSupportedSMVersion() < 2)
    {
        offsetObject->getNode()->setMaterialFlag(irr::video::EMF_LIGHTING, Settings::getInstance()->nonshaderLight);
    }
    else
    {
        offsetObject->getNode()->setMaterialFlag(irr::video::EMF_LIGHTING, false);
    }
    printf("-------------- texture: %p\n", texture);
    offsetObject->getNode()->setMaterialTexture(0, texture);

    if (hkShape)
    {
        hk::lock();
        hkpRigidBodyCinfo groundInfo;
        groundInfo.m_shape = hkShape;
        groundInfo.m_position.set(apos.X, apos.Y, apos.Z);
        if (objectType == Vehicle)
        {
            groundInfo.m_motionType = hkpMotion::MOTION_BOX_INERTIA;
            groundInfo.m_mass = mass;
            // TODO
            //groundInfo.m_position.set(0.0f, 0.0f, 0.0f);
            groundInfo.m_inertiaTensor.setDiagonal(1.0f, 1.0f, 1.0f);
            groundInfo.m_centerOfMass.set(center.X, center.Y, center.Z);
        }
        else
        {
            groundInfo.m_motionType = hkpMotion::MOTION_FIXED;
        }
        groundInfo.m_friction = friction;
        hkpRigidBody* hkBody = new hkpRigidBody(groundInfo);
        if (objectType != Vehicle)
        {
            hk::hkWorld->addEntity(hkBody);
            hkpPropertyValue val(1);
            hkBody->addProperty(hk::materialType::treeId, val);
        }
        else
        {
            hkpPropertyValue val(1);
            hkBody->addProperty(hk::materialType::vehicleId, val);
        }
        hk::unlock();
        offsetObject->setBody(hkBody);
        //hkBody->activate();
    }

    offsetObject->setPool(this);

    if (addToOffsetManager)
    {
        offsetObject->addToManager();
    }
    offsetObject->getNode()->setVisible(true);
    return offsetObject;
}

void ObjectPool::putObject(OffsetObject* object)
{
    object->getNode()->setVisible(false);
    hkpRigidBody* hkBody = object->getBody();
    if (hkBody)
    {
        hk::lock();
        hkBody->removeReference();
        if (objectType != Vehicle)
        {
            hk::hkWorld->removeEntity(hkBody);
        }
        hk::unlock();
        hkBody = 0;
        object->setBody(0);
        object->setPool(0);
    }
    object->removeFromManager();
    if (Settings::getInstance()->cacheObjects)
    {
        objectList.push_back(object);
    }
    else
    {
        object->getNode()->remove();
        object->setNode(0);
        delete object;
    }
}

OffsetObject* ObjectPool::createNewInstance()
{
    /*
        switch (objectType)
        {
            case Standard:
            case Grass:
            case Vehicle:
            case Tree:
            {
                break;
            }
        }
    */
    irr::scene::IAnimatedMeshSceneNode* objectNode = TheGame::getInstance()->getSmgr()->addAnimatedMeshSceneNode(objectMesh);
    objectNode->setVisible(false);
    objectNode->setMaterialTexture(0, texture);
    objectNode->setMaterialTexture(1, texture2);
    objectNode->setMaterialType(material);
    // TODO
    OffsetObject* offsetObject = new OffsetObject(objectNode, objectType == Vehicle);

    return offsetObject;
}

irr::scene::SAnimatedMesh* ObjectPool::readMySimpleObject(const std::string& meshFilename)
{
    FILE* f;
    unsigned int numOfVertices, numOfPols;
    float x,y,z,tu,tv;
    irr::u32 r,g,b;
    int ret, index;
    irr::u32 verInd;
    irr::video::S3DVertex vtx;
    vtx.Color.set(255,255,255,255);
    vtx.Normal.set(0,1,0);

#ifdef MSO_DEBUG
    printf("Read my simple object: %s\n", name);
#endif
    
    errno_t error = fopen_s(&f, meshFilename.c_str(), "r");
    
    if (error)
    {
        printf("my simple object file unable to open: %s\n", meshFilename.c_str());
        return 0;
    }
    
    irr::scene::SMeshBuffer* buffer = new irr::scene::SMeshBuffer();
    irr::scene::SMesh* mesh = new irr::scene::SMesh();
    
#ifdef MSO_DEBUG
    printf("read vertices\n");
#endif    
    ret = fscanf_s(f, "vertices\n%u\n", &numOfVertices);
    if (ret <= 0)
    {
       printf("error reading %s ret %d errno %d\n", meshFilename.c_str(), ret, errno);
       fclose(f);
       return 0;
    }

#ifdef MSO_DEBUG
    printf("vertices: %u\n", numOfVertices);
#endif    

    for (unsigned int ind = 0; ind < numOfVertices; ind++)
    {
#ifdef MSO_DEBUG
    printf("read a vertex\n");
#endif    
        ret = fscanf_s(f, "%d %f %f %f %f %f %u %u %u\n", &index, &x, &y, &z, &tu, &tv, &r, &g, &b);
        if (ret <= 0)
        {
           printf("error reading %s ret %d errno %d\n", meshFilename.c_str(), ret, errno);
           fclose(f);
           return 0;
        }
#ifdef MSO_DEBUG
    printf("vertex read done\n");
#endif    
        vtx.Pos.X = x;            
        vtx.Pos.Z = z;
        vtx.Pos.Y = y; 
        vtx.TCoords.X = tu;
        vtx.TCoords.Y = tv;
        vtx.Color.set(255,r,g,b);
        buffer->Vertices.push_back(vtx);
    }
#ifdef MSO_DEBUG
    printf("read polygons number\n");
#endif    
    ret = fscanf_s(f, "polygons\n%u\n", &numOfPols);
    if (ret <= 0)
    {
       printf("error reading %s ret %d errno %d\n", meshFilename.c_str(), ret, errno);
       fclose(f);
       return 0;
    }
#ifdef MSO_DEBUG
    printf("polygons: %u\n", numOfPols);
#endif    
    for (unsigned int ind = 0; ind < numOfPols*3; ind++)
    {
#ifdef MSO_DEBUG
    printf("read a poly part\n");
#endif    
        ret = fscanf_s(f, "%u\n", &verInd);
        if (ret <= 0)
        {
           printf("error reading %s ret %d errno %d\n", meshFilename.c_str(), ret, errno);
           fclose(f);
           return 0;
        }
#ifdef MSO_DEBUG
    printf("read a poly part done\n");
#endif    
        if (verInd >= numOfVertices)
        {
           printf("!!!!! verInd >= numOfVertices: %d > %u\n", verInd, numOfVertices);
        }
        buffer->Indices.push_back(verInd);
    }

#ifdef MSO_DEBUG
    printf("renormalize\n");
#endif    
    for (irr::s32 ind=0; ind<(irr::s32)buffer->Indices.size(); ind+=3)
    {
        irr::core::plane3d<irr::f32> p(
            buffer->Vertices[buffer->Indices[ind+0]].Pos,
            buffer->Vertices[buffer->Indices[ind+1]].Pos,
            buffer->Vertices[buffer->Indices[ind+2]].Pos);
        p.Normal.normalize();

        buffer->Vertices[buffer->Indices[ind+0]].Normal = p.Normal;
        buffer->Vertices[buffer->Indices[ind+1]].Normal = p.Normal;
        buffer->Vertices[buffer->Indices[ind+2]].Normal = p.Normal;
    }
#ifdef MSO_DEBUG
    printf("renormalize done\n");
#endif    
   
    buffer->recalculateBoundingBox();

    irr::scene::SAnimatedMesh* animatedMesh = new irr::scene::SAnimatedMesh();
    mesh->addMeshBuffer(buffer);
    mesh->recalculateBoundingBox();
    animatedMesh->addMesh(mesh);
    animatedMesh->recalculateBoundingBox();

    mesh->drop();
    buffer->drop();

    fclose(f);
#ifdef MSO_DEBUG
    printf("read done return %p\n", animatedMesh);
#endif    

    return animatedMesh;
}

hkpShape* ObjectPool::calculateCollisionMesh(irr::scene::IAnimatedMesh* objectMesh, bool /*box*/)
{
    hkpShape* hkShape = 0;
    if (objectMesh == 0) return hkShape;
    
    int sizeOfBuffers = 0;
    for (unsigned int i = 0; i < objectMesh->getMeshBufferCount(); i++)
    {
        if (objectMesh->getMeshBuffer(i)->getVertexType() != irr::video::EVT_STANDARD)
        {
            printf("ojject %u type missmatch %u\n", i, objectMesh->getMeshBuffer(i)->getVertexType());
            assert(0);
            return hkShape;
        }
            
        sizeOfBuffers += objectMesh->getMeshBuffer(i)->getVertexCount();
    }
        
    float* my_vertices = new float[sizeOfBuffers*4];
    int cursor = 0;
        
    for (unsigned int i = 0; i < objectMesh->getMeshBufferCount();i++)
    {
        irr::scene::IMeshBuffer* mb = objectMesh->getMeshBuffer(i);
        irr::video::S3DVertex* mb_vertices = (irr::video::S3DVertex*)mb->getVertices();
        for (unsigned int j = 0; j < mb->getVertexCount(); j++)
        {
            my_vertices[(cursor+j)*4] = mb_vertices[j].Pos.X /* scale.X */;
            my_vertices[(cursor+j)*4+1] = mb_vertices[j].Pos.Y /* scale.Y */;
            my_vertices[(cursor+j)*4+2] = mb_vertices[j].Pos.Z /* scale.Z */;
            my_vertices[(cursor+j)*4+3] = 0.0f;
        }
        cursor += mb->getVertexCount();
    }
    
    hk::lock();
    hkStridedVertices stridedVerts;
    stridedVerts.m_numVertices = sizeOfBuffers;
    stridedVerts.m_striding = 4*sizeof(float);
    stridedVerts.m_vertices = my_vertices;

    hkShape = new hkpConvexVerticesShape(stridedVerts);
    hk::unlock();

    delete [] my_vertices;

    return hkShape;
}

#define GENERATED_GRASS_SIZE 10
irr::scene::SAnimatedMesh* ObjectPool::generateGrassMesh()
{
    irr::scene::SMeshBuffer* buffer = new irr::scene::SMeshBuffer();
    irr::scene::SMesh* mesh = new irr::scene::SMesh();
    const float width = 3.f;//((float)(rand() % 10) + 1.f) / 5.f;
    const float height = 2.f;//((float)(rand() % 20) + 2.f) / 6.f;
    irr::video::S3DVertex vtx;
    vtx.Color.set(255,255,255,255);
    vtx.Normal.set(0,1,0);
            
    for (int j = 0; j < 7; j++)
    {
        float offx = ((float)(rand() % GENERATED_GRASS_SIZE - (GENERATED_GRASS_SIZE / 2)));
        float offz = ((float)(rand() % GENERATED_GRASS_SIZE - (GENERATED_GRASS_SIZE / 2)));
                        
        vtx.Pos.X = offx-width/2.f;
        vtx.Pos.Z = offz+0.f;
        vtx.Pos.Y = 0.f;
        vtx.TCoords.X = 0.f;
        vtx.TCoords.Y = 1.f;
        buffer->Vertices.push_back(vtx);
                
        vtx.Pos.X = offx+width/2.f;
        vtx.Pos.Z = offz+0.f;
        vtx.Pos.Y = 0.f;
        vtx.TCoords.X = 1.f/*width/3.f*/;
        vtx.TCoords.Y = 1.f;
        buffer->Vertices.push_back(vtx);
                
        vtx.Pos.X = offx+width/2.f;
        vtx.Pos.Z = offz+0.f;
        vtx.Pos.Y = height;
        vtx.TCoords.X = 1.f/*width/3.f*/;
        vtx.TCoords.Y = 0.f;
        buffer->Vertices.push_back(vtx);
                
        vtx.Pos.X = offx-width/2.f;
        vtx.Pos.Z = offz+0.f;
        vtx.Pos.Y = height;
        vtx.TCoords.X = 0.f;
        vtx.TCoords.Y = 0.f;
        buffer->Vertices.push_back(vtx);
                
    // other dir
        vtx.Pos.X = offx+0.f;
        vtx.Pos.Z = offz-width/2.f;
        vtx.Pos.Y = 0.f;
        vtx.TCoords.X = 0.f;
        vtx.TCoords.Y = 1.f;
        buffer->Vertices.push_back(vtx);
                
        vtx.Pos.X = offx+0.f;
        vtx.Pos.Z = offz+width/2.f;
        vtx.Pos.Y = 0.f;
        vtx.TCoords.X = 1.f/*width/3.f*/;
        vtx.TCoords.Y = 1.f;
        buffer->Vertices.push_back(vtx);
                
        vtx.Pos.X = offx+0.f;
        vtx.Pos.Z = offz+width/2.f;
        vtx.Pos.Y = height;
        vtx.TCoords.X = 1.f/*width/3.f*/;
        vtx.TCoords.Y = 0.f;
        buffer->Vertices.push_back(vtx);
                
        vtx.Pos.X = offx+0.f;
        vtx.Pos.Z = offz-width/2.f;
        vtx.Pos.Y = height;
        vtx.TCoords.X = 0.f;
        vtx.TCoords.Y = 0.f;
        buffer->Vertices.push_back(vtx);
                
        buffer->Indices.push_back((j*8)+0);
        buffer->Indices.push_back((j*8)+1);
        buffer->Indices.push_back((j*8)+2);
                
        buffer->Indices.push_back((j*8)+0);
        buffer->Indices.push_back((j*8)+2);
        buffer->Indices.push_back((j*8)+3);
                
        buffer->Indices.push_back((j*8)+0);
        buffer->Indices.push_back((j*8)+2);
        buffer->Indices.push_back((j*8)+1);
                
        buffer->Indices.push_back((j*8)+0);
        buffer->Indices.push_back((j*8)+3);
        buffer->Indices.push_back((j*8)+2);
    // other dir
        buffer->Indices.push_back((j*8)+4);
        buffer->Indices.push_back((j*8)+5);
        buffer->Indices.push_back((j*8)+6);
                
        buffer->Indices.push_back((j*8)+4);
        buffer->Indices.push_back((j*8)+6);
        buffer->Indices.push_back((j*8)+7);
                
        buffer->Indices.push_back((j*8)+4);
        buffer->Indices.push_back((j*8)+6);
        buffer->Indices.push_back((j*8)+5);
                
        buffer->Indices.push_back((j*8)+4);
        buffer->Indices.push_back((j*8)+7);
        buffer->Indices.push_back((j*8)+6);
    }
    //printf("debug %d norm start\n", i);
                    
    for (irr::s32 ind=0; ind<(irr::s32)buffer->Indices.size(); ind+=3)
    {
        irr::core::plane3d<irr::f32> p(
            buffer->Vertices[buffer->Indices[ind+0]].Pos,
            buffer->Vertices[buffer->Indices[ind+1]].Pos,
            buffer->Vertices[buffer->Indices[ind+2]].Pos);
        p.Normal.normalize();
        
        buffer->Vertices[buffer->Indices[ind+0]].Normal = p.Normal;
        buffer->Vertices[buffer->Indices[ind+1]].Normal = p.Normal;
        buffer->Vertices[buffer->Indices[ind+2]].Normal = p.Normal;
    }
            
    //printf("debug %d norm end\n", i);
                   
    buffer->recalculateBoundingBox();
            
    irr::scene::SAnimatedMesh* animatedMesh = new irr::scene::SAnimatedMesh();
    mesh->addMeshBuffer(buffer);
    mesh->recalculateBoundingBox();
    animatedMesh->addMesh(mesh);
    animatedMesh->recalculateBoundingBox();
            
    mesh->drop();
    buffer->drop();
                    
    return animatedMesh;
}
