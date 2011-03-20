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

class OffsetObject;
class TheEarth;

class Terrain
{
public:
    Terrain(const irr::core::vector3di& posi, TheEarth* earth);
    virtual ~Terrain();

    void load(TheEarth* earth);

    void setVisible(bool visible);

    void registerTerrain();

    float getHeight(float x, float z);

private:
    irr::scene::TerrainSceneNode*   terrain;
    hkpShape*                       hkShape;
    OffsetObject*                   offsetObject;
    bool                            visible;
    int                             offsetX;
    int                             offsetY;
};


inline float Terrain::getHeight(float x, float z)
{
    float h = terrain->getHeight(x, z);
    //printf("getHeight(%f, %f) = %f (%f, %f - %f %f)\n", x ,z, h, terrain->getPosition().X, terrain->getPosition().Z, terrain->getPosition().X+2048.f, terrain->getPosition().Z+2048.f);
    return h;
}

#endif // TERRAIN_H
