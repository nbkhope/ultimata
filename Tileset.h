/** @file
 */

#ifndef __TILESET_H__
#define __TILESET_H__

#include <iostream>

#include "SDL2/SDL.h"

#include "Constants.h"

using namespace std;

/**
 * A Tileset object keeps information about a tilesheet, 
 * but has nothing to do with its rendering.
 */
class Tileset
{
public:
	Tileset();
	~Tileset();
	
	/**
	 * Returns a pointer to a specific sprite, given tileset tile id.
	 */
	SDL_Rect* getSprite(int index);
private:
	/**
	 * To hold clipping data for each individual tile in the tilesheet image
	 */
	SDL_Rect sprite[256];
	
	int tiles_across, tiles_down;
};

#endif
