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
#include "enemymovement.h"
#include "gfx/dungeon.h"
#include "gfx/dungeon_gfx.h"
#include "maingameloop.h"
#include "menuandeditfunctions.h"

enemy_t enemy[NUM_ENEMIES];
extern money_t money[NUM_POTS];
relic_t relic[NUM_RELICS];
boss_t boss[NUM_BOSS];

const uint16_t defaultboss_type[NUM_BOSS] = {1, 2, 3};
const uint16_t defaultboss_xlist[NUM_BOSS] = {5, 6, 7};
const uint16_t defaultboss_ylist[NUM_BOSS] = {5, 6, 7};

const uint16_t defaultenemy_ylist[NUM_ENEMIES] = {187, 187, 187, 187, 187, 187, 187};
//from test map {90,91,92,90,91,90,91,74,75,74,75,93,81,81,81,81,81,81,81,82,82,82,82,82,82,82,65,65,65,65,65,65,65}

const uint16_t defaultenemy_xlist[NUM_ENEMIES] = {12, 13, 14, 15, 16, 17, 18};
//from test map {76,76,76,77,77,78,78,106,107,107,106,15,52,53,54,55,56,57,58,52,53,54,55,56,57,58,25,26,27,28,29,30,31}

const uint16_t defaultenemy_movelist[NUM_ENEMIES] = {0, 0, 0, 0, 0, 0, 0};
//from test map {1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1}

const uint16_t defaultenemy_typelist[NUM_ENEMIES] = {0, 1, 2, 3, 4, 5, 6};
//from test map {0,1,2,3,4,5,6,2,2,2,2,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,3,4,5,6}






extern int i;
extern int menuoption;
extern int playerface;
int editweaponx = 90;
int editweapony = 0;
int editweaponsmallx = 66;
int editweaponsmally = 152;
int editscale = 6;
int editscalep = 12;
int editpx = 140;
int editpy = 10;
extern unsigned int tmap_pxl_x_offset;
extern unsigned int tmap_pxl_y_offset;
int mmhotkey = 0;
bool damagenumbers;

/*
uint8_t enemytype;
int deadset;
uint24_t enemyx;
uint24_t enemyy;
int enemyhealth;
*/

bool drawhealth = true;
TYPE_SET_NUMBER setnumber;
extern player_setup_t player_setup;

extern int mapstartx;
extern int mapstarty;
extern int tileoffsetx;
extern int tileoffsety;
extern kb_key_t key;
extern int showminimap;

gfx_sprite_t *enemySprite;

int ii;
int purchased[22];
int inventory[22];
int equipped[5];

extern int num_p;

extern int minimapposition;

void keywait(void) { while (kb_AnyKey()); }

uint8_t mainmenu(void) {
	gfx_FillScreen(MENU_COLOR);
	gfx_SetColor(ACCENT_COLOR);
	gfx_Rectangle(0, 0, 320, 240);
	gfx_Rectangle(2, 2, 316, 236);
	gfx_ScaledTransparentSprite_NoClip(mainmenulogo, 33, 20, 2, 2);
	gfx_SetTextScale(2, 2);
	gfx_SetTextFGColor(TEXT_COLOR);
	gfx_PrintStringXY("1. Continue", 68, 120);
	gfx_PrintStringXY("2. New Game", 68, 145);
	gfx_PrintStringXY("3. Free Play", 68, 170);
	gfx_PrintStringXY("4. Edit Player", 68, 195);
	gfx_SetTextScale(1, 1);
	gfx_PrintStringXY("By: Pieman7373   2019", 8, 228);
	gfx_SwapDraw();

	while (!(kb_Data[6] & kb_Clear)) {
		if (kb_IsDown(kb_Key1)) {
			return 1;
		}
		if (kb_IsDown(kb_Key2)) {
			return 2;
		}
		if (kb_IsDown(kb_Key3)) {
			return 3;
		}
		if (kb_IsDown(kb_Key4)) {
			return 4;
		}
	}

	return 0;

}

void drawsavemenu(void) {
	gfx_SetColor(SUBMENU_COLOR);
	gfx_FillRectangle(0, 154, 108, 66);
	gfx_SetColor(ACCENT_COLOR);
	gfx_Rectangle(0, 154, 108, 66);
	gfx_Rectangle(2, 156, 104, 62);
	submenubottombar();
	gfx_PrintStringXY("1. Save", 5, 160);
	gfx_SwapDraw();
	do {
		if (kb_Data[3] & kb_1) { savegame(); }
	} while (!(kb_Data[1] & kb_Window));
}

void drawstore(void) {
	int cursorposition = 1;
	int storepage = 1;
	int storepane = 1;
	int windowpress = 0;
	do {
		gfx_SetColor(SUBMENU_COLOR);
		gfx_FillRectangle(140, 125, 150, 95);
		gfx_SetColor(ACCENT_COLOR);
		gfx_Rectangle(140, 125, 150, 95);
		gfx_Rectangle(142, 127, 146, 91);
		submenubottombar();
		windowpress = 0;
		gfx_SetTextFGColor(TEXT_COLOR);
		gfx_PrintStringXY("Money:  ", 150, 207);
		gfx_PrintUInt(player_setup.money, sizeof(player_setup.money));
		gfx_PrintStringXY("<", 160, 228);
		gfx_PrintStringXY(">", 288, 228);
		if (storepane == 1) {
			gfx_PrintStringXY("ARMOR", 208, 228);
			if (storepage == 1) {
				if (cursorposition == 1) { gfx_SetTextFGColor(SELECTED_COLOR); }
				else {
					gfx_SetTextFGColor(TEXT_COLOR);
					if (PRICE_LEATHER_HELMET > player_setup.money) {
						gfx_SetTextFGColor(RED_COLOR);
					}
					if (purchased[0] == 1) {
						gfx_SetTextFGColor(GREEN_COLOR);
					}
				}
				gfx_PrintStringXY("Leather Helmet    ", 150, 132);
				gfx_PrintInt(PRICE_LEATHER_HELMET, 2);

				if (cursorposition == 2) { gfx_SetTextFGColor(SELECTED_COLOR); }
				else {
					gfx_SetTextFGColor(TEXT_COLOR);
					if (PRICE_LEATHER_TUNIC > player_setup.money) {
						gfx_SetTextFGColor(RED_COLOR);
					}
					if (purchased[1] == 1) {
						gfx_SetTextFGColor(GREEN_COLOR);
					}
				}
				gfx_PrintStringXY("Leather Tunic    ", 150, 147);
				gfx_PrintInt(PRICE_LEATHER_TUNIC, 2);

				if (cursorposition == 3) { gfx_SetTextFGColor(SELECTED_COLOR); }
				else {
					gfx_SetTextFGColor(TEXT_COLOR);
					if (PRICE_LEATHER_BOOTS > player_setup.money) {
						gfx_SetTextFGColor(RED_COLOR);
					}
					if (purchased[2] == 1) {
						gfx_SetTextFGColor(GREEN_COLOR);
					}
				}
				gfx_PrintStringXY("Leather Boots    ", 150, 162);
				gfx_PrintInt(PRICE_LEATHER_BOOTS, 2);

				if (cursorposition == 4) { gfx_SetTextFGColor(SELECTED_COLOR); }
				else {
					gfx_SetTextFGColor(TEXT_COLOR);
					if (PRICE_IRON_HELMET > player_setup.money) {
						gfx_SetTextFGColor(RED_COLOR);
					}
					if (purchased[3] == 1) {
						gfx_SetTextFGColor(GREEN_COLOR);
					}
				}
				gfx_PrintStringXY("Iron Helmet    ", 150, 177);
				gfx_PrintInt(PRICE_IRON_HELMET, 2);

				if (cursorposition == 5) { gfx_SetTextFGColor(SELECTED_COLOR); }
				else {
					gfx_SetTextFGColor(TEXT_COLOR);
					if (PRICE_CHAIN_SHIRT > player_setup.money) {
						gfx_SetTextFGColor(RED_COLOR);
					}
					if (purchased[4] == 1) {
						gfx_SetTextFGColor(GREEN_COLOR);
					}
				}
				gfx_PrintStringXY("Chain Shirt    ", 150, 192);
				gfx_PrintInt(PRICE_CHAIN_SHIRT, 2);
			}
			if (storepage == 2) {
				if (cursorposition == 1) { gfx_SetTextFGColor(SELECTED_COLOR); }
				else {
					gfx_SetTextFGColor(TEXT_COLOR);
					if (PRICE_CHAIN_BOOTS > player_setup.money) {
						gfx_SetTextFGColor(RED_COLOR);
					}
					if (purchased[5] == 1) {
						gfx_SetTextFGColor(GREEN_COLOR);
					}
				}
				gfx_PrintStringXY("Chain Boots    ", 150, 132);
				gfx_PrintInt(PRICE_CHAIN_BOOTS, 2);

				if (cursorposition == 2) { gfx_SetTextFGColor(SELECTED_COLOR); }
				else {
					gfx_SetTextFGColor(TEXT_COLOR);
					if (PRICE_STEEL_HELMET > player_setup.money) {
						gfx_SetTextFGColor(RED_COLOR);
					}
					if (purchased[6] == 1) {
						gfx_SetTextFGColor(GREEN_COLOR);
					}
				}
				gfx_PrintStringXY("Steel Helmet    ", 150, 147);
				gfx_PrintInt(PRICE_STEEL_HELMET, 3);

				if (cursorposition == 3) { gfx_SetTextFGColor(SELECTED_COLOR); }
				else {
					gfx_SetTextFGColor(TEXT_COLOR);
					if (PRICE_STEEL_CHESTPLATE > player_setup.money) {
						gfx_SetTextFGColor(RED_COLOR);
					}
					if (purchased[7] == 1) {
						gfx_SetTextFGColor(GREEN_COLOR);
					}
				}
				gfx_PrintStringXY("Steel Chest   ", 150, 162);
				gfx_PrintInt(PRICE_STEEL_CHESTPLATE, 3);

				if (cursorposition == 4) { gfx_SetTextFGColor(SELECTED_COLOR); }
				else {
					gfx_SetTextFGColor(TEXT_COLOR);
					if (PRICE_STEEL_BOOTS > player_setup.money) {
						gfx_SetTextFGColor(RED_COLOR);
					}
					if (purchased[8] == 1) {
						gfx_SetTextFGColor(GREEN_COLOR);
					}
				}
				gfx_PrintStringXY("Steel Boots    ", 150, 177);
				gfx_PrintInt(PRICE_STEEL_BOOTS, 3);

				if (cursorposition == 5) { gfx_SetTextFGColor(SELECTED_COLOR); }
				else {
					gfx_SetTextFGColor(TEXT_COLOR);
					if (PRICE_DRAGON_HELMET > player_setup.money) {
						gfx_SetTextFGColor(RED_COLOR);
					}
					if (purchased[9] == 1) {
						gfx_SetTextFGColor(GREEN_COLOR);
					}
				}
				gfx_PrintStringXY("Dragon Helmet    ", 150, 192);
			}
			gfx_PrintInt(PRICE_DRAGON_HELMET, 3);
			if (storepage == 3) {
				if (cursorposition == 1) { gfx_SetTextFGColor(SELECTED_COLOR); }
				else {
					gfx_SetTextFGColor(TEXT_COLOR);
					if (PRICE_DRAGON_CHESTPLATE > player_setup.money) {
						gfx_SetTextFGColor(RED_COLOR);
					}
					if (purchased[10] == 1) {
						gfx_SetTextFGColor(GREEN_COLOR);
					}
				}
				gfx_PrintStringXY("Dragon Chest    ", 150, 132);
				gfx_PrintInt(PRICE_DRAGON_CHESTPLATE, 3);

				if (cursorposition == 2) { gfx_SetTextFGColor(SELECTED_COLOR); }
				else {
					gfx_SetTextFGColor(TEXT_COLOR);
					if (PRICE_DRAGON_BOOTS > player_setup.money) {
						gfx_SetTextFGColor(RED_COLOR);
					}
					if (purchased[11] == 1) {
						gfx_SetTextFGColor(GREEN_COLOR);
					}
				}
				gfx_PrintStringXY("Dragon Boots    ", 150, 147);
				gfx_PrintInt(PRICE_DRAGON_BOOTS, 3);

				if (cursorposition == 3) { gfx_SetTextFGColor(SELECTED_COLOR); }
				else { gfx_SetTextFGColor(GRAY_COLOR); }
				gfx_PrintStringXY("--------", 150, 162);

				if (cursorposition == 4) { gfx_SetTextFGColor(SELECTED_COLOR); }
				else { gfx_SetTextFGColor(GRAY_COLOR); }
				gfx_PrintStringXY("--------", 150, 177);

				if (cursorposition == 5) { gfx_SetTextFGColor(SELECTED_COLOR); }
				else { gfx_SetTextFGColor(GRAY_COLOR); }
				gfx_PrintStringXY("--------", 150, 192);
			}
		}
		if (storepane == 2) {
			gfx_PrintStringXY("WEAPONS", 203, 228);
			if (storepage == 1) {
				if (cursorposition == 1) { gfx_SetTextFGColor(SELECTED_COLOR); }
				else {
					gfx_SetTextFGColor(TEXT_COLOR);
				}
				if (PRICE_CLUB > player_setup.money) {
					gfx_SetTextFGColor(RED_COLOR);
				}
				if (purchased[12] == 1) {
					gfx_SetTextFGColor(GREEN_COLOR);
				}
				gfx_PrintStringXY("Club    ", 150, 132);
				gfx_PrintInt(PRICE_CLUB, 2);

				if (cursorposition == 2) { gfx_SetTextFGColor(SELECTED_COLOR); }
				else {
					gfx_SetTextFGColor(TEXT_COLOR);
				}
				if (PRICE_IRON_SWORD > player_setup.money) {
					gfx_SetTextFGColor(RED_COLOR);
				}
				if (purchased[13] == 1) {
					gfx_SetTextFGColor(GREEN_COLOR);
				}
				gfx_PrintStringXY("Iron Sword    ", 150, 147);
				gfx_PrintInt(PRICE_IRON_SWORD, 2);

				if (cursorposition == 3) { gfx_SetTextFGColor(SELECTED_COLOR); }
				else {
					gfx_SetTextFGColor(TEXT_COLOR);
				}
				if (PRICE_STEEL_SWORD > player_setup.money) {
					gfx_SetTextFGColor(RED_COLOR);
				}
				if (purchased[14] == 1) {
					gfx_SetTextFGColor(GREEN_COLOR);
				}
				gfx_PrintStringXY("Steel Sword    ", 150, 162);
				gfx_PrintInt(PRICE_STEEL_SWORD, 3);

				if (cursorposition == 4) { gfx_SetTextFGColor(SELECTED_COLOR); }
				else {
					gfx_SetTextFGColor(TEXT_COLOR);
				}
				if (PRICE_DRAGON_SWORD > player_setup.money) {
					gfx_SetTextFGColor(RED_COLOR);
				}
				if (purchased[15] == 1) {
					gfx_SetTextFGColor(GREEN_COLOR);
				}
				gfx_PrintStringXY("Dragon Sword    ", 150, 177);
				gfx_PrintInt(PRICE_DRAGON_SWORD, 4);

				if (cursorposition == 5) { gfx_SetTextFGColor(SELECTED_COLOR); }
				else {
					gfx_SetTextFGColor(TEXT_COLOR);
				}
				if (PRICE_BASIC_BOW > player_setup.money) {
					gfx_SetTextFGColor(RED_COLOR);
				}
				if (purchased[16] == 1) {
					gfx_SetTextFGColor(GREEN_COLOR);
				}
				gfx_PrintStringXY("Basic Bow    ", 150, 192);
				gfx_PrintInt(PRICE_BASIC_BOW, 2);
			}
			if (storepage == 2) {
				if (cursorposition == 1) { gfx_SetTextFGColor(SELECTED_COLOR); }
				else {
					gfx_SetTextFGColor(TEXT_COLOR);
				}
				if (PRICE_RECURVE_BOW > player_setup.money) {
					gfx_SetTextFGColor(RED_COLOR);
				}
				if (purchased[17] == 1) {
					gfx_SetTextFGColor(GREEN_COLOR);
				}
				gfx_PrintStringXY("Recurve Bow    ", 150, 132);
				gfx_PrintInt(PRICE_RECURVE_BOW, 2);

				if (cursorposition == 2) { gfx_SetTextFGColor(SELECTED_COLOR); }
				else {
					gfx_SetTextFGColor(TEXT_COLOR);
				}
				if (PRICE_COMPOUND_BOW > player_setup.money) {
					gfx_SetTextFGColor(RED_COLOR);
				}
				if (purchased[18] == 1) {
					gfx_SetTextFGColor(GREEN_COLOR);
				}
				gfx_PrintStringXY("Compound Bow    ", 150, 147);
				gfx_PrintInt(PRICE_COMPOUND_BOW, 3);

				if (cursorposition == 3) { gfx_SetTextFGColor(SELECTED_COLOR); }
				else {
					gfx_SetTextFGColor(TEXT_COLOR);
				}
				if (PRICE_DRAGON_BOW > player_setup.money) {
					gfx_SetTextFGColor(RED_COLOR);
				}
				if (purchased[19] == 1) {
					gfx_SetTextFGColor(GREEN_COLOR);
				}
				gfx_PrintStringXY("Dragon Bow    ", 150, 162);
				gfx_PrintInt(PRICE_DRAGON_BOW, 3);

				if (cursorposition == 4) { gfx_SetTextFGColor(SELECTED_COLOR); }
				else { gfx_SetTextFGColor(GRAY_COLOR); }
				gfx_PrintStringXY("--------", 150, 177);

				if (cursorposition == 5) { gfx_SetTextFGColor(SELECTED_COLOR); }
				else { gfx_SetTextFGColor(GRAY_COLOR); }
				gfx_PrintStringXY("--------", 150, 192);
			}
		}
		if (storepane == 3) {
			gfx_PrintStringXY("ITEMS", 208, 228);
			if (storepage == 1) {
				if (cursorposition == 1) { gfx_SetTextFGColor(SELECTED_COLOR); }
				else {
					gfx_SetTextFGColor(TEXT_COLOR);
				}
				if (PRICE_SMALL_POTION > player_setup.money) {
					gfx_SetTextFGColor(RED_COLOR);
				}
				gfx_PrintStringXY("Small Potion    ", 150, 132);
				gfx_PrintInt(PRICE_SMALL_POTION, 2);

				if (cursorposition == 2) { gfx_SetTextFGColor(SELECTED_COLOR); }
				else {
					gfx_SetTextFGColor(TEXT_COLOR);
				}
				if (PRICE_MEDIUM_POTION > player_setup.money) {
					gfx_SetTextFGColor(RED_COLOR);
				}
				gfx_PrintStringXY("Med. Potion    ", 150, 147);
				gfx_PrintInt(PRICE_MEDIUM_POTION, 2);

				if (cursorposition == 3) { gfx_SetTextFGColor(SELECTED_COLOR); }
				else {
					gfx_SetTextFGColor(TEXT_COLOR);
				}
				if (PRICE_LARGE_POTION > player_setup.money) {
					gfx_SetTextFGColor(RED_COLOR);
				}
				gfx_PrintStringXY("Large Potion    ", 150, 162);
				gfx_PrintInt(PRICE_LARGE_POTION, 3);

				if (cursorposition == 4) { gfx_SetTextFGColor(SELECTED_COLOR); }
				else { gfx_SetTextFGColor(GRAY_COLOR); }
				gfx_PrintStringXY("--------", 150, 177);

				if (cursorposition == 5) { gfx_SetTextFGColor(SELECTED_COLOR); }
				else { gfx_SetTextFGColor(GRAY_COLOR); }
				gfx_PrintStringXY("--------", 150, 192);
			}
		}

		gfx_SwapDraw();

		if (kb_Data[1] & kb_Window) {
			windowpress = 1;
		}
		if ((kb_Data[1] & kb_Zoom) || (kb_Data[7] & kb_Left)) {
			storepane--;
			if (storepane == 0) {
				storepane = 3;
			}
		}
		if ((kb_Data[1] & kb_Graph) || (kb_Data[7] & kb_Right)) {
			storepane++;
			if (storepane == 4) {
				storepane = 1;
			}
		}
		if (kb_Data[7] & kb_Down) {
			cursorposition++;
			if (cursorposition == 6) {
				cursorposition = 1;
				if (storepane == 1) {
					if (storepage == 1) { storepage = 2; }
					else if (storepage == 2) { storepage = 3; }
					else if (storepage == 3) { storepage = 1; }
				}
				if (storepane == 2) {
					if (storepage == 1) { storepage = 2; }
					else if (storepage == 2) { storepage = 1; }
				}
			}
		}
		if (kb_Data[7] & kb_Up) {
			cursorposition = (cursorposition - 1);
			if (cursorposition == 0) {
				cursorposition = 5;
				if (storepane == 1) {
					if (storepage == 2) { storepage = 1; }
					else if (storepage == 3) { storepage = 2; }
					else if (storepage == 1) { storepage = 3; }
				}
				if (storepane == 2) {
					if (storepage == 2) { storepage = 1; }
					else if (storepage == 1) { storepage = 2; }
				}
			}
		}
		if (kb_Data[1] & kb_2nd) {
			if (storepane == 1) {
				if (storepage == 1) {
					if (cursorposition == 1) {
						if (player_setup.money >= PRICE_LEATHER_HELMET) {
							player_setup.money = (player_setup.money - PRICE_LEATHER_HELMET);
							purchased[0] = 1;
						}
					}
					if (cursorposition == 2) {
						if (player_setup.money >= PRICE_LEATHER_TUNIC) {
							player_setup.money = (player_setup.money - PRICE_LEATHER_TUNIC);
							purchased[1] = 1;
						}
					}
					if (cursorposition == 3) {
						if (player_setup.money >= PRICE_LEATHER_BOOTS) {
							player_setup.money = (player_setup.money - PRICE_LEATHER_BOOTS);
							purchased[2] = 1;
						}
					}
					if (cursorposition == 4) {
						if (player_setup.money >= PRICE_IRON_HELMET) {
							player_setup.money = (player_setup.money - PRICE_IRON_HELMET);
							purchased[3] = 1;
						}
					}
					if (cursorposition == 5) {
						if (player_setup.money >= PRICE_CHAIN_SHIRT) {
							player_setup.money = (player_setup.money - PRICE_CHAIN_SHIRT);
							purchased[4] = 1;
						}
					}

				}
				if (storepage == 2) {
					if (cursorposition == 1) {
						if (player_setup.money >= PRICE_CHAIN_BOOTS) {
							player_setup.money = (player_setup.money - PRICE_CHAIN_BOOTS);
							purchased[5] = 1;
						}
					}
					if (cursorposition == 2) {
						if (player_setup.money >= PRICE_STEEL_HELMET) {
							player_setup.money = (player_setup.money - PRICE_STEEL_HELMET);
							purchased[6] = 1;
						}
					}
					if (cursorposition == 3) {
						if (player_setup.money >= PRICE_STEEL_CHESTPLATE) {
							player_setup.money = (player_setup.money - PRICE_STEEL_CHESTPLATE);
							purchased[7] = 1;
						}
					}
					if (cursorposition == 4) {
						if (player_setup.money >= PRICE_STEEL_BOOTS) {
							player_setup.money = (player_setup.money - PRICE_STEEL_BOOTS);
							purchased[8] = 1;
						}
					}
					if (cursorposition == 5) {
						if (player_setup.money >= PRICE_DRAGON_HELMET) {
							player_setup.money = (player_setup.money - PRICE_DRAGON_HELMET);
							purchased[9] = 1;
						}
					}
				}
				if (storepage == 3) {
					if (cursorposition == 1) {
						if (player_setup.money >= PRICE_DRAGON_CHESTPLATE) {
							player_setup.money = (player_setup.money - PRICE_DRAGON_CHESTPLATE);
							purchased[10] = 1;
						}
					}
					if (cursorposition == 2) {
						if (player_setup.money >= PRICE_DRAGON_BOOTS) {
							player_setup.money = (player_setup.money - PRICE_DRAGON_BOOTS);
							purchased[11] = 1;
						}
					}
				}
			}
			if (storepane == 2) {
				if (storepage == 1) {
					if (cursorposition == 1) {
						if (player_setup.money >= PRICE_CLUB) {
							player_setup.money = (player_setup.money - PRICE_CLUB);
							purchased[12] = 1;
						}
					}
					if (cursorposition == 2) {
						if (player_setup.money >= PRICE_IRON_SWORD) {
							player_setup.money = (player_setup.money - PRICE_IRON_SWORD);
							purchased[13] = 1;
						}
					}
					if (cursorposition == 3) {
						if (player_setup.money >= PRICE_STEEL_SWORD) {
							player_setup.money = (player_setup.money - PRICE_STEEL_SWORD);
							purchased[14] = 1;
						}
					}
					if (cursorposition == 4) {
						if (player_setup.money >= PRICE_DRAGON_SWORD) {
							player_setup.money = (player_setup.money - PRICE_DRAGON_SWORD);
							purchased[15] = 1;
						}
					}
					if (cursorposition == 5) {
						if (player_setup.money >= PRICE_BASIC_BOW) {
							player_setup.money = (player_setup.money - PRICE_BASIC_BOW);
							purchased[16] = 1;
						}
					}
				}
				if (storepage == 2) {
					if (cursorposition == 1) {
						if (player_setup.money >= PRICE_RECURVE_BOW) {
							player_setup.money = (player_setup.money - PRICE_RECURVE_BOW);
							purchased[17] = 1;
						}
					}
					if (cursorposition == 2) {
						if (player_setup.money >= PRICE_COMPOUND_BOW) {
							player_setup.money = (player_setup.money - PRICE_COMPOUND_BOW);
							purchased[18] = 1;
						}
					}
					if (cursorposition == 3) {
						if (player_setup.money >= PRICE_DRAGON_BOW) {
							player_setup.money = (player_setup.money - PRICE_DRAGON_BOW);
							purchased[19] = 1;
						}
					}
				}
			}
			if (storepane == 3) {
				if (storepage == 1) {
					if (cursorposition == 1) {
						if (player_setup.money >= PRICE_SMALL_POTION) {
							player_setup.money = (player_setup.money - PRICE_SMALL_POTION);
							purchased[20]++;
						}
					}
					if (cursorposition == 2) {
						if (player_setup.money >= PRICE_MEDIUM_POTION) {
							player_setup.money = (player_setup.money - PRICE_MEDIUM_POTION);
							purchased[21]++;
						}
					}
					if (cursorposition == 5) {
						if (player_setup.money >= PRICE_LARGE_POTION) {
							player_setup.money = (player_setup.money - PRICE_LARGE_POTION);
							purchased[22]++;
						}
					}
				}
			}
		}

		keywait();
		kb_SetMode(MODE_3_CONTINUOUS);
	} while (!(windowpress));
	kb_SetMode(MODE_3_CONTINUOUS);
}

void drawstatsmenu(void) {
	extern int dmgmultiplier;
	extern int blockchance;
	extern int playerdamage;
	extern int walkspeed;

	gfx_SetColor(SUBMENU_COLOR);
	gfx_FillRectangle(140, 115, 150, 105);
	gfx_SetColor(ACCENT_COLOR);
	gfx_Rectangle(140, 115, 150, 105);
	gfx_Rectangle(142, 117, 146, 101);
	submenubottombar();
	gfx_PrintStringXY("Money:  ", 150, 122);
	gfx_PrintUInt(player_setup.money, sizeof(player_setup.money));
	gfx_PrintStringXY("Player Damage:  ", 150, 137);
	gfx_PrintInt(playerdamage, 2);
	gfx_PrintStringXY("Damage Mult. :  ", 150, 152);
	gfx_PrintInt(dmgmultiplier, 2);
	gfx_PrintStringXY("Block % Chance:  ", 150, 167);
	gfx_PrintInt(blockchance, 2);
	gfx_PrintString("%");
	gfx_PrintStringXY("Walk Speed:  ", 150, 182);
	gfx_PrintInt(walkspeed, 2);
	gfx_PrintStringXY("Health:  ", 150, 197);
	gfx_PrintInt(player_setup.health, sizeof(player_setup.health));
	gfx_SwapDraw();
	do {
	} while (!(kb_Data[1] & kb_Window));
}

void drawoptions(void) {
	int cursorposition = 1;
	int optionspage = 1;
	int windowpress = 0;
	do {
		gfx_SetColor(SUBMENU_COLOR);
		gfx_FillRectangle(140, 125, 150, 95);
		gfx_SetColor(ACCENT_COLOR);
		gfx_Rectangle(140, 125, 150, 95);
		gfx_Rectangle(142, 127, 146, 91);
		submenubottombar();
		windowpress = 0;
		if (optionspage == 1) {


			if (cursorposition == 1) { gfx_SetTextFGColor(SELECTED_COLOR); }
			else { gfx_SetTextFGColor(TEXT_COLOR); }
			gfx_PrintStringXY("Toggle Minimap", 145, 132);
			if (showminimap == 1) { gfx_PrintStringXY("<ON>", 258, 132); }
			if (showminimap == 0) { gfx_PrintStringXY("<OFF>", 250, 132); }

			if (cursorposition == 2) { gfx_SetTextFGColor(SELECTED_COLOR); }
			else { gfx_SetTextFGColor(TEXT_COLOR); }
			if (showminimap == 0) { gfx_SetTextFGColor(RED_COLOR); }
			gfx_PrintStringXY("MM Position: ", 145, 147);
			if (minimapposition == 1) { gfx_PrintStringXY("<Top L>", 239, 148); }
			if (minimapposition == 2) { gfx_PrintStringXY("<Top R>", 239, 148); }
			if (minimapposition == 3) { gfx_PrintStringXY("<Btm L>", 240, 148); }
			if (minimapposition == 4) { gfx_PrintStringXY("<Btm R>", 240, 148); }

			if (cursorposition == 3) { gfx_SetTextFGColor(SELECTED_COLOR); }
			else { gfx_SetTextFGColor(TEXT_COLOR); }
			gfx_PrintStringXY("MM Hotkey", 145, 162);
			if (mmhotkey == 0) { gfx_PrintStringXY("<None>", 242, 162); }
			if (mmhotkey == 1) { gfx_PrintStringXY("<[alpha]>", 227, 162); }
			if (mmhotkey == 2) { gfx_PrintStringXY("<[mode]>", 232, 162); }
			if (mmhotkey == 3) { gfx_PrintStringXY("<[del]>", 243, 162); }


			if (cursorposition == 4) { gfx_SetTextFGColor(SELECTED_COLOR); }
			else { gfx_SetTextFGColor(TEXT_COLOR); }
			gfx_PrintStringXY("Enemy HP bar", 145, 177);
			if (drawhealth) { gfx_PrintStringXY("<ON>", 258, 177); }
			if (!drawhealth) { gfx_PrintStringXY("<OFF>", 250, 177); }

			if (cursorposition == 5) { gfx_SetTextFGColor(SELECTED_COLOR); }
			else { gfx_SetTextFGColor(TEXT_COLOR); }
			gfx_PrintStringXY("Damage Nums.", 145, 192);
			if (damagenumbers) { gfx_PrintStringXY("<ON>", 258, 192); }
			if (!damagenumbers) { gfx_PrintStringXY("<OFF>", 250, 192); }

			if (cursorposition == 6) { gfx_SetTextFGColor(SELECTED_COLOR); }
			else { gfx_SetTextFGColor(TEXT_COLOR); }
			gfx_PrintStringXY("[2nd] to SAVE", 145, 207);
		}

		gfx_SwapDraw();

		if (kb_Data[1] & kb_Window) {
			windowpress = 1;
		}
		if (kb_Data[7] & kb_Left) {
			if (optionspage == 1) {
				if (cursorposition == 1) {
					if (showminimap == 1) {
						showminimap = 0;
					} else if (showminimap == 0) {
						showminimap = 1;
					}
				}
				if (cursorposition == 2) {
					minimapposition = (minimapposition - 1);
					if (minimapposition <= 0) { minimapposition = 4; }
				}
				if (cursorposition == 3) {
					mmhotkey = mmhotkey++;
					if (mmhotkey > 3) {
						mmhotkey = 0;
					}
				}
				if (cursorposition == 4) {
					drawhealth = !drawhealth;
				}
				if (cursorposition == 5) {
					damagenumbers = !damagenumbers;
				}
			}
		}
		if (kb_Data[7] & kb_Right) {
			if (optionspage == 1) {
				if (cursorposition == 1) {
					if (showminimap == 1) {
						showminimap = 0;
					} else if (showminimap == 0) {
						showminimap = 1;
					}
				}
				if (cursorposition == 2) {
					minimapposition = (minimapposition + 1);
					if (minimapposition > 4) { minimapposition = 1; }
				}
				if (cursorposition == 3) {
					mmhotkey = mmhotkey--;
					if (mmhotkey < 0) {
						mmhotkey = 3;
					}
				}
				if (cursorposition == 4) {
					drawhealth = !drawhealth;
				}
				if (cursorposition == 5) {
					damagenumbers = !damagenumbers;
				}
			}
		}
		if (kb_Data[7] & kb_Down) {
			cursorposition++;
			if (cursorposition == 2) {
				if (showminimap == 0) {
					cursorposition = 3;
				}
			}
			if (cursorposition == 7) {
				cursorposition = 1;
			}
		}
		if (kb_Data[7] & kb_Up) {
			cursorposition = (cursorposition - 1);
			if (cursorposition == 2) {
				if (showminimap == 0) {
					cursorposition = 1;
				}
			}
			if (cursorposition == 0) {
				cursorposition = 6;
			}
		}
		if (kb_Data[1] & kb_2nd) {
			if (optionspage == 1) {
				if (cursorposition == 6) {
					//drawsavemenu();
				}
			}
		}
		keywait();
		kb_SetMode(MODE_3_CONTINUOUS);
	} while (!(windowpress));
	kb_SetMode(MODE_3_CONTINUOUS);
}

void drawpouch(void) {
	int windowpress = 0;

	do {
		gfx_FillScreen(SUBMENU_COLOR);
		gfx_SetColor(ACCENT_COLOR);
		gfx_Rectangle(0, 0, 320, 240);
		gfx_Rectangle(2, 2, 316, 236);

		player_setup.helmet = equipped[0];
		player_setup.chestplate = equipped[1];
		player_setup.boots = equipped[2];
		player_setup.weapon = equipped[3];

		gfx_SetColor(SUBMENU_COLOR);
		draweditedplayer();
		submenubottombar();

		updateinventory();

		gfx_SwapDraw();


		windowpress = 0;


		if (kb_Data[1] & kb_Window) {
			windowpress = 1;
		}

		keywait();
		kb_SetMode(MODE_3_CONTINUOUS);
	} while (!(windowpress));
	kb_SetMode(MODE_3_CONTINUOUS);
}

void updateinventory(void) {
	int i;
	int inv_x = 20;
	int inv_y = 95;
	int columns = 0;
	int tilespace = 24;
	for (i = 0; i < 22; i++) {
		if (purchased[i] != 0) {
			inventory[i] = purchased[i];
		}
	}
	for (i = 0; i < 22; i++) {
		//draw rectangle
		gfx_SetColor(0x00);
		gfx_FillRectangle(inv_x, inv_y, 20, 20);
		gfx_SetColor(ACCENT_COLOR);
		gfx_FillRectangle((inv_x + 2), (inv_y + 2), 16, 16);

		if (inventory[i] != 0) {
			//resize and draw tile
		}
		columns++;
		inv_x = (inv_x + tilespace);
		if (columns == 5) {
			inv_y = (inv_y + tilespace);
			columns = 0;
			inv_x = 20;
		}
	}
}

void submenubottombar(void) {
	gfx_SetColor(0x00);
	gfx_FillRectangle(0, 224, 320, 16);
	gfx_SetTextFGColor(TEXT_COLOR);
	gfx_PrintStringXY("[Close]", 80, 228);
}

void savegame(void) {
	//save player_setup to Appvar
}

void loadsave(void) {
	//load player_setup from Appvar
	mapstartx = player_setup.x;
	mapstarty = player_setup.y;
}

void newgame(void) {
	player_setup.health = 100;
	player_setup.money = 0;
	playerface = 4;

	for (i = 0; i < 22; i++) {
		purchased[i] = 0;
	}
	drawhealth = true;
	showminimap = 1;
	minimapposition = 1;
	num_p = 0;
	resetmoney();
	resetenemies();
	resetpots();

	resetrelics();
	resetbosses();
}

void playercreate(void) {
	int keypress;
	do {
		gfx_FillScreen(MENU_COLOR);
		gfx_SetColor(ACCENT_COLOR);
		gfx_Rectangle(0, 0, 320, 240);
		gfx_Rectangle(2, 2, 316, 236);
		gfx_HorizLine(0, 223, 320);
		gfx_HorizLine(0, 221, 320);
		gfx_SetTextTransparentColor(TRANSP_COLOR);
		gfx_SetTextFGColor(TEXT_COLOR);
		gfx_SetTextBGColor(TRANSP_COLOR);
		gfx_SetTextScale(2, 3);
		gfx_PrintStringXY("1.", 40, 20);
		gfx_PrintStringXY("2.", 40, 79);
		gfx_PrintStringXY("3.", 40, 137);
		gfx_PrintStringXY("4.", 40, 197);
		gfx_SetTextScale(1, 1);
		gfx_PrintStringXY("f(1-5) = change set, # = select item, del = back", 8, 227);
		gfx_SetTextFGColor(RED_COLOR);
		gfx_PrintStringXY("This character is for Free Play only!", 4, 4);

		drawequipment();
		gfx_SetColor(0x00);
		draweditedplayer();
		gfx_SwapDraw();

		do {
			keypress = 0;
			if (kb_Data[1] & kb_Yequ) {
				setnumber = NAKED;
				keypress = 1;
			} else if (kb_Data[1] & kb_Window) {
				setnumber = LEATHER;
				keypress = 1;
			} else if (kb_Data[1] & kb_Zoom) {
				setnumber = CHAIN;
				keypress = 1;
			} else if (kb_Data[1] & kb_Trace) {
				setnumber = STEEL;
				keypress = 1;
			} else if (kb_Data[1] & kb_Graph) {
				setnumber = DRAGON;
				keypress = 1;
			}

			if (kb_Data[3] & kb_1) {
				(player_setup.helmet = setnumber);
				keypress = 1;
			}
			if (kb_Data[4] & kb_2) {
				(player_setup.chestplate = setnumber);
				keypress = 1;
			}
			if (kb_Data[5] & kb_3) {
				(player_setup.boots = setnumber);
				keypress = 1;
			}
			if (kb_Data[3] & kb_4) {
				(player_setup.weapon = setnumber);
				keypress = 1;
			}
			if (kb_Data[1] & kb_Del) { keypress = 1; }
		} while (keypress == 0);

	} while (!(kb_Data[1] & kb_Del));
}

void draweditedplayer(void) {
	gfx_FillRectangle(editpx, editpy + 2, ((editscale * 32) - 15), (editscale * 32));
	gfx_ScaledTransparentSprite_NoClip(player_naked_down, editpx, editpy, editscale, editscale);
	if (player_setup.helmet == 1) {
		gfx_ScaledTransparentSprite_NoClip(leather_helmet_down, editpx, editpy, editscale, editscale);
	}
	if (player_setup.chestplate == 1) {
		gfx_ScaledTransparentSprite_NoClip(leather_chestplate_down, editpx, editpy, editscale, editscale);
	}
	if (player_setup.boots == 1) {
		gfx_ScaledTransparentSprite_NoClip(leather_boots_down, editpx, editpy + (27 * editscale), editscale, editscale);
	}
	if (player_setup.weapon == 1) {
		gfx_ScaledTransparentSprite_NoClip(club, editweaponx, editweapony, editscale - 1, editscale - 1);
	}

	if (player_setup.helmet == 2) {
		gfx_ScaledTransparentSprite_NoClip(chain_helmet_down, editpx, editpy, editscale, editscale);
	}
	if (player_setup.chestplate == 2) {
		gfx_ScaledTransparentSprite_NoClip(chain_chestplate_down, editpx, editpy, editscale, editscale);
	}
	if (player_setup.boots == 2) {
		gfx_ScaledTransparentSprite_NoClip(chain_boots_down, editpx, editpy + (27 * editscale), editscale, editscale);
	}
	if (player_setup.weapon == 2) {
		gfx_ScaledTransparentSprite_NoClip(iron_sword, editweaponx, editweapony, editscale - 1, editscale - 1);
	}

	if (player_setup.helmet == 3) {
		gfx_ScaledTransparentSprite_NoClip(steel_helmet_down, editpx, editpy, editscale, editscale);
	}
	if (player_setup.chestplate == 3) {
		gfx_ScaledTransparentSprite_NoClip(steel_chestplate_down, editpx, editpy, editscale, editscale);
	}
	if (player_setup.boots == 3) {
		gfx_ScaledTransparentSprite_NoClip(steel_boots_down, editpx, editpy + (27 * editscale), editscale, editscale);
	}
	if (player_setup.weapon == 3) {
		gfx_ScaledTransparentSprite_NoClip(steel_sword, editweaponx, editweapony, editscale - 1, editscale - 1);
	}

	if (player_setup.helmet == 4) {
		gfx_ScaledTransparentSprite_NoClip(dragon_helmet_down, editpx, editpy, editscale, editscale);
	}
	if (player_setup.chestplate == 4) {
		gfx_ScaledTransparentSprite_NoClip(dragon_chestplate_down, editpx, editpy, editscale, editscale);
	}
	if (player_setup.boots == 4) {
		gfx_ScaledTransparentSprite_NoClip(dragon_boots_down, editpx, editpy + (27 * editscale), editscale, editscale);
	}
	if (player_setup.weapon == 4) {
		gfx_ScaledTransparentSprite_NoClip(dragon_sword, editweaponx, editweapony, editscale - 1, editscale - 1);
	}
}

void drawequipment(void) {
	gfx_SetColor(0x00);
	gfx_FillRectangle(65, 12, 96, 208);

	if (setnumber == LEATHER) {
		gfx_ScaledTransparentSprite_NoClip(leather_helmet_down, 50, 15, 3, 3);
		gfx_ScaledTransparentSprite_NoClip(leather_chestplate_down, 50, 25, 3, 3);
		gfx_ScaledTransparentSprite_NoClip(leather_boots_down, 50, 142, 3, 3);
		gfx_ScaledTransparentSprite_NoClip(club, editweaponsmallx, editweaponsmally, 2, 2);
	}
	if (setnumber == CHAIN) {
		gfx_ScaledTransparentSprite_NoClip(chain_helmet_down, 50, 15, 3, 3);
		gfx_ScaledTransparentSprite_NoClip(chain_chestplate_down, 50, 25, 3, 3);
		gfx_ScaledTransparentSprite_NoClip(chain_boots_down, 50, 142, 3, 3);
		gfx_ScaledTransparentSprite_NoClip(iron_sword, editweaponsmallx, editweaponsmally, 2, 2);
	}
	if (setnumber == STEEL) {
		gfx_ScaledTransparentSprite_NoClip(steel_helmet_down, 50, 15, 3, 3);
		gfx_ScaledTransparentSprite_NoClip(steel_chestplate_down, 50, 25, 3, 3);
		gfx_ScaledTransparentSprite_NoClip(steel_boots_down, 50, 142, 3, 3);
		gfx_ScaledTransparentSprite_NoClip(steel_sword, editweaponsmallx, editweaponsmally, 2, 2);
	}
	if (setnumber == DRAGON) {
		gfx_ScaledTransparentSprite_NoClip(dragon_helmet_down, 50, 15, 3, 3);
		gfx_ScaledTransparentSprite_NoClip(dragon_chestplate_down, 50, 25, 3, 3);
		gfx_ScaledTransparentSprite_NoClip(dragon_boots_down, 50, 142, 3, 3);
		gfx_ScaledTransparentSprite_NoClip(dragon_sword, editweaponsmallx, editweaponsmally, 2, 2);
	}
}

void menubkgnd(void) {
	/*
	gfx_FillScreen(MENU_COLOR);
	gfx_SetColor(ACCENT_COLOR);
	gfx_Rectangle(0,0,320,240);
	gfx_Rectangle(2,2,316,236);
	gfx_SwapDraw();
	*/
}

void resetenemies(void) {
	for (i = 0; i < NUM_ENEMIES; i++) {
		enemy[i].type = defaultenemy_typelist[i];
		enemy[i].dead = 0;
		enemy[i].x = defaultenemy_xlist[i] * 32;
		enemy[i].y = defaultenemy_ylist[i] * 32;
		enemy[i].move = defaultenemy_movelist[i];
		enemy[i].health = ((enemy[i].type + 1) * 10);
	}
}

void resetmoney(void) {
	for (i = 0; i < NUM_POTS; i++) {
		money[i].moneydead = 1;
	}
}

void resetrelics(void) {
	for (i = 0; i < NUM_RELICS; i++) {
		relic[i].relicshow = 0;
		relic[i].relictaken = 0;
		relic[i].relicnumber = i;
	}
}

void resetbosses(void) {
	for (i = 0; i < NUM_BOSS; i++) {
		boss[i].boss_type = defaultboss_type[i];
		boss[i].boss_dead = 0;
		boss[i].boss_health = ((boss[i].boss_type + 1) * 100);
		boss[i].boss_relicnum = i;
		boss[i].b_x = defaultboss_xlist[i];
		boss[i].b_y = defaultboss_ylist[i];
	}
}

void updateenemies(void) {
	int randcheck = 20;
	for (ii = 0; ii < NUM_ENEMIES; ii++) {
		if ((enemy[ii].type) == 0) { enemySprite = slime_blue; }
		if ((enemy[ii].type) == 1) { enemySprite = slime_green; }
		if ((enemy[ii].type) == 2) { enemySprite = slime_red; }
		if ((enemy[ii].type) == 3) { enemySprite = bokoblin_light; }
		if ((enemy[ii].type) == 4) { enemySprite = bokoblin_dark; }
		if ((enemy[ii].type) == 5) { enemySprite = knight_green; }
		if ((enemy[ii].type) == 6) { enemySprite = knight_red; }

		//if within current screen view
		if (enemy[ii].health <= 0) {
			if (enemy[ii].dead == 0) {
				enemy[ii].dead = 1;

				money[ii].m_x = enemy[ii].x;
				money[ii].m_y = enemy[ii].y;
				money[ii].moneydead = 0;
				money[ii].moneyvalue = 1;
				randcheck = 30;
				if (randcheck >= randInt(0, 300)) {
					money[ii].moneyvalue = 5;
				}
				randcheck = 20;
				if (randcheck >= randInt(0, 300)) {
					money[ii].moneyvalue = 10;
				}
				randcheck = 10;
				if (randcheck >= randInt(0, 300)) {
					money[ii].moneyvalue = 20;
				}
				randcheck = 1;
				if (randcheck >= randInt(0, 300)) {
					money[ii].moneyvalue = 100;
				}
			}

		}
		if ((enemy[ii].dead) == 0) {
			if (tmap_pxl_x_offset <= (enemy[ii].x)) {
				if ((enemy[ii].x) <= (tmap_pxl_x_offset + 288)) {
					if (tmap_pxl_y_offset <= (enemy[ii].y)) {
						if ((enemy[ii].y) <= (tmap_pxl_y_offset + 192)) {
							if (enemy[ii].move == 1) {
								if (randcheck >= randInt(0, 100)) {
									enemymove();
								}
							}
						}
						renderenemy(&enemy[ii]);
					}
				}
			}
		}
	}
}

void renderenemy(enemy_t *enemy) {
	gfx_TransparentSprite(enemySprite, enemy->x - tmap_pxl_x_offset, enemy->y - tmap_pxl_y_offset);
	gfx_SetTextFGColor(RED_COLOR);
	if (drawhealth) {
		gfx_SetTextXY(enemy->x - tmap_pxl_x_offset, enemy->y - tmap_pxl_y_offset);
		gfx_PrintUInt(enemy->health, 2);
	}
	/*only for debug
	gfx_PrintString("---");
	gfx_PrintUInt((enemy->x)/32,3);
	gfx_PrintString("-");
	gfx_PrintUInt((enemy->y)/32,3);
	*/

	enemyattack();
}
