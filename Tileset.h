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

#ifndef __TILESET_H__
#define __TILESET_H__

#include <iostream>

#include "SDL2/SDL.h"

#include "Constants.h"

using namespace std;

/**
 * A Tileset object keeps information about a tilesheet, 
 * but has nothing to do with its rendering.
 */
class Tileset
{
public:
	Tileset();
	~Tileset();
	
	/**
	 * Returns a pointer to a specific sprite, given tileset tile id.
	 */
	SDL_Rect* getSprite(int index);
private:
	/**
	 * To hold clipping data for each individual tile in the tilesheet image
	 */
	SDL_Rect sprite[256];
	
	int tiles_across, tiles_down;
};

#endif
