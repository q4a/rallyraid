#ifndef TERRAIN_DEFS_H
#define TERRAIN_DEFS_H

#define CATDIV                  100
#define TILE_POINTS_NUM         32
#define TILE_SCALE              64
#define TILE_FINE_SCALE         2
#define TILE_FINE_RATE          (TILE_SCALE / TILE_FINE_SCALE)
#define TILE_FINE_POINTS_NUM    (TILE_POINTS_NUM * TILE_FINE_RATE)
#define TILE_DETAIL_SCALE       8
#define TILE_DETAIL_RATE        (TILE_SCALE / TILE_DETAIL_SCALE)
#define TILE_DETAIL_FINE_RATE   (TILE_DETAIL_SCALE / TILE_FINE_SCALE)
#define TILE_DETAIL_POINTS_NUM  (TILE_POINTS_NUM * TILE_DETAIL_RATE)
#define TILE_SIZE               (TILE_POINTS_NUM * TILE_SCALE)
#define TILE_HSIZE              (TILE_SIZE / 2)
#define TILE_SCALE_F            64.f
#define TILE_FINE_SCALE_F       2.f
#define TILE_FINE_RATE_F        (TILE_SCALE_F / TILE_FINE_SCALE_F)
#define TILE_DETAIL_SCALE_F     8.f
#define TILE_DETAIL_RATE_F      (TILE_SCALE_F / TILE_DETAIL_SCALE_F)
#define TILE_SIZE_F             ((float)TILE_POINTS_NUM * TILE_SCALE_F)
#define TILE_HSIZE_F            (TILE_SIZE_F * 0.5f)
#define VISUAL_BOX_SIZE_F       512.0f
#define VISUAL_BOX_HSIZE_F      (VISUAL_BOX_SIZE_F * 0.5f)


#endif // TERRAIN_DEFS_H
