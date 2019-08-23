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
#include "maingameloop.h"
#include "menuandeditfunctions.h"
#include "minimap.h"
#include "xcollisiondetection.h"

projectile_t projectile[NUM_PROJECTILES];
pots_t pots[NUM_POTS];
money_t money[NUM_POTS];

const uint16_t default_potylist[NUM_POTS] = {194, 194};
// Test map: {97,97, 76, 76,91,91,91,92,92,92,80,80,80,80,80,80,80,71,71,71,71,71,71,71,71,71,71,71,71,71,71,72,72,72,72,72,72,72,72,72,72,72,72,72,72,73,73,73,73,73,73,73,73,73,73,73,73,73,73,74,74,74,74,74,74,74,74,74,74,74,74,74,74,75,75,75,75,75,75,75,75,75,75,75,75,75,75}

const uint16_t default_potxlist[NUM_POTS] = {18, 22};
// Test map {77,78,106,107,14,15,16,14,15,16,52,53,54,55,56,57,58,21,22,23,24,25,26,27,28,29,30,31,32,33,34,21,22,23,24,25,26,27,28,29,30,31,32,33,34,21,22,23,24,25,26,27,28,29,30,31,32,33,34,21,22,23,24,25,26,27,28,29,30,31,32,33,34,21,22,23,24,25,26,27,28,29,30,31,32,33,34}

int mapstartx = ((TILEMAP_WIDTH / 2) - 5);
int mapstarty = (TILEMAP_HEIGHT - 10);






int i;
int goleft;
int goright;
int goup;
int godown;
int spritexoffset = 160;
int spriteyoffset = 128;
int tileoffsetx;
int tileoffsety;
int player;

//1=left,2=up,3=right,4=down
int playerface;
int player_screenpxlx;
int player_screenpxly;
int player_mappxlx;
int player_mappxly;
int hpbar_length;
int hpbar_color;
gfx_sprite_t *weapon;
gfx_sprite_t *weaponrotated;
gfx_sprite_t *flippedequip;
gfx_sprite_t *playerwalk;
gfx_sprite_t *bootanimation;
gfx_sprite_t *animation;
gfx_sprite_t *helmet;
gfx_sprite_t *chestplate;
gfx_sprite_t *boots;

extern uint8_t tilemap_map[];
extern unsigned int tmap_pxl_x_offset;
extern unsigned int tmap_pxl_y_offset;
extern int mapstarty;
int player_maptilex;
int player_maptiley;
int *inputx;
int *inputy;
extern player_setup_t player_setup;
int dmgmultiplier = 1;
int blockchance;
int walkwait = 1500;
int w;
int walkspeed = 100;
int minimapposition = 1;

extern gfx_tilemap_t tilemap;

extern int mmhotkey;
extern int purchased[];
extern int equipped[];

bool showminimap = true;

int walkanimation = 1;
int animationcount_min = 1;
int animationcount_mid = 3;
int animationcount_max = 6;
int p_hit;
int num_p = 0;
int max_p = 25;

//Start of the game
void menuloop(void) {
	while (!kb_IsDown(kb_KeyClear)) {
		// pick whether to continue or not
		uint8_t option = mainmenu();

		if (option == 2) {
			newgame();
			player_setup.helmet = 0;
			player_setup.chestplate = 0;
			player_setup.boots = 0;
			player_setup.weapon = 0;
			player_setup.x = 0;
			player_setup.y = 0;
			player_setup.health = 100;
			player_setup.money = 0;
			maingameloop();
		}

		if (option == 4) {
			playercreate();
			option = 3;
		}

		if (option == 3) {
			newgame();
			equipped[0] = player_setup.helmet;
			equipped[1] = player_setup.chestplate;
			equipped[2] = player_setup.boots;
			equipped[3] = player_setup.weapon;
			maingameloop();
		}
	};

	gfx_End();
	exit(0);
}

void maingameloop(void) {

	tmap_pxl_x_offset = mapstartx * 32;
	tmap_pxl_y_offset = mapstarty * 32;
	do {

		(goleft = 0);
		(goright = 0);
		(goup = 0);
		(godown = 0);

		drawmap();
		drawcharacter();
		drawplayerattack();
		updateenemies();
		updatepots();
		updatemoney();
		updateprojectiles();
		minimap();
		checkplayerstatus();
		drawbottombar();

		gfx_SwapDraw();

		mapshifter();
		if (kb_IsDown(kb_KeyYequ)) {
			drawpouch();
		}
		if (kb_IsDown(kb_KeyZoom)) {
			drawstatsmenu();
		}
		if (kb_IsDown(kb_KeyTrace)) {
			drawstore();
		}
		if (kb_IsDown(kb_KeyGraph)) {
			drawoptions();
		}
		if ((kb_IsDown(kb_KeyAlpha) && mmhotkey == 1) || (kb_IsDown(kb_KeyMode) && mmhotkey == 2) ||
			(kb_IsDown(kb_KeyDel) && mmhotkey == 3)) {
			showminimap = !showminimap;
		}

		walkanimation++;
		if (walkanimation > animationcount_max) {
			walkanimation = animationcount_min;
		}

	} while (!(kb_Data[6] & kb_Clear));
	menuloop();
}

void drawmap(void) {
	player_screenpxlx = ((tmap_pxl_x_offset / tmap_pxl_x_offset) + spritexoffset);
	player_screenpxly = ((tmap_pxl_y_offset / tmap_pxl_y_offset) + spriteyoffset);
	player_mappxlx = (tmap_pxl_x_offset + (spritexoffset));
	player_mappxly = (tmap_pxl_y_offset + (spriteyoffset));
	gfx_Tilemap(&tilemap, tmap_pxl_x_offset, tmap_pxl_y_offset);
	gfx_SetColor(0x00);
	gfx_FillRectangle(0, 224, 320, 16);

	gfx_SetTextBGColor(0x00);
	gfx_SetTextXY(10, 150);
	gfx_PrintInt(num_p, 3);

//coordinate debug numbers
	/*
	gfx_SetTextBGColor(0x00);
	gfx_SetTextXY(10,10);
	gfx_PrintString("tmap_px_offset---");
	gfx_PrintInt(tmap_pxl_x_offset,4);
	gfx_PrintString(" , ");
	gfx_PrintInt(tmap_pxl_y_offset,4);
	*/

/*print debug numbers for collision and stuff
	gfx_PrintStringXY("tile", 8, 224);
	gfx_PrintUInt((gfx_GetTile(&tilemap,player_mappxlx,player_mappxly)),2);
	gfx_PrintString("  tile L");
	gfx_PrintUInt((gfx_GetTile(&tilemap,player_mappxlx-32,player_mappxly)),2);
	gfx_PrintString("  tile U");
	gfx_PrintUInt((gfx_GetTile(&tilemap,player_mappxlx,player_mappxly-32)),2);
	gfx_PrintString("  tile R");
	gfx_PrintUInt((gfx_GetTile(&tilemap,player_mappxlx+32,player_mappxly)),2);
	gfx_PrintString("  tile D");
	gfx_PrintUInt((gfx_GetTile(&tilemap,player_mappxlx,player_mappxly+32)),2);
*/
}

void mapshifter(void) {
	player_maptilex = (player_mappxlx / 32);
	player_maptiley = (player_mappxly / 32);
	inputx = &player_maptilex;
	inputy = &player_maptiley;
	player = 1;

	if (kb_Data[7] & kb_Left) {
		(playerface = 1);
		collisionleft();
		if (goleft == 1) {
			(tmap_pxl_x_offset = (tmap_pxl_x_offset - MAP_SHIFT));
		}
	}
	if (kb_Data[7] & kb_Right) {
		(playerface = 3);
		collisionright();
		if (goright == 1) {
			(tmap_pxl_x_offset = (tmap_pxl_x_offset + MAP_SHIFT));
		}
	}
	if (kb_Data[7] & kb_Up) {
		(playerface = 2);
		collisionup();
		if (goup == 1) {
			(tmap_pxl_y_offset = (tmap_pxl_y_offset - MAP_SHIFT));
		}
	}
	if (kb_Data[7] & kb_Down) {
		(playerface = 4);
		collisiondown();
		if (godown == 1) {
			(tmap_pxl_y_offset = (tmap_pxl_y_offset + MAP_SHIFT));
		}
	}
}

void drawcharacter(void) {
	gfx_UninitedSprite(playerwalk, 32, 32);
	if (playerface == 1) {
		animation = player_naked_left;
	}
	if (playerface == 2) {
		if (walkanimation <= animationcount_mid) {
			animation = player_naked_up;
		}
		if (walkanimation > animationcount_mid) {
			gfx_FlipSpriteY(player_naked_up, playerwalk);
			animation = playerwalk;
		}
	}
	if (playerface == 3) {
		animation = player_naked_right;
	}
	if (playerface == 4) {
		if (walkanimation <= animationcount_mid) {
			animation = player_naked_down;
		}
		if (walkanimation > animationcount_mid) {
			gfx_FlipSpriteY(player_naked_down, playerwalk);
			animation = playerwalk;
		}
	}
	gfx_TransparentSprite(animation, player_screenpxlx, player_screenpxly);
	drawhelmet();
	drawchestplate();
	drawboot();

	/*
	gfx_SetTextXY(10,30);
	gfx_PrintString("player_screenpxl-");
	gfx_PrintInt(player_screenpxlx,4);
	gfx_PrintString(" , ");
	gfx_PrintInt(player_screenpxly,4);
	gfx_SetTextXY(10,50);
	gfx_PrintString("player_mappxl-----");
	gfx_PrintInt(player_mappxlx,4);
	gfx_PrintString(" , ");
	gfx_PrintInt(player_mappxly,4);
	gfx_SetTextXY(10,70);
	gfx_PrintString("player_maptile----");
	gfx_PrintInt(player_maptilex,4);
	gfx_PrintString(" , ");
	gfx_PrintInt(player_maptiley,4);
	*/

}

void drawhelmet(void) {
	gfx_UninitedSprite(flippedequip, 32, 32);
	if (player_setup.helmet == 1) {
		if (playerface == 1) { helmet = leather_helmet_left; }
		else if (playerface == 2) { helmet = leather_helmet_up; }
		else if (playerface == 3) { helmet = (gfx_FlipSpriteY(leather_helmet_left, flippedequip)); }
		else if (playerface == 4) { helmet = leather_helmet_down; }
	} else if (player_setup.helmet == 2) {
		if (playerface == 1) { helmet = chain_helmet_left; }
		else if (playerface == 2) { helmet = chain_helmet_up; }
		else if (playerface == 3) { helmet = (gfx_FlipSpriteY(chain_helmet_left, flippedequip)); }
		else if (playerface == 4) { helmet = chain_helmet_down; }
	} else if (player_setup.helmet == 3) {
		if (playerface == 1) { helmet = steel_helmet_left; }
		else if (playerface == 2) { helmet = steel_helmet_up; }
		else if (playerface == 3) { helmet = (gfx_FlipSpriteY(steel_helmet_left, flippedequip)); }
		else if (playerface == 4) { helmet = steel_helmet_down; }
	} else if (player_setup.helmet == 4) {
		if (playerface == 1) { helmet = dragon_helmet_left; }
		else if (playerface == 2) { helmet = dragon_helmet_up; }
		else if (playerface == 3) { helmet = (gfx_FlipSpriteY(dragon_helmet_left, flippedequip)); }
		else if (playerface == 4) { helmet = dragon_helmet_down; }
	}
	if (player_setup.helmet != 0) {
		gfx_TransparentSprite(helmet, player_screenpxlx, player_screenpxly);
	}
}

void drawchestplate(void) {
	gfx_UninitedSprite(flippedequip, 32, 32);
	if (player_setup.chestplate == 1) {
		if (playerface == 1) { chestplate = leather_chestplate_left; }
		else if (playerface == 2) { chestplate = leather_chestplate_up; }
		else if (playerface == 3) { chestplate = (gfx_FlipSpriteY(leather_chestplate_left, flippedequip)); }
		else if (playerface == 4) { chestplate = leather_chestplate_down; }
	} else if (player_setup.chestplate == 2) {
		if (playerface == 1) { chestplate = chain_chestplate_left; }
		else if (playerface == 2) { chestplate = chain_chestplate_up; }
		else if (playerface == 3) { chestplate = (gfx_FlipSpriteY(chain_chestplate_left, flippedequip)); }
		else if (playerface == 4) { chestplate = chain_chestplate_down; }
	} else if (player_setup.chestplate == 3) {
		if (playerface == 1) { chestplate = steel_chestplate_left; }
		else if (playerface == 2) { chestplate = steel_chestplate_up; }
		else if (playerface == 3) { chestplate = (gfx_FlipSpriteY(steel_chestplate_left, flippedequip)); }
		else if (playerface == 4) { chestplate = steel_chestplate_down; }
	} else if (player_setup.chestplate == 4) {
		if (playerface == 1) { chestplate = dragon_chestplate_left; }
		else if (playerface == 2) { chestplate = dragon_chestplate_up; }
		else if (playerface == 3) { chestplate = (gfx_FlipSpriteY(dragon_chestplate_left, flippedequip)); }
		else if (playerface == 4) { chestplate = dragon_chestplate_down; }
	}
	if (player_setup.chestplate != 0) {
		gfx_TransparentSprite(chestplate, player_screenpxlx, player_screenpxly);
	}
}

void drawboot(void) {
	gfx_UninitedSprite(flippedequip, 32, 32);
	gfx_UninitedSprite(bootanimation, 32, 32);
	//beck: if player_setup.boots is zero, "boots" will never be set
	if (player_setup.boots) {
		if (player_setup.boots == 1) {
			if (playerface == 1) { boots = leather_boots_left; }
			else if (playerface == 2) { boots = leather_boots_up; }
			else if (playerface == 3) { boots = (gfx_FlipSpriteY(leather_boots_left, flippedequip)); }
			else if (playerface == 4) { boots = leather_boots_down; }
		} else if (player_setup.boots == 2) {
			if (playerface == 1) { boots = chain_boots_left; }
			else if (playerface == 2) { boots = chain_boots_up; }
			else if (playerface == 3) { boots = (gfx_FlipSpriteY(chain_boots_left, flippedequip)); }
			else if (playerface == 4) { boots = chain_boots_down; }
		} else if (player_setup.boots == 3) {
			if (playerface == 1) { boots = steel_boots_left; }
			else if (playerface == 2) { boots = steel_boots_up; }
			else if (playerface == 3) { boots = (gfx_FlipSpriteY(steel_boots_left, flippedequip)); }
			else if (playerface == 4) { boots = steel_boots_down; }
		} else if (player_setup.boots == 4) {
			if (playerface == 1) { boots = dragon_boots_left; }
			else if (playerface == 2) { boots = dragon_boots_up; }
			else if (playerface == 3) { boots = (gfx_FlipSpriteY(dragon_boots_left, flippedequip)); }
			else if (playerface == 4) { boots = dragon_boots_down; }
		}

		//Only draw the sprite "boots" if it was set.
		//The only case it gets set is when player_setup.boots is not zero
		if (walkanimation <= animationcount_mid) {
			gfx_TransparentSprite(boots, player_screenpxlx, player_screenpxly + 27);
		} else if (walkanimation > animationcount_mid) {
			if ((playerface == 2) || (playerface == 4)) {
				gfx_FlipSpriteY(boots, bootanimation);
			}
			if ((playerface == 1) || (playerface == 3)) {
				bootanimation = boots;
			}
			if (player_setup.boots != 0) {
				gfx_TransparentSprite(bootanimation, player_screenpxlx, player_screenpxly + 27);
			}
		}
	}
}

void drawplayerattack(void) {
	gfx_UninitedSprite(weaponrotated, 32, 32);
	int p;
	p = 1;
	player_setup.weapon = 5;

	if (kb_Data[1] & kb_2nd) {
		if (player_setup.weapon == 0) { weapon = fist; }
		if (player_setup.weapon == 1) { weapon = club; }
		if (player_setup.weapon == 2) { weapon = iron_sword; }
		if (player_setup.weapon == 3) { weapon = steel_sword; }
		if (player_setup.weapon == 4) { weapon = dragon_sword; }
		if (player_setup.weapon == 5) { weapon = bow_basic; }
		if (player_setup.weapon == 6) { weapon = bow_recurve; }
		if (player_setup.weapon == 7) { weapon = bow_compound; }
		if (player_setup.weapon == 8) { weapon = bow_dragon; }


		if (playerface == 1) {
			if (player_setup.weapon < 5) {
				gfx_TransparentSprite(gfx_RotateSpriteCC(weapon, weaponrotated), player_screenpxlx - 32,
									  player_screenpxly);
			}
		}
		if (playerface == 2) {
			if (player_setup.weapon < 5) {
				gfx_TransparentSprite(weapon, player_screenpxlx, player_screenpxly - 32);
			}

		}
		if (playerface == 3) {
			if (player_setup.weapon < 5) {
				gfx_TransparentSprite(gfx_RotateSpriteC(weapon, weaponrotated), player_screenpxlx + 32,
									  player_screenpxly);
			}

		}
		if (playerface == 4) {
			if (player_setup.weapon < 5) {
				gfx_TransparentSprite(gfx_RotateSpriteHalf(weapon, weaponrotated), player_screenpxlx,
									  player_screenpxly + 32);
			}
		}

		if (player_setup.weapon >= 5) {
			if (num_p < max_p) {
				num_p++;
			}
			if (player_setup.weapon <= 8) {
				projectile[num_p].p_type = 1;
			}
			projectile[num_p].p_x = player_mappxlx;
			projectile[num_p].p_y = player_mappxly;
			projectile[num_p].p_alive = 1;
			projectile[num_p].p_direction = playerface;
			projectile[num_p].p_speed = 5;


			if (playerface == 1) {
				gfx_TransparentSprite(gfx_RotateSpriteCC(weapon, weaponrotated), player_screenpxlx - 32,
									  player_screenpxly);
				if (projectile[num_p].p_type == 1) {
					projectile[num_p].p_y = (projectile[num_p].p_y + 13);
				}
				projectile[num_p].p_vx = -1;
				projectile[num_p].p_vy = 0;
			}
			if (playerface == 2) {
				gfx_TransparentSprite(weapon, player_screenpxlx, player_screenpxly - 7);
				if (projectile[num_p].p_type == 1) {
					projectile[num_p].p_x = (projectile[num_p].p_x + 13);
				}
				projectile[num_p].p_vx = 0;
				projectile[num_p].p_vy = -1;
			}
			if (playerface == 3) {
				gfx_TransparentSprite(gfx_RotateSpriteC(weapon, weaponrotated), player_screenpxlx + 32,
									  player_screenpxly);
				if (projectile[num_p].p_type == 1) {
					projectile[num_p].p_y = (projectile[num_p].p_y + 13);
				}
				projectile[num_p].p_vx = 1;
				projectile[num_p].p_vy = 0;
			}
			if (playerface == 4) {
				gfx_TransparentSprite(gfx_RotateSpriteHalf(weapon, weaponrotated), player_screenpxlx,
									  player_screenpxly + 32);
				if (projectile[num_p].p_type == 1) {
					projectile[num_p].p_x = (projectile[num_p].p_x + 13);
				}
				projectile[num_p].p_vx = 0;
				projectile[num_p].p_vy = 1;
			}

			/*
			gfx_SetTextBGColor(0x00);
			gfx_SetTextXY(10,90);
			gfx_PrintString("arrow coords-----");
			gfx_PrintInt(projectile[num_p].p_x,4);
			gfx_PrintString(" , ");
			gfx_PrintInt(projectile[num_p].p_y,4);
			*/

		}


		playerattackhitcheck();
	}
}

void checkplayerstatus(void) {
	extern int playerdamage;
	int i;
	//checks if you are standing on a spike
	if ((gfx_GetTile(&tilemap, player_mappxlx, player_mappxly)) == 9) {
		(player_setup.health = player_setup.health - 5);
		gfx_FillScreen(0xE0);
	}

	//checks player health and adjusts healthbar
	if (player_setup.health > 100) { player_setup.health = 100; }
	if ((100 >= player_setup.health) & (player_setup.health > 90)) {
		hpbar_length = 60;
		hpbar_color = GREEN_COLOR;
	} else if ((90 >= player_setup.health) & (player_setup.health > 80)) {
		hpbar_length = 54;
		hpbar_color = GREEN_COLOR;
	} else if ((80 >= player_setup.health) & (player_setup.health > 70)) {
		hpbar_length = 48;
		hpbar_color = GREEN_COLOR;
	} else if ((70 >= player_setup.health) & (player_setup.health > 60)) {
		hpbar_length = 42;
		hpbar_color = GREEN_COLOR;
	} else if ((60 >= player_setup.health) & (player_setup.health > 50)) {
		hpbar_length = 36;
		hpbar_color = YELLOW_COLOR;
	} else if ((50 >= player_setup.health) & (player_setup.health > 40)) {
		hpbar_length = 30;
		hpbar_color = YELLOW_COLOR;
	} else if ((40 >= player_setup.health) & (player_setup.health > 30)) {
		hpbar_length = 24;
		hpbar_color = YELLOW_COLOR;
	} else if ((30 >= player_setup.health) & (player_setup.health > 20)) {
		hpbar_length = 18;
		hpbar_color = RED_COLOR;
	} else if ((20 >= player_setup.health) & (player_setup.health > 10)) {
		hpbar_length = 12;
		hpbar_color = RED_COLOR;
	}
	if (player_setup.health <= 0) { youdied(); }

	//determines player_setup
	player_setup.helmet = equipped[0];
	player_setup.chestplate = equipped[1];
	player_setup.boots = equipped[2];
	player_setup.weapon = equipped[3];
	//determined by weapon
	playerdamage = player_setup.weapon + 1;
	//helmet and chestplate added together
	blockchance = ((player_setup.helmet + player_setup.chestplate) * 10);
	//set by boots
	//walkspeed = ((player_setup.boots+1)*20);



}

void drawbottombar(void) {
	gfx_SetColor(0x00);
	gfx_FillRectangle(0, 224, 320, 16);
	gfx_SetColor(hpbar_color);
	gfx_FillRectangle(80, 227, hpbar_length, 10);
	gfx_TransparentSprite(health_empty, 80, 224);
	if ((10 >= player_setup.health) & (player_setup.health > 0)) { gfx_TransparentSprite(health10, 80, 224); }
	gfx_SetTextScale(1, 1);
	gfx_SetTextFGColor(TEXT_COLOR);
	gfx_PrintStringXY("[POUCH]  HP:", 4, 228);
	gfx_PrintStringXY("[STATS]", 150, 228);
	gfx_PrintStringXY("[STORE]", 208, 228);
	gfx_PrintStringXY("[OPTNS]", 266, 228);
}

void youdied(void) {
	gfx_FillScreen(MENU_COLOR);
	gfx_SetColor(ACCENT_COLOR);
	gfx_Rectangle(0, 0, 320, 240);
	gfx_Rectangle(2, 2, 316, 236);
	gfx_ScaledTransparentSprite_NoClip(tombstone, 70, 30, 5, 5);
	gfx_SetTextFGColor(0xE8);
	gfx_SetTextScale(2, 2);
	gfx_PrintStringXY("You died!", 102, 192);
	gfx_SetTextScale(1, 1);
	gfx_PrintStringXY("[f1] Main Menu", 100, 210);
	gfx_PrintStringXY("[f2] Quit", 100, 218);
	gfx_SwapDraw();
	do {
		if (kb_Data[1] & kb_Yequ) { menuloop(); }
	} while (!(kb_Data[1] & kb_Window));
	gfx_End();
	exit(0);
}

void resetpots(void) {
	int n = 0;
	for (i = 0; i < NUM_POTS; i++) {
		n++;
		if (n > 5) { n = 0; }
		if (3 <= n) {
			pots[i].pottype = 0;
		} else {
			pots[i].pottype = 1;
		}
		pots[i].potdead = 0;
		pots[i].p_x = default_potxlist[i] * 32;
		pots[i].p_y = default_potylist[i] * 32;
	}
}

void updatepots(void) {
	for (i = 0; i < NUM_POTS; i++) {
		if ((pots[i].potdead) == 0) {
			renderpots(&pots[i]);
		}
	}
}

void renderpots(pots_t *pots) {
	gfx_TransparentSprite(pot, (pots->p_x - tmap_pxl_x_offset), (pots->p_y - tmap_pxl_y_offset));

/*only for testing
gfx_SetTextFGColor(0xA8);
gfx_SetTextXY(pots->p_x - tmap_pxl_x_offset,pots->p_y - tmap_pxl_y_offset);
gfx_PrintUInt(pots->pottype,1);
*/

/*for debug
gfx_PrintString("---");
gfx_PrintUInt((pots->p_x)/32,3);
gfx_PrintString("-");
gfx_PrintUInt((pots->p_y)/32,3);
*/

}

void updatemoney(void) {
	for (i = 0; i < NUM_POTS; i++) {
		if ((money[i].moneydead) == 0) {
			rendermoney(&money[i]);
		}
	}
}

void rendermoney(money_t *money) {
	gfx_sprite_t *moneySprite;

	moneySprite = money1;
	if (money->moneyvalue == 5) { moneySprite = money5; }
	if (money->moneyvalue == 10) { moneySprite = money10; }
	if (money->moneyvalue == 20) { moneySprite = money20; }
	if (money->moneyvalue == 100) { moneySprite = money100; }
	gfx_TransparentSprite(moneySprite, money->m_x - tmap_pxl_x_offset, money->m_y - tmap_pxl_y_offset);

}

void updateprojectiles(void) {
	for (i = 0; i < num_p; i++) {
		if ((projectile[i].p_alive) == 1) {
			projectile[i].p_x = (projectile[i].p_x + (projectile[i].p_vx * projectile[i].p_speed));
			projectile[i].p_y = (projectile[i].p_y + (projectile[i].p_vy * projectile[i].p_speed));
			p_hit = 0;

			//projectilemapcollision();
			//projectileentitycollision();

			//if it hit a wall
			if (p_hit == 1) {
				gfx_TransparentSprite(arrow_hitwall, projectile[i].p_x, projectile[i].p_y);
				num_p--;
				projectile[i].p_alive = 0;
			}
			if (p_hit == 0) {
				renderprojectiles(&projectile[i]);
			}
		}
	}
}

void renderprojectiles(projectile_t *projectile) {
	gfx_UninitedSprite(p_unrotated, 32, 5);
	gfx_UninitedSprite(p_rotated, 32, 5);

	if (projectile->p_type == 1) { p_unrotated = arrow; }

	if (projectile->p_direction == 1) {
		gfx_RotateSpriteHalf(p_unrotated, p_rotated);
	}
	if (projectile->p_direction == 2) {
		gfx_RotateSpriteCC(p_unrotated, p_rotated);
	}
	if (projectile->p_direction == 3) {
		p_rotated = p_unrotated;
	}
	if (projectile->p_direction == 4) {
		gfx_RotateSpriteC(p_unrotated, p_rotated);
	}
	gfx_TransparentSprite(p_rotated, projectile->p_x - tmap_pxl_x_offset, projectile->p_y - tmap_pxl_y_offset);
}


