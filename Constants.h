//	Copyright (C) 2010-2011, 2015  Renan Martins
//
//	This program is free software; you can redistribute it and/or
//	modify it under the terms of the GNU General Public License
//	as published by the Free Software Foundation; either version 2
//	of the License, or (at your option) any later version.
//
//	This program is distributed in the hope that it will be useful,
//	but WITHOUT ANY WARRANTY; without even the implied warranty of
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//	GNU General Public License for more details.
//
//	You should have received a copy of the GNU General Public License
//	along with this program; if not, write to the Free Software
//	Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

/** @file
 */

#ifndef __CONSTANTS_H__
#define __CONSTANTS_H__

#define __DEBUG_MODE__
//#define __TILE_MOVEMENT__

const int FRAMES_PER_SECOND = 30;

/* The screen dimensions */
//const int SCREEN_WIDTH = 640;
const int SCREEN_WIDTH = 720;
const int SCREEN_HEIGHT = 480;

const int DEFAULT_MAP_WIDTH = 720;
const int DEFAULT_MAP_HEIGHT = 480;
/* The size of each game map tile */
const int TILESIZE = 48;

const int PLAYER_MOVE_SPEED = TILESIZE / 4;

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

#endif
