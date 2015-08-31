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
	
	name = "Unknown";
	health = healthMax = 120;
	mana = manaMax = 40;
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

string Creature::getName() const
{
	return name;
}
/*
uint32_t Creature::getLevel() const
{
	return level;
}

uint64_t Creature::getExperience() const
{
	return experience;
}
*/
int Creature::getHealth() const
{
	return health;
}

int Creature::getHealthMax() const
{
	return healthMax;
}

int Creature::getMana() const
{
	return mana;
}

int Creature::getManaMax() const
{
	return manaMax;
}

void Creature::setHealth(int newHealth)
{
	health = newHealth;
}
/*
void Creature::setLevel(int newLevel)
{
	level = newLevel;
}

void Creature::setExperience(int newExperience)
{
	experience = newExperience;
}
*/

void Creature::setHealthMax(int newHealthMax)
{
	healthMax = newHealthMax;
}

void Creature::setMana(int newMana)
{
	mana = newMana;
}

void Creature::setManaMax(int newManaMax)
{
	manaMax = newManaMax;
}
