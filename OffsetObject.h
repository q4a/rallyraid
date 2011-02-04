#ifndef OFFSETOBJECT_H
#define OFFSETOBJECT_H

#include <irrlicht.h>
#include "hk.h"
#include "OffsetManager.h"

class OffsetObject
{
public:
    OffsetObject();
    OffsetObject(irr::scene::ISceneNode* node);
    OffsetObject(irr::scene::ISceneNode* node, hkpRigidBody* hkBody);
    OffsetObject(irr::scene::ISceneNode* node, bool dynamic);
    OffsetObject(irr::scene::ISceneNode* node, hkpRigidBody* hkBody, bool dynamic);
    
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

    virtual void updatePositionCB(const irr::core::vector3df& newPos)
    {
    }

    virtual ~OffsetObject();
    
    void update(const irr::core::vector3df& offset, const irr::core::vector3df& loffset);
    
    irr::core::vector3df& getPos() {return pos;}

    void addToManager();
    void removeFromManager();
    
private:
    friend class OffsetManager;
    irr::scene::ISceneNode*         node;
    hkpRigidBody*                   hkBody;
    irr::core::vector3df            pos;
    offsetObjectList_t::Iterator    iterator;
    bool                            dynamic;
    OffsetManager*                  offsetManager;
};

#endif // OFFSETOBJECT_H
