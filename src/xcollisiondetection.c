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

extern enemy_t enemy[NUM_ENEMIES];
extern money_t money[NUM_POTS];
extern pots_t pots[NUM_POTS];
extern projectile_t projectile[NUM_PROJECTILES];

uint24_t tmap_pxl_x_offset;
uint24_t tmap_pxl_y_offset;






int playerdamage;
extern int i;
extern player_setup_t player_setup;
extern int playerface;
extern int goup;
extern int godown;
extern int goleft;
extern int goright;
extern gfx_tilemap_t tilemap;
int replacex;
int replacey;
int hit;
extern int p_hit;
extern int num_p;
extern int dmgmultiplier;
extern int player;

int enemy_maptilex;
int enemy_maptiley;
int pot_maptilex;
int pot_maptiley;
extern int player_mappxlx;
extern int player_mappxly;
extern int player_maptilex;
extern int player_maptiley;
extern int *inputx;
extern int *inputy;
extern unsigned int tmap_pxl_x_offset;
extern unsigned int tmap_pxl_y_offset;
int enemycoordx;
int enemycoordy;
extern int ii;
int healthincrement = 10;

void collisionleft(void) {
	enemycoordx = (enemy[ii].x / 32);
	enemycoordy = (enemy[ii].y / 32);
	goleft = 1;
	if (player == 1) {
		if (((gfx_GetTile(&tilemap, player_mappxlx - 32, player_mappxly)) >= 18)) {
			goleft = 0;
		}
	}
	if (player == 2) {
		gfx_SetTextXY(enemy[ii].x, enemy[ii].y);
		gfx_PrintInt(gfx_GetTile(&tilemap, enemycoordx - 32, enemycoordy), 3);
		gfx_PrintString("-");
		gfx_PrintInt(enemycoordx, 3);
		if (((gfx_GetTile(&tilemap, enemycoordx - 32, enemycoordy)) >= 18)) {
			goleft = 0;
		}
	}

	for (i = 0; i < NUM_ENEMIES; i++) {
		enemy_maptilex = (enemy[i].x / 32);
		enemy_maptiley = (enemy[i].y / 32);
		if (tmap_pxl_x_offset <= (enemy[i].x)) {
			if ((enemy[i].x) <= (tmap_pxl_x_offset + 288)) {
				if (tmap_pxl_y_offset <= (enemy[i].y)) {
					if ((enemy[i].y) <= (tmap_pxl_y_offset + 192)) {
						if (((*inputx - 1) == enemy_maptilex) && (*inputy == enemy_maptiley)) {
							if (enemy[i].dead == 0) {
								goleft = 0;
							}
						}
					}
				}
			}
		}
	}
	for (i = 0; i < NUM_POTS; i++) {
		pot_maptilex = (pots[i].p_x / 32);
		pot_maptiley = (pots[i].p_y / 32);
		if (tmap_pxl_x_offset <= (pots[i].p_x)) {
			if ((pots[i].p_x) <= (tmap_pxl_x_offset + 288)) {
				if (tmap_pxl_y_offset <= (pots[i].p_y)) {
					if ((pots[i].p_y) <= (tmap_pxl_y_offset + 192)) {
						if (((*inputx - 1) == pot_maptilex) && (*inputy == pot_maptiley)) {
							if (pots[i].potdead == 0) {
								goleft = 0;
							}
						}
					}
				}
			}
		}
	}
}

void collisionright(void) {
	enemycoordx = (enemy[ii].x / 32);
	enemycoordy = (enemy[ii].y / 32);
	goright = 1;
	if (player == 1) {
		if (((gfx_GetTile(&tilemap, player_mappxlx + 32, player_mappxly)) >= 18)) {
			goright = 0;
		}
	}
	if (player == 2) {
		if (((gfx_GetTile(&tilemap, enemycoordx + 32, enemycoordy)) >= 18)) {
			goright = 0;
		}
	}
	for (i = 0; i < NUM_ENEMIES; i++) {
		enemy_maptilex = (enemy[i].x / 32);
		enemy_maptiley = (enemy[i].y / 32);
		if (tmap_pxl_x_offset <= (enemy[i].x)) {
			if ((enemy[i].x) <= (tmap_pxl_x_offset + 288)) {
				if (tmap_pxl_y_offset <= (enemy[i].y)) {
					if ((enemy[i].y) <= (tmap_pxl_y_offset + 192)) {
						if (((*inputx + 1) == enemy_maptilex) && (*inputy == enemy_maptiley)) {
							if (enemy[i].dead == 0) {
								goright = 0;
							}
						}
					}
				}
			}
		}

	}
	for (i = 0; i < NUM_POTS; i++) {
		pot_maptilex = (pots[i].p_x / 32);
		pot_maptiley = (pots[i].p_y / 32);
		if (tmap_pxl_x_offset <= (pots[i].p_x)) {
			if ((pots[i].p_x) <= (tmap_pxl_x_offset + 288)) {
				if (tmap_pxl_y_offset <= (pots[i].p_y)) {
					if ((pots[i].p_y) <= (tmap_pxl_y_offset + 192)) {
						if (((*inputx + 1) == pot_maptilex) && (*inputy == pot_maptiley)) {
							if (pots[i].potdead == 0) {
								goright = 0;
							}
						}
					}
				}
			}
		}
	}
}

void collisionup(void) {
	enemycoordx = (enemy[ii].x / 32);
	enemycoordy = (enemy[ii].y / 32);
	goup = 1;
	if (player == 1) {
		if (((gfx_GetTile(&tilemap, player_mappxlx, player_mappxly - 32)) >= 18)) {
			goup = 0;
		}
	}
	if (player == 2) {
		if (((gfx_GetTile(&tilemap, enemycoordx, enemycoordy - 32)) >= 18)) {
			goup = 0;
		}
	}
	for (i = 0; i < NUM_ENEMIES; i++) {
		enemy_maptilex = (enemy[i].x / 32);
		enemy_maptiley = (enemy[i].y / 32);
		if (tmap_pxl_x_offset <= (enemy[i].x)) {
			if ((enemy[i].x) <= (tmap_pxl_x_offset + 288)) {
				if (tmap_pxl_y_offset <= (enemy[i].y)) {
					if ((enemy[i].y) <= (tmap_pxl_y_offset + 192)) {
						if ((*inputx == enemy_maptilex) && ((*inputy - 1) == enemy_maptiley)) {
							if (enemy[i].dead == 0) {
								goup = 0;
							}
						}
					}
				}
			}
		}
	}
	for (i = 0; i < NUM_POTS; i++) {
		pot_maptilex = (pots[i].p_x / 32);
		pot_maptiley = (pots[i].p_y / 32);
		if (tmap_pxl_x_offset <= (pots[i].p_x)) {
			if ((pots[i].p_x) <= (tmap_pxl_x_offset + 288)) {
				if (tmap_pxl_y_offset <= (pots[i].p_y)) {
					if ((pots[i].p_y) <= (tmap_pxl_y_offset + 192)) {
						if ((*inputx == pot_maptilex) && ((*inputy - 1) == pot_maptiley)) {
							if (pots[i].potdead == 0) {
								goup = 0;
							}
						}
					}
				}
			}
		}
	}
}

void collisiondown(void) {
	enemycoordx = (enemy[ii].x / 32);
	enemycoordy = (enemy[ii].y / 32);
	godown = 1;

	if (player == 1) {
		if (((gfx_GetTile(&tilemap, player_mappxlx, player_mappxly + 32)) >= 18)) {
			godown = 0;
		}
	}
	if (player == 2) {
		if (((gfx_GetTile(&tilemap, enemycoordx, enemycoordy + 32)) >= 18)) {
			godown = 0;
		}
	}

	for (i = 0; i < NUM_ENEMIES; i++) {
		enemy_maptilex = (enemy[i].x / 32);
		enemy_maptiley = (enemy[i].y / 32);
		if (tmap_pxl_x_offset <= (enemy[i].x)) {
			if ((enemy[i].x) <= (tmap_pxl_x_offset + 288)) {
				if (tmap_pxl_y_offset <= (enemy[i].y)) {
					if ((enemy[i].y) <= (tmap_pxl_y_offset + 192)) {
						if ((*inputx == enemy_maptilex) && ((*inputy + 1) == enemy_maptiley)) {
							if (enemy[i].dead == 0) {
								godown = 0;
							}
						}
					}
				}
			}
		}
	}
	for (i = 0; i < NUM_POTS; i++) {
		pot_maptilex = (pots[i].p_x / 32);
		pot_maptiley = (pots[i].p_y / 32);
		if (tmap_pxl_x_offset <= (pots[i].p_x)) {
			if ((pots[i].p_x) <= (tmap_pxl_x_offset + 288)) {
				if (tmap_pxl_y_offset <= (pots[i].p_y)) {
					if ((pots[i].p_y) <= (tmap_pxl_y_offset + 192)) {
						if ((*inputx == pot_maptilex) && ((*inputy + 1) == pot_maptiley)) {
							if (pots[i].potdead == 0) {
								godown = 0;
							}
						}
					}
				}
			}
		}
	}
}

void projectilemapcollision(void) {
	if (projectile[i].p_type == 1) {
		if (gfx_GetTile(&tilemap, projectile[i].p_x, projectile[i].p_y) >= 18) {
			p_hit = 1;
		} else if (projectile[i].p_direction == 3) {
			if (gfx_GetTile(&tilemap, (projectile[i].p_x + 32), projectile[i].p_y) >= 18) {
				p_hit = 1;
				projectile[i].p_x = (projectile[i].p_x + 32);
			}
		} else if (projectile[i].p_direction == 4) {
			if (gfx_GetTile(&tilemap, projectile[i].p_x, (projectile[i].p_y + 32)) >= 18) {
				p_hit = 1;
				projectile[i].p_y = (projectile[i].p_y + 32);
			}
		}

	}
}

void projectileentitycollision(void) {

}

void playerattackhitcheck(void) {
	int money_maptilex;
	int money_maptiley;
	int randcheck = 1;
///*money
	for (i = 0; i < NUM_POTS; i++) {
		money_maptilex = (money[i].m_x / 32);
		money_maptiley = (money[i].m_y / 32);
		hit = 0;
		if (playerface == 1) {
			if (((player_maptilex - 1) == money_maptilex) & (player_maptiley == money_maptiley)) {
				hit = 1;
			}
		}
		if (playerface == 2) {
			if ((player_maptilex == money_maptilex) & ((player_maptiley - 1) == money_maptiley)) {
				hit = 1;
			}
		}
		if (playerface == 3) {
			if (((player_maptilex + 1) == money_maptilex) & (player_maptiley == money_maptiley)) {
				hit = 1;
			}
		}
		if (playerface == 4) {
			if ((player_maptilex == money_maptilex) & ((player_maptiley + 1) == money_maptiley)) {
				hit = 1;
			}
		}

		if (hit == 1) {
			if (money[i].moneydead == 0) {
				player_setup.money = (player_setup.money + money[i].moneyvalue);
				if (player_setup.money > 99999) { player_setup.money = 99999; }
			}
			money[i].moneydead = 1;
		}
	}
//*/
//enemies
	for (i = 0; i < NUM_ENEMIES; i++) {
		enemy_maptilex = (enemy[i].x / 32);
		enemy_maptiley = (enemy[i].y / 32);
		hit = 0;
		if (playerface == 1) {
			if (((player_maptilex - 1) == enemy_maptilex) & (player_maptiley == enemy_maptiley)) {
				hit = 1;
			}
		}
		if (playerface == 2) {
			if ((player_maptilex == enemy_maptilex) & ((player_maptiley - 1) == enemy_maptiley)) {
				hit = 1;
			}
		}
		if (playerface == 3) {
			if (((player_maptilex + 1) == enemy_maptilex) & (player_maptiley == enemy_maptiley)) {
				hit = 1;
			}
		}
		if (playerface == 4) {
			if ((player_maptilex == enemy_maptilex) & ((player_maptiley + 1) == enemy_maptiley)) {
				hit = 1;
			}
		}
		if (hit == 1) {
			(enemy[i].health) = ((enemy[i].health) - (playerdamage * dmgmultiplier));
		}
	}
//pots
	for (i = 0; i < NUM_POTS; i++) {
		pot_maptilex = (pots[i].p_x / 32);
		pot_maptiley = (pots[i].p_y / 32);
		hit = 0;
		if (playerface == 1) {
			if (((player_maptilex - 1) == pot_maptilex) & (player_maptiley == pot_maptiley)) {
				hit = 1;
			}
		}
		if (playerface == 2) {
			if ((player_maptilex == pot_maptilex) & ((player_maptiley - 1) == pot_maptiley)) {
				hit = 1;
			}
		}
		if (playerface == 3) {
			if (((player_maptilex + 1) == pot_maptilex) & (player_maptiley == pot_maptiley)) {
				hit = 1;
			}
		}
		if (playerface == 4) {
			if ((player_maptilex == pot_maptilex) & ((player_maptiley + 1) == pot_maptiley)) {
				hit = 1;
			}
		}
		if (hit == 1) {
			if (pots[i].potdead == 0) {
				if (pots[i].pottype == 1) {
					player_setup.health = (player_setup.health + healthincrement);
				}
				money[i].m_x = pots[i].p_x;
				money[i].m_y = pots[i].p_y;
				money[i].moneydead = 0;
				money[i].moneyvalue = 1;
				randcheck = 30;
				if (randcheck >= randInt(0, 100)) {
					money[i].moneyvalue = 5;
				}
				randcheck = 20;
				if (randcheck >= randInt(0, 100)) {
					money[i].moneyvalue = 10;
				}
				randcheck = 10;
				if (randcheck >= randInt(0, 100)) {
					money[i].moneyvalue = 20;
				}
				randcheck = 1;
				if (randcheck >= randInt(0, 100)) {
					money[i].moneyvalue = 100;
				}
			}
			pots[i].potdead = 1;
		}
	}
}