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

#include "GameMap.h"

GameMap::GameMap()
{
	cout << "Constructing GameMap object. . ." << endl;
	width = SCREEN_WIDTH;
	height = SCREEN_HEIGHT;
	cout << "GameMap dimensions: " << width << " by " << height << endl;
	
	tiles_across = SCREEN_WIDTH / TILESIZE;
	tiles_down = SCREEN_HEIGHT / TILESIZE;
	
	cout << "Allocating tiles . . ." << endl;
	tiles = new Tile[tiles_across * tiles_down];
	
	
	cout << tiles_across * tiles_down << " tiles allocated." << endl;
}

GameMap::~GameMap()
{
	cout << "Deconstructing GameMap object. . ." << endl;
	cout << "Deallocating tiles . . ." << endl;
	delete [] tiles;
	cout << tiles_across * tiles_down << " tiles deallocated." << endl;
}

int GameMap::getWidth() const
{
	return width;
}

int GameMap::getHeight() const
{
	return height;
}

int GameMap::getTilesAcross() const
{
	return tiles_across;
}

int GameMap::getTilesDown() const
{
	return tiles_down;
}

void GameMap::setWidth(int w)
{
	width = w;
}

void GameMap::setHeight(int h)
{
	height = h;
}
