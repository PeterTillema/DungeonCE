#include <math.h>
#include <setjmp.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tice.h>

#include <fileioc.h>
#include <graphx.h>
#include <keypadc.h>
#include <debug.h>

#include "defines.h"
#include "gfx/dungeon.h"
#include "gfx/tiles_gfx.h"
#include "maingameloop.h"
#include "tilemap/tilemapdata.h"

gfx_tilemap_t tilemap;
gfx_tilemap_t minimapTilemap;
player_setup_t player_setup;

void main(void) {
	uint8_t i;
	gfx_sprite_t **minimapTileset;
	gfx_sprite_t **minimapTilesetPointers;

	if (!dungeon_init()) { abort(); }

	// Malloc stuff
	minimapTilesetPointers = (gfx_sprite_t **) malloc(sizeof(uint8_t) * tileset_tiles_num);
	minimapTileset = (gfx_sprite_t **) malloc(tileset_tiles_num * (MINIMAP_TILE_SIZE * MINIMAP_TILE_SIZE + 2));

	// Initialize the tilemap structure
	tilemap.map = tilemap_map;
	tilemap.tiles = tileset_tiles;
	tilemap.type_width = gfx_tile_32_pixel;
	tilemap.type_height = gfx_tile_32_pixel;
	tilemap.tile_height = TILEMAP_TILE_SIZE;
	tilemap.tile_width = TILEMAP_TILE_SIZE;
	tilemap.draw_height = TILEMAP_DRAW_HEIGHT;
	tilemap.draw_width = TILEMAP_DRAW_WIDTH;
	tilemap.height = TILEMAP_HEIGHT;
	tilemap.width = TILEMAP_WIDTH;
	tilemap.y_loc = Y_OFFSET;
	tilemap.x_loc = X_OFFSET;

	// Initialize the minimap tilemap structure
	minimapTilemap.map = tilemap_map;
	minimapTilemap.tiles = minimapTilesetPointers;
	minimapTilemap.type_width = gfx_tile_no_pow2;
	minimapTilemap.type_height = gfx_tile_no_pow2;
	minimapTilemap.tile_height = MINIMAP_TILE_SIZE;
	minimapTilemap.tile_width = MINIMAP_TILE_SIZE;
	minimapTilemap.draw_height = MINIMAP_ROWS;
	minimapTilemap.draw_width = MINIMAP_COLS;
	minimapTilemap.height = TILEMAP_HEIGHT;
	minimapTilemap.width = TILEMAP_WIDTH;

	// Generate all the scaled sprites for the minimap tileset cache
	for (i = 0; i < tileset_tiles_num; i++) {
		minimapTilesetPointers[i] = &minimapTileset[i][0];
		minimapTileset[i]->width = MINIMAP_TILE_SIZE;
		minimapTileset[i]->height = MINIMAP_TILE_SIZE;

		gfx_ScaleSprite(tilemap.tiles[i], minimapTileset[i]);
	}

	// Setup the graphics
	gfx_Begin();
	gfx_SetDrawBuffer();
	gfx_SetPalette(tiles_gfx_pal, sizeof_tiles_gfx_pal, 0);
	gfx_SetTransparentColor(TRANSP_COLOR);
	kb_SetMode(MODE_3_CONTINUOUS);
	//gfx_SetTextConfig(1);

	// Dev: 3,2,2,4,0,0,100,0
	player_setup.health = 100;

	// Start the main game
	menuloop();

	gfx_End();
}

