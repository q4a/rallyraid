
#include "Terrain.h"
#include "Terrain_defs.h"
#include "TheGame.h"
#include "OffsetObject.h"
#include "TheEarth.h"
#include <math.h>
#include "Shaders.h"
#include "Settings.h"


Terrain::Terrain(const std::string& prefix)
    : terrain(0),
      hkShape(0),
      offsetObject(0),
      visible(false),
      offsetX(0),
      offsetY(0),
      prefix(prefix),
      image(0)
{
}

Terrain::~Terrain()
{
    if (visible)
    {
        setVisible(false);
    }
    if (hkShape)
    {
        hk::lock();
        hkShape->removeReference();
        hk::unlock();
        hkShape = 0;
    }
    if (image)
    {
        image->drop();
        image = 0;
    }
    delete terrain;
    delete offsetObject;
}

void Terrain::setVisible(bool p_visible)
{
    if (visible == p_visible) return;

    visible = p_visible;

    if (visible)
    {
        if (hkShape && offsetObject->getBody() == 0)
        {
            //dprintf(MY_DEBUG_NOTE, "create terrain object\n");
            hk::lock();
            hkpRigidBodyCinfo groundInfo;
            groundInfo.m_shape = hkShape;
            groundInfo.m_position.set(terrain->getPosition().X, terrain->getPosition().Y, terrain->getPosition().Z);
            groundInfo.m_motionType = hkpMotion::MOTION_FIXED;
            groundInfo.m_friction = 0.7f;
            groundInfo.m_collisionFilterInfo = hkpGroupFilter::calcFilterInfo(hk::materialType::terrainId);
            hkpRigidBody* hkBody = new hkpRigidBody(groundInfo);
            hkpPropertyValue val(1);
            hkBody->addProperty(hk::materialType::terrainId, val);
            hk::hkWorld->addEntity(hkBody);
            hk::unlock();
            offsetObject->setBody(hkBody);
        }
        //printf("add to manager %s ... \n", prefix.c_str());
        offsetObject->addToManager();
        //printf("add to manager %s ... done\n", prefix.c_str());
        {
            char textureMapPartName[255];
            sprintf_s(textureMapPartName, "%s_textureMapPart_%d_%d", prefix.c_str(), offsetX, offsetY);
            TheGame::getInstance()->getSmgr()->getVideoDriver()->setTextureCreationFlag(irr::video::ETCF_CREATE_MIP_MAPS, false);
            //irr::video::ITexture* texture = TheGame::getInstance()->getSmgr()->getVideoDriver()->addTexture(textureMapPartName, terrain->getGeneratedImage());
            irr::video::ITexture* texture = TheGame::getInstance()->getSmgr()->getVideoDriver()->addTexture(textureMapPartName, image);
            TheGame::getInstance()->getSmgr()->getVideoDriver()->setTextureCreationFlag(irr::video::ETCF_CREATE_MIP_MAPS, true);
            terrain->setMaterialTexture(0, texture);
            terrain->setMaterialTexture(1, TheGame::getInstance()->getDriver()->getTexture("data/earthdata/detailmap_03.png"));
            if (Shaders::getInstance()->getSupportedSMVersion() < 2)
            {
                terrain->setMaterialFlag(irr::video::EMF_LIGHTING, Settings::getInstance()->nonshaderLight);
            }
            else
            {
                terrain->setMaterialFlag(irr::video::EMF_LIGHTING, false);
            }
            //terrain->setMaterialFlag(irr::video::EMF_TEXTURE_WRAP, true);
            terrain->scaleTexture(1.0f, /*TILE_SIZE_F*/(float)TILE_POINTS_NUM);
            terrain->setMaterialType(Shaders::getInstance()->materialMap["terrain"]);
            //image->drop();
        }
        //printf("add to manager %s ... done 2\n", prefix.c_str());
    }
    else
    {
        hkpRigidBody* hkBody = offsetObject->getBody();
        if (hkBody)
        {
            hk::lock();
            hkBody->removeReference();
            hk::hkWorld->removeEntity(hkBody);
            hk::unlock();
            hkBody = 0;
            offsetObject->setBody(0);
        }
        offsetObject->removeFromManager();
    }
    terrain->setVisible(visible);
    //dprintf(MY_DEBUG_NOTE, "terrain: nodepos: %f, %f, %f, bodypos: %f, %f, %f\n",
    //    offsetObject->getNode()->getPosition().X, offsetObject->getNode()->getPosition().Y, offsetObject->getNode()->getPosition().Z,
    //    offsetObject->getBody()->getPosition()(0), offsetObject->getBody()->getPosition()(1), offsetObject->getBody()->getPosition()(2));
}

