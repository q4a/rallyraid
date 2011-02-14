
#include "Terrain.h"
#include "Terrain_defs.h"
#include "TheGame.h"
#include "OffsetObject.h"
#include "TheEarth.h"
#include <math.h>
#include "Shaders.h"

class HeightFieldHelper : public hkpSampledHeightFieldShape
{
public:
    HeightFieldHelper(const hkpSampledHeightFieldBaseCinfo& ci, TheEarth* earth, int offsetX, int offsetY)
        : hkpSampledHeightFieldShape(ci),
          earth(earth),
          offsetX(offsetX),
          offsetY(offsetY)
    {
    }
    
    HK_FORCE_INLINE hkReal getHeightAtImpl(int x, int z) const
    {
        return (hkReal)earth->getTileHeight((unsigned int)abs(offsetX+x), (unsigned int)abs(offsetY+z));
    }
    
    /// this should return true if the two triangles share the edge p00-p11
    ///      otherwise it should return false if the triangles shere the edge p01-p10
    HK_FORCE_INLINE hkBool getTriangleFlipImpl() const
    {	
        return true;
    }
    
    virtual void collideSpheres( const CollideSpheresInput& input, SphereCollisionOutput* outputArray) const
    {
        hkSampledHeightFieldShape_collideSpheres(*this, input, outputArray);
    }
private:
    TheEarth* earth;
    int offsetX;
    int offsetY;
};


Terrain::Terrain(const irr::core::vector3di& posi, TheEarth* earth)
    : terrain(0),
      hkShape(0),
      offsetObject(0),
      visible(false),
      offsetX(posi.X / TILE_SCALE),
      offsetY(posi.Y / TILE_SCALE)
{
    terrain = new irr::scene::TerrainSceneNode(0, 
        TheGame::getInstance()->getSmgr(),
        TheGame::getInstance()->getDevice()->getFileSystem(),
        -1,
        4,
        irr::scene::ETPS_17,
        irr::core::vector3df((float)posi.X, 0.0f, (float)posi.Z),
        irr::core::vector3df(),
        irr::core::vector3df(TILE_SCALE, 1.0f, TILE_SCALE));
    terrain->setVisible(false);
    offsetObject = new OffsetObject(terrain);

    hk::lock();
    hkpSampledHeightFieldBaseCinfo ci;
    ci.m_xRes = TILE_POINTS_NUM + 1;
    ci.m_zRes = TILE_POINTS_NUM + 1;
    ci.m_scale.set(TILE_SCALE_F, 1.0f, TILE_SCALE_F);

    hkShape = new HeightFieldHelper(ci, earth, posi.X, posi.Z);
    hk::unlock();
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
    delete terrain;
    delete offsetObject;
}

void Terrain::load(TheEarth* earth)
{
    terrain->loadHeightMap(earth, offsetX, offsetY, TILE_POINTS_NUM);

    if (TheGame::getInstance()->getShaders()->getSupportedSMVersion() < 2)
    {
        terrain->setMaterialFlag(irr::video::EMF_LIGHTING, true);
    }
    else
    {
        terrain->setMaterialFlag(irr::video::EMF_LIGHTING, false);
    }
    terrain->scaleTexture(1.0f, TILE_SIZE_F);
    terrain->setMaterialTexture(0, terrain->getGeneratedTexture());
    terrain->setMaterialTexture(1, TheGame::getInstance()->getDriver()->getTexture("data/earthdata/detailmap.jpg"));
    terrain->setMaterialType(TheGame::getInstance()->getShaders()->materialMap["terrain"]);
}

void Terrain::setVisible(bool p_visible)
{
    if (visible == p_visible) return;

    visible = p_visible;

    if (visible)
    {
        if (hkShape)
        {
            hk::lock();
            hkpRigidBodyCinfo groundInfo;
            groundInfo.m_shape = hkShape;
            groundInfo.m_position.set(offsetObject->getPos().X, offsetObject->getPos().Y, offsetObject->getPos().Z);
            groundInfo.m_motionType = hkpMotion::MOTION_FIXED;
            groundInfo.m_friction = 0.7f;
            hkpRigidBody* hkBody = new hkpRigidBody(groundInfo);
            //hkpPropertyValue val(1);
            //hkBody->addProperty(terrainID, val);
            hk::hkWorld->addEntity(hkBody);
            hk::unlock();
            offsetObject->setBody(hkBody);
        }
        offsetObject->addToManager();
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
}
