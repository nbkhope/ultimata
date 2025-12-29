/** @file
 */

#ifndef __INPUT_H__
#define __INPUT_H__

#include <string>
// #include <sstream>
#include <iostream>

#include "SDL2/SDL.h"

#include "Constants.h"
#include "Creature.h"
#include "Cursor.h"
#include "GameMap.h"
#include "Timer.h"
//#include "Graphics.h" // this is a problem
// Input should not deal with Graphics, but Graphics should take an input object as an argument

using namespace std;

/**
 * An Input object is responsible for handling player
 * input and change the game parameters accordingly.
 */
class Input
{
public:
	Input();
	~Input();
	
	/**
	 * Handles input from the player
	 */
	bool get(GameMap* gameMap, Creature *c);
	
	/**
	 * Handles input from the player (test version)
	 */
	bool testGet(GameMap* gameMap, Creature *c);
	
	// Alpha blending <~test~>
	Uint8 getAlpha() const;
	void setAlpha(Uint8 a);
	Uint8* getColor();
	void setColor(Uint8* color);
	
	// Change tile <~test~>
	int getTestTileId() const;
	
	// Image Loading Different Types <~test~>
	int getTestImageLoad() const;
	
	/**
	 * @return	a pointer to the cursor
	 */
	Cursor* getCursor();
	
	Timer* getTimer();
	
	bool getDialogueToggle() const;
	void setDialogueToggle(bool toggle);
	
	void checkPlayerMovement(GameMap* gameMap, Creature* c);
	//void movePlayer(Creature *c);

	// void get();

private:
	/**
	 * Structure to hold input events
	 */
	SDL_Event e;
	
	// Alpha blending <~test~>
	Uint8 alpha;
	Uint8 color[3];
	
	// Change tile <~test~>
	int testTileId;
	
	// Image Loading Different Types <~test~>
	int testImageLoad;
	
	// Dialogue system
	bool dialogueToggle;
	
	/**
	 * The screen cursor
	 */
	Cursor cursor; // to select a certain tile and eventually change it with [, ]
	
	Timer timer;

	// stringstream stream;
};

#endif
