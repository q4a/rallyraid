
#include "ObjectWireGlobalObject.h"
#include "ObjectPool.h"
#include "OffsetObject.h"
#include "OffsetManager.h"
#include "TheEarth.h"
#include "TheGame.h"

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

void ObjectWireGlobalObject::editorRender(bool last)
{
    irr::video::IVideoDriver* driver = TheGame::getInstance()->getDriver();
    irr::core::vector3df renderPos = apos - OffsetManager::getInstance()->getOffset();

    irr::core::vector3df min = renderPos;
    irr::core::vector3df max = renderPos;
    irr::video::SColor color(255, 255, 255, 0);
    min.X -= 2.f;
    min.Z -= 2.f;
    max.X += 2.f;
    max.Y += 20.f;
    max.Z += 2.f;

    if (last) color.setGreen(0);

    driver->draw3DBox(irr::core::aabbox3df(min, max), color);
}
