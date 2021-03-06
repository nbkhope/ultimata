/** @file
 * Contains the declaration of the Creature class
 */

#ifndef __CREATURE_H__
#define __CREATURE_H__

#include <iostream>

#include "SDL2/SDL.h"

#include "Constants.h"

using namespace std;

/**
 * A creature is the base for all the other entities 
 * such as Player, Monster, and Npc (non-playable character)
 */
class Creature
{
public:
	Creature();
	~Creature();
	
	/**
	 * @return the position of the creature
	 */
	int getPosX() const;
	int getPosY() const;
	void getPos(int &x, int &y) const;
	
	/**
	 * @return the dimensions of the creature
	 */
	int getWidth() const;
	int getHeight() const;
	
	/**
	 * @return the creature speed
	 */
	int getSpeedX() const;
	int getSpeedY() const;
	void getSpeed(int &xs, int &ys);
	
	/**
	 * @return the creature's current step
	 */
	int getStep() const;
	/**
	 * @return the direction the creature is facing
	 */
	int getDirection() const;
	bool getNewDirection() const;
	
	/**
	 * Sets the creature position
	 */
	void setPosX(int x);
	void setPosY(int y);
	void setPos(int x, int y);
	
	/**
	 * Sets the creature speed (for movement)
	 */
	void setSpeedX(int newSpeed);
	void setSpeedY(int newSpeed);
	void setSpeed(int newSpeedX, int newSpeedY);
	
	void setStep(int newStep);
	void setDirection(int newDirection);
	void setNewDirection (bool newNewDirection);
	
	/**
	 * Shifts creature position by the given amount.
	 */
	void shiftPosX(int xshift);
	void shiftPosY(int yshift);
	/**
	 * Shifts the creature step (for animation purposes)
	 * Step will be brought back to 0 after a certain number of shifts
	 */
	void shiftStep();
	
	/**
	 * @return the name of the creature
	 */
	string getName() const;
	
	/*
	 * @return the level of the creature
	 */
	uint32_t getLevel() const;
	
	/*
	 * Returns how much experience the creature has.
	 * Perhaps this could also be used, in the case of monsters, 
	 * to be the experienced gained from killing them.
	 */
	uint64_t getExperience() const;
	
	/**
	 * @return the current health points of the creature
	 */
	int getHealth() const;
	
	/**
	 * @return the maximum health points of the creture
	 */
	int getHealthMax() const;
	
	/**
	 * @return the current mana points of the creature
	 */
	int getMana() const;
	
	/**
	 * @return the maximum mana points of the creture
	 */
	int getManaMax() const;
	
	/*
	 * Functions to set the attributes of the creature
	 */
	void setLevel(int newLevel);
	void setExperience(int newExperience);
	void setHealth(int newHealth);
	void setHealthMax(int newHealthMax);
	void setMana(int newMana);
	void setManaMax(int newManaMax);
private:
	/*
	 * Rectangle structure to hold creature position and area
	 */
	SDL_Rect pos;
	//int pos_x, pos, y;
	
	/**
	 * To keep track of the creature movement
	 */
	int xspeed, yspeed;
	/**
	 * For the animation. There are four states for the character image.
	 * As the creature moves, the steps shift gradually from 0 to 3 and then
	 * back to 0 again.
	 */
	int step;
	/**
	 * Keeps track of the direction the creature is facing
	 */
	int direction;
	bool newDirection;
	
	string name;
	
	uint32_t level;
	uint64_t experience;
	
	int health;
	int healthMax;
	int mana;
	int manaMax;
};

#endif
