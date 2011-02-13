#ifndef TERRAIN_DEFS_H
#define TERRAIN_DEFS_H

#define CATDIV                  100
#define TILE_POINTS_NUM         32
#define TILE_SCALE              64
#define TILE_SIZE               (TILE_POINTS_NUM * TILE_SCALE)
#define TILE_HSIZE              (TILE_SIZE / 2)
#define TILE_SCALE_F            64.f
#define TILE_SIZE_F             ((float)TILE_POINTS_NUM * TILE_SCALE_F)
#define TILE_HSIZE_F            (TILE_SIZE_F * 0.5f)
#define VISUAL_BOX_SIZE_F       512.0f
#define VISUAL_BOX_HSIZE_F      (VISUAL_BOX_SIZE_F * 0.5f)


#endif // TERRAIN_DEFS_H
