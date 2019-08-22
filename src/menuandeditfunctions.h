#ifndef MENUANDEDITFUNCTIONS_H
#define MENUANDEDITFUNCTIONS_H

typedef enum {
	NAKED,
	LEATHER,
	CHAIN,
	STEEL,
	DRAGON		// rawr
} TYPE_SET_NUMBER;

void keywait();

void keyclear();

void mainmenu();

void menubkgnd();

void newgame();

void loadsave();

void options();

void playercreate();

void drawequipment();

void draweditedplayer();

void savegame();

void drawsavemenu();

void drawstatsmenu();

void drawstore();

void drawoptions();

void drawpouch();

void updateinventory();

void submenubottombar();

void resetmoney();

void resetenemies();

void resetrelics();

void resetbosses();

void updateenemies();

void renderenemy();

#endif