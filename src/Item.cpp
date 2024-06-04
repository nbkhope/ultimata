#include "Item.h"

Item::Item() : id(0), type(UNUSABLE)
{
	//every item is gonna load this... very inefficient.. that's why we need an itemset class or put this in the map private section
	//itemset = loadImage("data/graphics/itemset.png");

	
}

Item::~Item()
{
//	SDL_FreeSurface(itemset);
}

Item::Item(const Item& item)
{
	id = item.getId();
	type = item.getType();
}

int Item::getId() const
{
	return id;
}

void Item::setId(int newId)
{
	id = newId;
}

//Tile& getTile();

ItemType Item::getType() const
{
	return type;
}

/*SDL_Surface* Item::getItemset()
{
	return itemset;
}*/

bool Item::loadDescriptionsTxt(string filename)
{
	ifstream file;

	return true;
}
