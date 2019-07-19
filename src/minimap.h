#ifndef MINIMAP_H
#define MINIMAP_H

// defines
#define MINIMAP_COLS 20
#define MINIMAP_ROWS 14
#define MINIMAP_TILE_SIZE 4

#define MINIMAP_X 3 
#define MINIMAP_Y 3

#define MINIMAP_WIDTH (MINIMAP_COLS * MINIMAP_TILE_SIZE)
#define MINIMAP_HEIGHT (MINIMAP_ROWS * MINIMAP_TILE_SIZE)
#define MINIMAP_SCALE_FACTOR (TILEMAP_TILE_SIZE / MINIMAP_TILE_SIZE)

#define MINIMAP_MAX_X (TILEMAP_MAX_X / MINIMAP_SCALE_FACTOR)
#define MINIMAP_MAX_Y (TILEMAP_MAX_Y / MINIMAP_SCALE_FACTOR)

// function prototypes
void minimap(void);

#endif
