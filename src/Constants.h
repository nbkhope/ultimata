/** @file
 */

#ifndef __CONSTANTS_H__
#define __CONSTANTS_H__

#define __DEBUG_MODE__
//#define __TILE_MOVEMENT__

#include <string>

#include <SDL2/SDL.h>

// Global constants to hold the screen settings
/* The screen dimensions */
//const int SCREEN_WIDTH = 640;
const int SCREEN_WIDTH = 720;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;

// a little fix for xcode now:
//#define __TILEMOVE__
#ifndef __TILEMOVE__
const int FRAMES_PER_SECOND = 30;
#else
const int FRAMES_PER_SECOND = 10;
#endif

const int DEFAULT_MAP_WIDTH = 720; // 1024
const int DEFAULT_MAP_HEIGHT = 480; // 768

const int MINIMAP_WIDTH = 160;
const int MINIMAP_HEIGHT = 120;

/* The size of each game map tile */
const int TILESIZE = 48; // 32
const int TILESET_SIZE = 256;
const int TILESET_TILES = 64;

//const char DEFAULT_FONT[] = "cour.ttf";
const std::string DEFAULT_FONT = "data/fonts/cour.ttf";
const int FONTSIZE = 24;

#ifndef __TILEMOVE__
// free movement
const int PLAYER_MOVEMENT = TILESIZE / 4;
#else
// tile based movement
const int PLAYER_MOVEMENT = TILESIZE;
#endif

const int PLAYER_MOVE_SPEED = TILESIZE / 4;

// Networking constants
const int MAX_MESSAGE_SIZE = 256;  // Buffer size for network messages
const int MAX_PACKET_SIZE = 1024;  // Maximum packet size

// Network message types - shared between client and server
namespace NetworkMessages {
    const char* const DISCONNECT = "disconnect";
    const char* const PLAYER_POSITION = "player_position";
    const char* const PLAYER_CONNECT = "player_connect";
    const char* const SERVER_WELCOME = "server_welcome";
}

// Network command constants (binary protocol)
namespace NetworkCommands {
    const int MOVE = 1;
    const int STOP = 2;
    const int ATTACK = 3;
}

enum GAME_MODE { GAME_MODE_TEST, GAME_MODE_NORMAL };

/**
 * For the direction a creature is facing
 * The order here is important, as the player charset poses
 * go from top to bottom in the given facing directions.
 * 0 1 2 3
 */
enum Direction
{
	DIRECTION_SOUTH,
	DIRECTION_NORTH,
	DIRECTION_EAST,
	DIRECTION_WEST
	
};

#define __TEXTURE_RENDERING__

enum ItemType
{
	UNUSABLE,
	WEAPON,
	SHIELD,
	POTION
};

enum TileType
{
	WALKABLE,
	NONWALKABLE,
	WATER
};

struct Circle
{
	int x, y;
	int r;
};
/*
enum Key
{
	KEY_A = SDLK_A,
	KEY_B = SDLK_B,
	KEY_C = SDLK_C,
	KEY_D = SDLK_D,
	KEY_E = SDLK_E,
	KEY_F = SDLK_F,
	KEY_G = SDLK_G,
	KEY_H = SDLK_H,
	KEY_I = SDLK_I,
	KEY_J = SDLK_J,
	KEY_K = SDLK_K,
	KEY_L = SDLK_L,
	KEY_M = SDLK_M,
	KEY_N = SDLK_N,
	KEY_O = SDLK_O,
	KEY_P = SDLK_P,
	KEY_Q = SDLK_Q,
	KEY_R = SDLK_R,
	KEY_S = SDLK_S,
	KEY_T = SDLK_T,
	KEY_U = SDLK_U,
	KEY_V = SDLK_V,
	KEY_W = SDLK_W,
	KEY_X = SDLK_X,
	KEY_Y = SDLK_Y,
	KEY_Z = SDLK_Z
};*/
#endif
