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

#ifndef __CREATURE_H__
#define __CREATURE_H__

#include "SDL2/SDL.h"

#include "Constants.h"

class Creature
{
public:
	Creature();
	~Creature();
	
	int getPosX() const;
	int getPosY() const;
	int getWidth() const;
	int getHeight() const;
	
	void setPosX(int x);
	void setPosY(int y);
private:
	SDL_Rect pos;
	//int pos_x, pos, y;
};

#endif
