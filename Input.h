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

#ifndef __INPUT_H__
#define __INPUT_H__

#include <iostream>

#include "SDL2/SDL.h"

#include "Constants.h"
#include "Creature.h"
#include "Cursor.h"
#include "GameMap.h"
//#include "Graphics.h" // this is a problem
// Input should not deal with Graphics, but Graphics should take an input object as an argument

using namespace std;

class Input
{
public:
	Input();
	~Input();
	
	/**
	 * Handles input from the player
	 */
	bool get(GameMap* gameMap, Creature *c);
	
	/**
	 * Handles input from the player (test version)
	 */
	bool testGet(GameMap* gameMap, Creature *c);
	
	// Alpha blending <~test~>
	Uint8 getAlpha() const;
	void setAlpha(Uint8 a);
	Uint8* getColor();
	void setColor(Uint8* color);
	
	// Change tile <~test~>
	int getTestTileId() const;
	
	// Image Loading Different Types <~test~>
	int getTestImageLoad() const;
	
	/**
	 * Returns a pointer to the cursor
	 */
	Cursor* getCursor();
	
private:
	/**
	 * Structure to hold input events
	 */
	SDL_Event e;
	
	// Alpha blending <~test~>
	Uint8 alpha;
	Uint8 color[3];
	
	// Change tile <~test~>
	int testTileId;
	
	// Image Loading Different Types <~test~>
	int testImageLoad;
	
	/**
	 * The screen cursor
	 */
	Cursor cursor; // to select a certain tile and eventually change it with [, ]
};

#endif
