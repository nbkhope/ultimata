//	Copyright (C) 2010-2011, 2015  nbkhope
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

#include "Tileset.h"

Tileset::Tileset()
{
	int i, j;
	int index;
	
	cout << "Constructing tileset . . ."  << endl;

	tiles_across = 16;
	tiles_down = 16;
	
	// Set up sprite regions
	for (i = 0; i < tiles_down; i++)
		for (j = 0; j < tiles_across; j++)
		{
			index = i * tiles_across + j;
			sprite[index].w = sprite[index].h = TILESIZE;
			sprite[index].x = j * TILESIZE;
			sprite[index].y = i * TILESIZE;
		}
	
	cout << "Tileset constructed." << endl;
}

Tileset::~Tileset()
{
	//
}

SDL_Rect* Tileset::getSprite(int index)
{
	return &sprite[index];
}
