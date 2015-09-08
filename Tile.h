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

#ifndef __TILE_H__
#define __TILE_H__

class Tile
{
public:
	Tile();
	~Tile();
	
	/**
	 * Returns the tile id. Useful to determine which part of the tileset to extract.
	 */
	int getId() const;
	/**
	 * Returns whether the tile is walkable
	 */
	bool isWalkable() const;
	
	void setId(int newId);
	void setWalkable(bool canWalk);
	
	void incrementId();
	void decrementId();
private:
	int id;
	
	bool walkable;
	// you could: check whether each tile is walkable with this
	// or let the tileset define which tile ids are walkable
	// we will use the first approach
};

#endif
