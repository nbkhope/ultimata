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

#include "Constants.h"
#include "Tile.h"

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
	
	void setWidth(int w);
	void setHeight(int h);
private:
	int width;
	int height;
	
	int tiles_across;
	int tiles_down;
	
	Tile* tiles;
};

#endif
