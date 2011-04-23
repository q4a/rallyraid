#ifndef TERRAIN_H
#define TERRAIN_H

/*namespace irr
{
    namespace scene
    {
        class TerrainSceneNode;
    }
}*/
#include <irrlicht.h>
#include "TerrainSceneNode.h"
#include "NTerrainSceneNode.h"
#include "hk.h"
#include <string>

class OffsetObject;
class TheEarth;

class Terrain
{
public:
    Terrain(const std::string& prefix);
    virtual ~Terrain();

    virtual void load(TheEarth* earth) = 0;
    
    void setVisible(bool visible);

    void registerTerrain();
 // inline
    float getHeight(float x, float z); // inline

protected:
    irr::scene::TerrainSceneNode*   terrain;
    hkpShape*                       hkShape;
    OffsetObject*                   offsetObject;
    bool                            visible;
    int                             offsetX;
    int                             offsetY;
    std::string                     prefix;
    irr::video::IImage*             image;
};


inline float Terrain::getHeight(float x, float z)
{
    float h = terrain->getHeight(x, z);
    //printf("getHeight(%f, %f) = %f (%f, %f - %f %f)\n", x ,z, h, terrain->getPosition().X, terrain->getPosition().Z, terrain->getPosition().X+2048.f, terrain->getPosition().Z+2048.f);
    return h;
}

inline void Terrain::registerTerrain()
{
    terrain->OnRegisterSceneNode();
}

#endif // TERRAIN_H
