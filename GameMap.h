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

#ifndef __GAMEMAP_H__
#define __GAMEMAP_H__

#include <iostream>
#include <fstream>

#include "Constants.h"
#include "Tile.h"
#include "Tileset.h"

#include "HelperFunctions.h"

using namespace std;

class GameMap
{
public:
	GameMap();
	~GameMap();
	
	int getWidth() const;
	int getHeight() const;
	
	int getTilesAcross() const;
	int getTilesDown() const;
	
	/**
	 * Returns a pointer to the tileset object.
	 */
	Tileset* getTileset();
	
	/**
	 * Returns a pointer to a specific map tile
	 * Each tile in the map has a position index, which is determined by counting
	 * the tile from left to right, top to bottom.
	 */
	Tile* getTile(int index);
	
	void setWidth(int w);
	void setHeight(int h);
	
	/**
	 * Loads map from a text file
	 */
	bool loadTxt(string filename);
	/**
	 * Saves map to a text file
	 */
	bool saveTxt(string filename);
private:
	int width;
	int height;
	
	int tiles_across;
	int tiles_down;
	
	/**
	 * To keep track of each tile in the map.
	 * Constructor will handle dynamic allocation of tiles
	 * while Destructor will handle deallocation.
	 */
	Tile* tiles;
	
	/**
	 * The tileset for the map. This is not the tilesheet image; it keeps track
	 * of tileset information.
	 */
	Tileset tileset;
	// tileset refers to the data; tileSheet refers to the texture (image file)
};

#endif
