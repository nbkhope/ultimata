#ifndef _MAP_H_
#define _MAP_H_

#include <iostream>
#include <string>
#include <sstream>
#include <stack>
#include <fstream>
#include <list>

#include <SDL/SDL.h>
//#include <SDL/SDL_rotozoom.h>

#include "sdl.h"

#include "Creature.h"
#include "Monster.h"
#include "Tile.h"
#include "Tileset.h"

using namespace std;


class Map
{
public:
	/**
	 * Constructors
	 */
	Map();
	Map(int w, int h, int tsize);

	/**
	 * Copy constructor
	 */
	Map(const Map& map);

	/**
	 * Destructor
	 */
	~Map();

	// use Graphics later on
	//void paintSurfaceArea(SDL_Rect* area, SDL_Surface* surf);

	/**
	 * Redraws the whole map surface
	 */
	//void paintSurface();
	void paintMonsters();


	void applyTile(const Tile &tile, int x, int y);
	void applyRandomTileRandomPosition();
	void fill(const Tile &tile);
	//void draw();
	bool loadTxt(string filename);
	//void loadTxtWithItemStack(string filename);
    void loadTxt2(string filename);

	// Acessor functions to obtain the object's attributes.
	// Note a const is used at the end of the function header to indicate
	// the it cannot not change any of the object's attributes.
	int getWidth() const;
	int getHeight() const;
	//int getTilesize() const;
	
	int getTilesAcross() const;
	int getTilesDown() const;

	SDL_Surface* getSurface() const;
	SDL_Surface* getMiniMapSurface() const;

	Tileset* getTileset();

	//Tile* getTiles() { return tiles; }
	
	int getTileId(int index);

	void setWidth(int w);
	void setHeight(int h);
	//int setTilesize(int size);
	
	void setTileId(int index, int id);

	void updateCreatureCamera(Creature& creat);
	void randomPositionCreature(Creature& creat);
	void randomPositionCreature();

	bool placeCreature(Creature* c, const SDL_Rect& pos);
	bool removeCreature(Creature* c);
	void addCreature(Creature* c);
	void moveCreature(Creature* creature);

	bool hasCollided(Creature* c, const Tile& tile);
	
	// New Functions
	int load(string filename, string format = "txt");
	int paintTile(int tileId, int pos_x, int pos_y);

private:
	int width;
	int height;

    // very inefficient; disable for now
	//Tile* tiles;
	
	Tile* tiles;
	
	int tiles_across;
	int tiles_down;

	// Do I need this if I have a tileset?
	//int tilesize;

	Tileset tileset;

	SDL_Surface* itemset;
	SDL_Surface* monstersprites;
	list<Creature*>* creatures;

	// Map should not be dealing with surfaces
	// This should be the job of the Graphics class
	// Map only keeps track internally of the tiles
	SDL_Surface* surface;
	SDL_Surface* minimapsurface;
};

#endif
