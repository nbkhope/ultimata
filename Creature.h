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

#include <iostream>

#include "SDL2/SDL.h"

#include "Constants.h"

using namespace std;

class Creature
{
public:
	Creature();
	~Creature();
	
	/**
	 * Returns the position of the creature
	 */
	int getPosX() const;
	int getPosY() const;
	/**
	 * Returns the dimensions of the creature
	 */
	int getWidth() const;
	int getHeight() const;
	
	void setPosX(int x);
	void setPosY(int y);
	
	/**
	 * Returns the name of the creature
	 */
	string getName() const;
	
	//uint32_t getLevel() const;
	
	//uint64_t getExperience() const;
	
	/**
	 * Returns the current health points of the creature
	 */
	int getHealth() const;
	
	/**
	 * Returns the maximum health points of the creture
	 */
	int getHealthMax() const;
	
	/**
	 * Returns the current mana points of the creature
	 */
	int getMana() const;
	
	/**
	 * Returns the maximum mana points of the creture
	 */
	int getManaMax() const;
	
	//void setLevel(int newLevel);
	//void setExperience(int newExperience);
	void setHealth(int newHealth);
	void setHealthMax(int newHealthMax);
	void setMana(int newMana);
	void setManaMax(int newManaMax);
private:
	SDL_Rect pos;
	//int pos_x, pos, y;
	
	string name;
	int health;
	int healthMax;
	int mana;
	int manaMax;
};

#endif
