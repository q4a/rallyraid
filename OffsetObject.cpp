
#include "OffsetObject.h"
//#include "TerrainPool.h"
//#include "gameplay.h"
#include <stdio.h>

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
}

void OffsetObject::update(const irr::core::vector3df& offset, const irr::core::vector3df& loffset)
{
    if (node)
    {
        if (dynamic)
        {
            pos = node->getPosition();
            node->setPosition(pos-loffset);
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
        updateCB->handleUpdatePos(node->getPosition());
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
