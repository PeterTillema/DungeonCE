#ifndef DEFINES_H
#define DEFINES_H

// Colors
#define TEXT_COLOR     			0xE6
#define ACCENT_COLOR   			0x80
#define MENU_COLOR     			0x00
#define SUBMENU_COLOR  			0x4A
#define TRANSP_COLOR   			0xF8
#define RED_COLOR      			0xE8
#define GREEN_COLOR    			0x25
#define YELLOW_COLOR   			0xE7
#define GRAY_COLOR     			0xAC
#define SELECTED_COLOR 			0xE3

// Prices
#define PRICE_LEATHER_HELMET 	10
#define PRICE_LEATHER_TUNIC 	10
#define PRICE_LEATHER_BOOTS 	10
#define PRICE_IRON_HELMET 		50
#define PRICE_CHAIN_SHIRT 		50
#define PRICE_CHAIN_BOOTS 		50
#define PRICE_STEEL_HELMET      100
#define PRICE_STEEL_CHESTPLATE  100
#define PRICE_STEEL_BOOTS       100
#define PRICE_DRAGON_HELMET     500
#define PRICE_DRAGON_CHESTPLATE 500
#define PRICE_DRAGON_BOOTS 		500
#define PRICE_CLUB 				10
#define PRICE_IRON_SWORD 		50
#define PRICE_STEEL_SWORD 		100
#define PRICE_DRAGON_SWORD 		500
#define PRICE_BASIC_BOW 		10
#define PRICE_RECURVE_BOW 		50
#define PRICE_COMPOUND_BOW 		100
#define PRICE_DRAGON_BOW 		500
#define PRICE_SMALL_POTION 		10
#define PRICE_MEDIUM_POTION 	50
#define PRICE_LARGE_POTION 		150


// Amounts
#define NUM_ENEMIES  			100
#define NUM_PROJECTILES         100
#define NUM_POTS  				100
#define NUM_RELICS  			5
#define NUM_BOSS  				3

// Tilemaps
#define TILEMAP_TILE_SIZE   	32
#define TILEMAP_WIDTH       	40
#define TILEMAP_HEIGHT      	200
#define TILEMAP_DRAW_WIDTH  	10
#define TILEMAP_DRAW_HEIGHT 	7
#define TILEMAP_MAX_X 			(TILEMAP_WIDTH * TILEMAP_TILE_SIZE)
#define TILEMAP_MAX_Y 			(TILEMAP_HEIGHT * TILEMAP_TILE_SIZE)
#define Y_OFFSET 				0
#define X_OFFSET 				0

// Minimap
#define MINIMAP_COLS 			20
#define MINIMAP_ROWS 			14
#define MINIMAP_TILE_SIZE 		5
#define MINIMAP_WIDTH 			(MINIMAP_COLS * MINIMAP_TILE_SIZE)
#define MINIMAP_HEIGHT 			(MINIMAP_ROWS * MINIMAP_TILE_SIZE)
#define MINIMAP_SCALE_FACTOR 	(TILEMAP_TILE_SIZE / MINIMAP_TILE_SIZE)
#define MINIMAP_MAX_X 			(TILEMAP_WIDTH * MINIMAP_TILE_SIZE)
#define MINIMAP_MAX_Y 			(TILEMAP_HEIGHT * MINIMAP_TILE_SIZE)

#define MAP_SHIFT				32

// Structs and enums
typedef struct {
    uint8_t type;
    int dead;
    uint24_t x;
    uint24_t y;
    int health;
    int move;
} enemy_t;

typedef struct {
    uint8_t pottype;
    int potdead;
    uint24_t p_x;
    uint24_t p_y;
} pots_t;


typedef struct {
    uint8_t moneyvalue;
    int moneydead;
    uint24_t m_x;
    uint24_t m_y;
} money_t;


typedef struct {
    int relicnumber;
    int relicshow;
    int relictaken;
    uint24_t r_x;
    uint24_t r_y;
} relic_t;

typedef struct boss {
    int boss_type;
    int boss_dead;
    int boss_health;
    int boss_relicnum;
    uint24_t b_x;
    uint24_t b_y;
} boss_t;

typedef struct {
    int p_type;
    int p_alive;
    int p_direction;
    int p_speed;
    int p_vx;
    int p_vy;
    int p_x;
    int p_y;
} projectile_t;

typedef struct {
    uint24_t helmet;
    uint24_t chestplate;
    uint24_t boots;
    uint24_t weapon;
    uint24_t x;
    uint24_t y;
    uint24_t health;
    uint24_t money;
} player_setup_t;

typedef enum {
    NAKED,
    LEATHER,
    CHAIN,
    STEEL,
    DRAGON		// rawr
} TYPE_SET_NUMBER;

#endif