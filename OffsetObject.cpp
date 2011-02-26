
#include "OffsetObject.h"
//#include "TerrainPool.h"
//#include "gameplay.h"
#include <stdio.h>

OffsetObject::dynamicObjectSet_t OffsetObject::dynamicObjectSet;
irr::core::matrix4 OffsetObject::helperMatrix;

OffsetObject::OffsetObject()
    : node(0), hkBody(0), pos(), iterator(), dynamic(false), offsetManager(OffsetManager::getInstance()), pool(0), updateCB(0)
{
}

OffsetObject::OffsetObject(irr::scene::ISceneNode* node)
    : node(node), hkBody(0), pos(), iterator(), dynamic(false), offsetManager(OffsetManager::getInstance()), pool(0), updateCB(0)
{
    if (node)
    {
        pos = node->getPosition();
    }
}

OffsetObject::OffsetObject(irr::scene::ISceneNode* node, hkpRigidBody* hkBody)
    : node(node), hkBody(hkBody), pos(), iterator(), dynamic(false), offsetManager(OffsetManager::getInstance()), pool(0), updateCB(0)
{
    if (node)
    {
        pos = node->getPosition();
    }
}

OffsetObject::OffsetObject(irr::scene::ISceneNode* node, bool dynamic)
    : node(node), hkBody(0), pos(), iterator(), dynamic(dynamic), offsetManager(OffsetManager::getInstance()), pool(0), updateCB(0)
{
    if (dynamic)
    {
        dynamicObjectSet.insert(this);
    }
    /*
    if (node)
    {
        pos = node->getPosition();
    }
    */
}

OffsetObject::OffsetObject(irr::scene::ISceneNode* node, hkpRigidBody* hkBody, bool dynamic)
    : node(node), hkBody(hkBody), pos(), iterator(), dynamic(dynamic), offsetManager(OffsetManager::getInstance()), pool(0), updateCB(0)
{
    if (dynamic)
    {
        dynamicObjectSet.insert(this);
    }
    /*
    if (node)
    {
        pos = node->getPosition();
    }
    */
}

OffsetObject::~OffsetObject()
{
    if (iterator != offsetObjectList_t::Iterator())
    {
        printf("offsetObject is in list but under deletion\n");
        *iterator = 0;
    }
    if (dynamic)
    {
        dynamicObjectSet.erase(this);
    }
}

void OffsetObject::update(const irr::core::vector3df& offset, const irr::core::vector3df& loffset)
{
    if (node)
    {
        if (dynamic)
        {
            pos = node->getPosition();
            //printf("update dyn before: pos: %f, %f loffset: %f, %f\n", pos.X, pos.Z, loffset.X, loffset.Z);
            node->setPosition(pos-loffset);
            //printf("update dyn after: pos: %f, %f \n", node->getPosition().X, node->getPosition().Z);
        }
        else
        {
            node->setPosition(pos-offset);
        }
    }
    if (hkBody)
    {
        irr::core::vector3df tv;
        if (dynamic)
        {
            tv = pos-loffset;
        }
        else
        {
            tv = pos-offset;
        }
        hkBody->setPosition(hkVector4(tv.X, tv.Y, tv.Z));
    }
    if (updateCB)
    {
        updateCB->handleUpdatePos(false);
    }
}

void OffsetObject::addToManager()
{
    offsetManager->addObject(this);
}

void OffsetObject::removeFromManager()
{
    offsetManager->removeObject(this);
}

void OffsetObject::setUpdateCB(OffsetObjectUpdateCB* p_updateCB)
{
    updateCB = p_updateCB;
}
    
/*static*/ void OffsetObject::updateDynamicToPhys()
{
    for (dynamicObjectSet_t::iterator it = dynamicObjectSet.begin();
         it != dynamicObjectSet.end();
         it++)
    {
        if ((*it)->hkBody && (*it)->node)
        {
            hkVector4 hkPos = (*it)->hkBody->getPosition();
            hkQuaternion hkQuat = (*it)->hkBody->getRotation();
            irr::core::vector3df newPos(hkPos(0), hkPos(1), hkPos(2));
            irr::core::quaternion(hkQuat.m_vec(0), hkQuat.m_vec(1), hkQuat.m_vec(2), hkQuat.m_vec(3)).getMatrix(helperMatrix, newPos);

            (*it)->node->setPosition(newPos);
            (*it)->node->setRotation(helperMatrix.getRotationDegrees());
            
            if ((*it)->updateCB)
            {
                (*it)->updateCB->handleUpdatePos(true);
            }
        }
    }
}
