
#include "ObjectWireGlobalObject.h"
#include "ObjectPool.h"
#include "OffsetObject.h"
#include "OffsetManager.h"
#include "TheEarth.h"

ObjectWireGlobalObject::ObjectWireGlobalObject(ObjectPool* objectPool,
    const irr::core::vector3df& apos,
    const irr::core::vector3df& rot,
    const irr::core::vector3df& scale)
    : objectPool(objectPool),
      apos(apos),
      rot(rot),
      scale(scale),
      visible(false),
      offsetObject(0)
{
}

ObjectWireGlobalObject::~ObjectWireGlobalObject()
{
    setVisible(false);
}

void ObjectWireGlobalObject::setVisible(bool p_visible)
{
    if (visible == p_visible) return;
    
    visible = p_visible;
    
    if (visible)
    {
        if (objectPool)
        {
            irr::core::vector3df objectPos = irr::core::vector3df(apos.X, -50.f, apos.Z);
            objectPos.Y = TheEarth::getInstance()->getHeight(objectPos-OffsetManager::getInstance()->getOffset());
            //printf("opos: %f %f %f\n", objectPos.X, objectPos.Y, objectPos.Z);
            if (objectPos.Y > 10.f)
            {
                offsetObject = objectPool->getObject(objectPos);
                if (offsetObject)
                {
                    offsetObject->getNode()->setRotation(rot);
                }
            }
        }
    }
    else
    {
        if (objectPool && offsetObject)
        {
            objectPool->putObject(offsetObject);
        }
    }
    
    updateVisible();
}
