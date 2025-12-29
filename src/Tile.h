/** @file
 */

#ifndef __TILE_H__
#define __TILE_H__

/**
 * A tile object represents a map tile. It has information 
 * about the tile id and whether the tile is walkable. The 
 * Tile object has nothing to do with tile images.
 */
#include <stack>
#include <SDL2/SDL.h>


#include "Constants.h"

using namespace std;

class Tile
{
public:
	Tile();
	Tile(int x, int y, int length);

	~Tile();

	const SDL_Rect* getDimension() const;
	void setDimension(int px, int py, int width = TILESIZE, int height = TILESIZE);

	/**
	 * @return the tile id. Useful to determine which part of the tileset to extract.
	 */
	int getId() const;
	/**
	 * Returns whether the tile is walkable
	 */
	bool isWalkable() const;
	
	void setId(int newId);
	void setWalkable(bool canWalk);
	
	void incrementId();
	void decrementId();

	int getType() const;

	void setType(int newType);

	/* These might be a problem the way they are now
	void addItem(Item& newItem);
	void removeItem(Item& removedItem = NULL);
	Item& getTopItem();
	*/

	// stack<Item>* getStack();
	
private:
	/**
	 * The identification of the tile will serve both to get its textual descriptions but also
	 * to determine its clipping position in the tileset. Note the latter could be done using
	 * SDL_Rect dimension. For now, we assume a TILESIZExTILESIZE tile.
	 */ 
	int id;
	
	bool walkable;
	// you could: check whether each tile is walkable with this
	// or let the tileset define which tile ids are walkable
	// we will use the first approach

	//int startx;
	//int starty;

	/**
	 * Each tile needs the clipping position and a size.
	 * The tileset will be clipped according to the coordinates x and y of the rectangle its width and height.
	 */
	SDL_Rect dimension;
	//int size;

	// Collision
	/**
	 * In essence, determines whether the tile is walkable
	 */
	int type;

	/**
	 * Each tile/floor will have a stack of items on it
	 */
	// stack<Item>* items;
	  
};

#endif
