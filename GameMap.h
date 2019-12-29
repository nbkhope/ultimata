/** @file
 */

#ifndef __GAMEMAP_H__
#define __GAMEMAP_H__

#include <iostream>
#include <fstream>

#include "Constants.h"
#include "Tile.h"
#include "Tileset.h"

#include "HelperFunctions.h"

using namespace std;

/**
 * Keeps track of the properties of the game map. 
 * This has nothing to do with graphics and images!
 *
 * Essentially, the GameMap object keeps track of each tile 
 * and has a tileset, which in turn is another object that
 * keeps track of tileset information
 * (again, nothing to do with graphics or images)
 */
class GameMap
{
public:
	GameMap();
	~GameMap();
	
	/**
	 * @return the map dimensions
	 */
	int getWidth() const;
	int getHeight() const;
	
	int getTilesAcross() const;
	int getTilesDown() const;
	
	/**
	 * @return a pointer to the tileset object.
	 */
	Tileset* getTileset();
	
	/**
	 * @return a pointer to a specific map tile
	 * Each tile in the map has a position index, which is determined by counting
	 * the tile from left to right, top to bottom.
	 */
	Tile* getTile(int index);
	
	/*
	 * Sets the map dimensions
	 */
	void setWidth(int w);
	void setHeight(int h);
	
	/**
	 * Loads map from a text file
	 *
	 * @param filename	path to the game map file
	 */
	bool loadTxt(string filename);
	/**
	 * Saves map to a text file
	 *
	 * @param filename	path to the game map file
	 */
	bool saveTxt(string filename);
private:
	/**
	 * The map dimensions
	 */
	int width;
	int height;
	
	int tiles_across;
	int tiles_down;
	
	/**
	 * To keep track of each tile in the map.
	 * Constructor will handle dynamic allocation of tiles
	 * while Destructor will handle deallocation.
	 */
	Tile* tiles;
	
	/**
	 * The tileset for the map. This is not the tilesheet image; it keeps track
	 * of tileset information.
	 */
	Tileset tileset;
	// tileset refers to the data; tileSheet refers to the texture (image file)
};

#endif
