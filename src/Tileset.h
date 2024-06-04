/** @file
 */

#ifndef __TILESET_H__
#define __TILESET_H__

#include <iostream>
#include <fstream>

#include "SDL2/SDL.h"
#include "sdl.h"

#include "Tile.h"

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

	bool loadTileDescriptionsTxt(string filename);

	SDL_Surface* getSheet() const;
	const Tile& getTile(int id) const;
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

	/**
	 * Surface to hold the image of the tileset sheet
	 */
	SDL_Surface* sheet;

	// implement this later
	//int tilesize;

	/**
	 * Holds the information for each tile in the set,
	 * i.e. each tile offset clip.
	 */
	Tile tiles[TILESET_TILES];
};

#endif
