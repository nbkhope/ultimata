#include "Tile.h"

Tile::Tile()
{
	id = 17;
	//id = 23;
	walkable = true;
	
	// Initialize dimension directly (no heap allocation needed)
	dimension.x = 0;
	dimension.y = 0;
	dimension.w = TILESIZE;
	dimension.h = TILESIZE;
	
	type = WALKABLE;
}

Tile::Tile(int x, int y, int length) : id(8)//, startx(x), starty(y), size(length)
{
	walkable = true;        // Initialize walkable for consistency
	
	dimension.x = x;
	dimension.y = y;
	dimension.w = length;
	dimension.h = length;

	type = WALKABLE;

	// items = new stack<Item>;
}

Tile::~Tile()
{
	// No manual cleanup needed for value types
	// delete items;
}

int Tile::getId() const
{
	return id;
}


bool Tile::isWalkable() const
{
	return walkable;
}


void Tile::setId(int newId)
{
	id = newId;
}

void Tile::setWalkable(bool canWalk)
{
	walkable = canWalk;
}

void Tile::incrementId()
{
	id++;
}


void Tile::decrementId()
{
	id--;
}

const SDL_Rect* Tile::getDimension() const
{
	return &dimension;
}

void Tile::setDimension(int px, int py, int width, int height)
{
	dimension.x = px;
	dimension.y = py;
	dimension.w = width;
	dimension.h = height;
}

int Tile::getType() const
{
	return type;
}

void Tile::setType(int newType)
{
	type = newType;
}

// stack<Item>* Tile::getStack()
// {
// 	return items;
// }

