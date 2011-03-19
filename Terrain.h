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
    return terrain->getHeight(x, z);
}

#endif // TERRAIN_H
