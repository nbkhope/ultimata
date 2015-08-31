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

#include "Creature.h"

Creature::Creature()
{
	pos.w = 48;
	pos.h = 48;
	
	// Right at the center
	pos.x = SCREEN_WIDTH / 2 - (SCREEN_WIDTH / 2) % 48;
	pos.y = SCREEN_HEIGHT / 2 - (SCREEN_HEIGHT/ 2) % 48;
}

Creature::~Creature()
{
	//
}

int Creature::getPosX() const
{
	return pos.x;
}

int Creature::getPosY() const
{
	return pos.y;
}

int Creature::getWidth() const
{
	return pos.w;
}

int Creature::getHeight() const
{
	return pos.h;
}

void Creature::setPosX(int x)
{
	pos.x = x;
}

void Creature::setPosY(int y)
{
	pos.y = y;
}
