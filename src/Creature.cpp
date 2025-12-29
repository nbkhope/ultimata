#include "Creature.h"
#include <iostream>

using namespace std;

Creature::Creature() 
{
	init();
	name = "Unknown";  // Set default name

	// 	attack = 10;
	// 	defense = 4;
	// 	frame = 0;

	// 	camera.x = 0;
	// 	camera.y = 0;
	// 	camera.w = SCREEN_WIDTH;
	// 	camera.h = SCREEN_HEIGHT;

	// //	charset = NULL;
	// 	charset = loadImage("data/graphics/charset2.png");
	// /*
	// 	// Set up the clipping
	// 	for (int j = 0; j < 4; j++)
	// 	{
	// 		for (int i = 0; i < 3; i++)
	// 		{
	// 			charsetclip[j][i].x = i * 32;
	// 			charsetclip[j][i].y = j * 64;
	// 			charsetclip[j][i].w = 32;
	// 			charsetclip[j][i].h = 64;
	// 		}
	// 	}*/

	// 	id = 4;
}

Creature::Creature(string name)
{
	init();
	this->name = name;  // Set the provided name
}

/**
 * Common initialization for all constructors
 * This eliminates code duplication and ensures consistent initialization
 */
void Creature::init()
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
	level = 1;
	experience = 0;
	health = healthMax = 120;
	mana = manaMax = 40;
	
	/**
	 * Graphics attributes
	 */
	charset = NULL;  // Initialize to prevent crash in destructor
}

Creature::~Creature()
{
	// Only free if charset was actually allocated
	if (charset != NULL)
	{
		SDL_FreeSurface(charset);
	}
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
/*
SDL_Rect Creature::getPosition() const
{
	return pos;
}
*/


///////////////////////////

void Creature::shiftPos(int sx, int sy)
{
	pos.x += sx;
	pos.y += sy;
}

////////////////////////////////////
/*
void Creature::setPosition(int px, int py)
{
	pos.x = px;
	pos.y = py;	
}*/


SDL_Surface* Creature::getCharset() const
{
	return charset;
}

// Camera is now part of the base class Creature instead of derived class Player
SDL_Rect* Creature::getCamera()// const
{
	return &camera;
}

/* This function had to be moved to the map class due to conflicting headers files when trying to add the monster class and
   list of creatures in the map class

void Creature::updateCamera(Map &map)//, SDL_Rect &camera)
{
	// Ceneter camera around player
//	camera.x = (pos.x + pos.w)/2 - SCREEN_WIDTH/2;
//	camera.y = (pos.y + pos.h)/2 - SCREEN_HEIGHT/2;

	camera.x = (pos.x + pos.w/2) - camera.w/2;
	camera.y = (pos.y + pos.h/2) - camera.h/2;

	if (camera.x < 0)
		camera.x = 0;

	// OMG! All the trouble because of a unseen semicolor right after this
	// if statement condition...
	if (camera.y < 0)
		camera.y = 0;

	if (camera.x > (map.getWidth() - camera.w))
		camera.x = (map.getWidth() - camera.w);

	if (camera.y > (map.getHeight() - camera.h))
		camera.y = (map.getHeight() - camera.h);

// Problematic: wrong..
/*	if (camera.x > (pos.x + pos.w/2) - camera.w/2)
		camera.x = map.getWidth() - camera.w;

	if (camera.y > (pos.y + pos.h/2) - camera.h/2)
		camera.y = (map.getHeight() - camera.h);
}
*/

