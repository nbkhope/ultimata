#ifndef _ITEM_H_
#define _ITEM_H_

#include <fstream>
#include <string>

#include <SDL/SDL.h>

#include "sdl.h"
#include "constants.h"

//#include "Tile.h"

using namespace std;

class Item
{
public:
	Item();
	~Item();
	Item(const Item& item);

	int getId() const;

	void setId(int newId);

//	Tile& getTile();
	ItemType getType() const;

//	SDL_Surface* getItemset();

	bool loadDescriptionsTxt(string filename);
private:
	int id;
	ItemType type;

//	Tile tile; // Item representation as a tile

//	SDL_Surface* itemset;
};

#endif
