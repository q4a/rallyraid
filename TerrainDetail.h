#ifndef TERRAINDETAIL_H
#define TERRAINDETAIL_H

#include <irrlicht.h>
#include "Terrain.h"
#include "Terrain_defs.h"


class TerrainDetail : public Terrain
{
public:
    TerrainDetail(const irr::core::vector3di& posi, TheEarth* earth);
    virtual ~TerrainDetail();

    virtual void load(TheEarth* earth);
    //virtual void setVisible(bool visible);

    float get(int x, int y); // inline

private:
    float get(int pos); // inline
    void set(int x, int y, float val); // inline
    void set(int pos, float val); // inline

private:
    float* fineHeights;
    

    friend class HeightFieldHelper;
};

inline float TerrainDetail::get(int x, int y)
{
    return fineHeights[(x+TILE_FINE_RATE) + ((TILE_FINE_POINTS_NUM+(3*TILE_FINE_RATE))*(y+TILE_FINE_RATE))];
}

inline float TerrainDetail::get(int pos)
{
    return fineHeights[pos];
}

inline void TerrainDetail::set(int x, int y, float val)
{
    fineHeights[(x+TILE_FINE_RATE) + ((TILE_FINE_POINTS_NUM+(3*TILE_FINE_RATE))*(y+TILE_FINE_RATE))] = val;
}

inline void TerrainDetail::set(int pos, float val)
{
    fineHeights[pos] = val;
}
#endif // TERRAINDETAIL_H