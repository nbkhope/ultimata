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


#ifndef __CURSOR_H__
#define __CURSOR_H__

#include "SDL2/SDL.h"

#include "Constants.h"
#include "GameMap.h"

class Cursor
{
public:
	Cursor();
	~Cursor();
	
	void updateTileInfo(GameMap* gameMap);
	
	/**
	 * To get the cursor position
	 */
	int getPosX() const;
	int getPosY() const;
	
	/**
	 * To get the cursor area dimensions
	 */
	int getWidth() const;
	int getHeight() const;
	
	/**
	 * To get the position index of the tile within the cursor area.
	 */
	int getCursorTileIndex() const;
	/**
	 * To get the id of the tile within the cursor area.
	 * The tile id is used to determine which tile in the tileset to use.
	 */
	int getCursorTileId() const;
	
	/**
	 * Sets the cursor position
	 */
	void setPosX(int x);
	void setPosY(int y);
	
	/**
	 * Sets the cursor tile position index value.
	 */
	void setCursorTileIndex(int index);
	/**
	 * Sets the cursor tile id value
	 */
	void setCursorTileId(int index);
private:
	/**
	 * Rectangle structure to hold the position and area of the cursor.
	 */
	SDL_Rect pos;
	
	int cursorTileIndex;
	int cursorTileId;
};

#endif
