/*
 *--------------------------------------
 * Program Name:CC22 Dungeon
 * Author:Pieman7373
 * License:
 * Description:
 *--------------------------------------
*/

/* Keep these headers */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <tice.h>

#include <math.h>
#include <setjmp.h>
#include <fileioc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <graphx.h>
#include <keypadc.h>
#include "menuandeditfunctions.h"
#include "maingameloop.h"
#include "collisiondetection.h"

#include "gfx/dungeon.h"
#include "gfx/dungeon_gfx.h"
#include "gfx/tiles_gfx.h"

/* Put all your globals here */
	int menuyes;
	int menuoption;
	int textcolor = (0xE6);
	int accentcolor = (0x40);
	int menucolor = (0x00);
	int submenucolor = (0x4A);
	int transcolor = (0xF8);
	int editweaponx = 90;
	int editweapony = 0;
	int editweaponsmallx = 75;
	int editweaponsmally = 150;
	int editscale = 6;
	int editscalep = 12;
	int editpx = 140;
	int editpy = 10;
	
	
	int mapshift;
	
	//player is 5 away
	int mapstartx;
	//player is 4 away
	int mapstarty;
	
	//for tilemap stuff

	#define TILE_WIDTH          32
	#define TILE_HEIGHT         32

	#define TILEMAP_WIDTH       150
	
	#define TILEMAP_HEIGHT      95
	
	#define TILEMAP_DRAW_WIDTH  10
	#define TILEMAP_DRAW_HEIGHT 7

	#define Y_OFFSET            0
	#define X_OFFSET            0
	
	
	unsigned int x_offset;
	unsigned int y_offset;
	
	extern uint8_t *dungeon[44];
	
	extern uint8_t tilemap_map[];
	extern uint8_t tilemap_enemies[];
	gfx_tilemap_t tilemap;
	gfx_tilemap_t enemytilemap;
	
	
	
	//0=naked, 1=leather, 2=chain, 3=steel, 4=dragon
	signed int setnumber = 0;
	
	
	/*
	!!!!!!!!!!!!!!!!!!!!!!
	!!!!!!!!!!!!!!!!!!!!!
	RESET PLAYER STATS
	!!!!!!!!!!!!!!!!!!!!
	!!!!!!!!!!!!!!!!!!!!
	*/
	//{helmet,chestplate,boots,weapon,x,y}
	uint8_t player_setup [6] = {3,2,4,3,0,0};
	
void main(void) {

	kb_key_t key;
	
	if (!dungeon_init()) { abort(); }

    /* Initialize the tilemap structure */
	tilemap.map         = tilemap_map;
    tilemap.tiles       = tileset_tiles;
    tilemap.type_width  = gfx_tile_32_pixel;
    tilemap.type_height = gfx_tile_32_pixel;
    tilemap.tile_height = TILE_HEIGHT;
    tilemap.tile_width  = TILE_WIDTH;
    tilemap.draw_height = TILEMAP_DRAW_HEIGHT;
    tilemap.draw_width  = TILEMAP_DRAW_WIDTH;
    tilemap.height      = TILEMAP_HEIGHT;
    tilemap.width       = TILEMAP_WIDTH;
    tilemap.y_loc       = Y_OFFSET;
    tilemap.x_loc       = X_OFFSET;
	
	    /* Initialize the enemy tilemap structure */
	enemytilemap.map         = tilemap_enemies;
    enemytilemap.tiles       = tileset_tiles;
    enemytilemap.type_width  = gfx_tile_32_pixel;
    enemytilemap.type_height = gfx_tile_32_pixel;
    enemytilemap.tile_height = TILE_HEIGHT;
    enemytilemap.tile_width  = TILE_WIDTH;
    enemytilemap.draw_height = TILEMAP_DRAW_HEIGHT;
    enemytilemap.draw_width  = TILEMAP_DRAW_WIDTH;
    enemytilemap.height      = TILEMAP_HEIGHT;
    enemytilemap.width       = TILEMAP_WIDTH;
    enemytilemap.y_loc       = Y_OFFSET;
    enemytilemap.x_loc       = X_OFFSET;
	
	gfx_SetPalette(dungeon_gfx_pal, sizeof_dungeon_gfx_pal, 0);
	gfx_SetTransparentColor(transcolor);
	gfx_Begin();
	kb_SetMode(3);

	
	do {
		
		/*pick whether to continue or not*/
		
		mainmenu ();

		//menuyes comes from the mainmenu function
		//menuyes chunk begin
		if (menuyes == 1) {
			if (kb_Data[3] & kb_1) {
				loadsave();
				maingameloop();
			}
		}
		if (menuyes == 1) {
			if (kb_Data[4] & kb_2) {
				newgame();
				maingameloop();
			}
		}
		if (menuyes == 1) {
			if (kb_Data[5] & kb_3) {
				options();
			}
		}
		//menuyes chunk end
	
	
	} while (!(kb_Data[6] & kb_Clear));
	
	gfx_End();
}

	
