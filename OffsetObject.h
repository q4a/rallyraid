#ifndef OFFSETOBJECT_H
#define OFFSETOBJECT_H

#include <irrlicht.h>
#include "hk.h"
#include "OffsetManager.h"

class ObjectPool;

class OffsetObjectUpdateCB
{
protected:
    friend class OffsetObject;
    virtual void handleUpdatePos(const irr::core::vector3df& newPos);
};

class OffsetObject
{
public:
    OffsetObject();
    OffsetObject(irr::scene::ISceneNode* node);
    OffsetObject(irr::scene::ISceneNode* node, hkpRigidBody* hkBody);
    OffsetObject(irr::scene::ISceneNode* node, bool dynamic);
    OffsetObject(irr::scene::ISceneNode* node, hkpRigidBody* hkBody, bool dynamic);
    virtual ~OffsetObject();

    irr::scene::ISceneNode* getNode() {return node;}
    void setNode(irr::scene::ISceneNode* p_node)
    {
        node = p_node;
        if (node)
        {
            pos = node->getPosition();
        }
        else
        {
            pos = irr::core::vector3df();
        }
    }

    hkpRigidBody* getBody() {return hkBody;}
    void setBody(hkpRigidBody* p_hkBody)
    {
        if (node)
        {
            hkBody = p_hkBody;
        }
        else
        {
            hkBody = 0;
        }
    }
    
    void update(const irr::core::vector3df& offset, const irr::core::vector3df& loffset);
    
    void setPos(const irr::core::vector3df& apos) {pos = apos;}
    irr::core::vector3df& getPos() {return pos;}

    ObjectPool* getPool() {return pool;}
    void setPool(ObjectPool* p_pool) {pool = p_pool;}

    void addToManager();
    void removeFromManager();

    void setUpdateCB(OffsetObjectUpdateCB* p_updateCB) {updateCB = p_updateCB;}
    
private:
    friend class OffsetManager;
    irr::scene::ISceneNode*         node;
    hkpRigidBody*                   hkBody;
    irr::core::vector3df            pos;
    offsetObjectList_t::Iterator    iterator;
    bool                            dynamic;
    OffsetManager*                  offsetManager;
    ObjectPool*                     pool;
    OffsetObjectUpdateCB*           updateCB;
};

#endif // OFFSETOBJECT_H
