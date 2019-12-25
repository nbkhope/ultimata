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

#include "Creature.h"

Creature::Creature()
{
	/**
	 * Position attributes
	 */
	pos.w = TILESIZE;
	pos.h = TILESIZE*2;
	
	// Right at the center
	pos.x = SCREEN_WIDTH / 2 - (SCREEN_WIDTH / 2) % TILESIZE;
	pos.y = SCREEN_HEIGHT / 2 - (SCREEN_HEIGHT/ 2) % TILESIZE;
	
	/**
	 * Movement attributes
	 */
	xspeed = 0;
	yspeed = 0;
	step = 0;
	direction = DIRECTION_SOUTH;
	newDirection = false;
	
	/**
	 * Stats attributes
	 */
	name = "Unknown";
	level = 1;
	experience = 0;
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

void Creature::getPos(int &x, int &y) const
{
	x = pos.x;
	y = pos.y;
}

int Creature::getWidth() const
{
	return pos.w;
}

int Creature::getHeight() const
{
	return pos.h;
}

int Creature::getSpeedX() const
{
	return xspeed;
}

int Creature::getSpeedY() const
{
	return yspeed;
}

void Creature::getSpeed(int &xs, int &ys)
{
	xs = xspeed;
	ys = yspeed;
}

int Creature::getStep() const
{
	return step;
}

int Creature::getDirection() const
{
	return direction;
}

bool Creature::getNewDirection() const
{
	return newDirection;
}

void Creature::setPosX(int x)
{
	pos.x = x;
}

void Creature::setPosY(int y)
{
	pos.y = y;
}

void Creature::setPos(int x, int y)
{
	pos.x = x;
	pos.y = y;
}

void Creature::setSpeedX(int newSpeed)
{
	xspeed = newSpeed;
}

void Creature::setSpeedY(int newSpeed)
{
	yspeed = newSpeed;
}

void Creature::setSpeed(int newSpeedX, int newSpeedY)
{
	xspeed = newSpeedX;
	yspeed = newSpeedY;
}

void Creature::setStep(int newStep)
{
	step = newStep;
}

void Creature::setDirection(int newDirection)
{
	direction = newDirection;
}

void Creature::setNewDirection(bool newNewDirection)
{
	newDirection = newNewDirection;
}

void Creature::shiftPosX(int xshift)
{
	pos.x += xshift;
}

void Creature::shiftPosY(int yshift)
{
	pos.y += yshift;
}

void Creature::shiftStep()
{
	step = (step + 1) % 4;
}

string Creature::getName() const
{
	return name;
}

uint32_t Creature::getLevel() const
{
	return level;
}

uint64_t Creature::getExperience() const
{
	return experience;
}

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

void Creature::setLevel(int newLevel)
{
	level = newLevel;
}

void Creature::setExperience(int newExperience)
{
	experience = newExperience;
}


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
