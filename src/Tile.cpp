#include "Tile.h"

Tile::Tile()
{
	id = 17;
	//id = 23;
	walkable = true;  // Primary walkability mechanism
	
	// Initialize dimension directly (no heap allocation needed)
	dimension.x = 0;
	dimension.y = 0;
	dimension.w = TILESIZE;
	dimension.h = TILESIZE;
	
	type = WALKABLE;  // Legacy field, kept for compatibility
}

Tile::Tile(int dimX, int dimY, int dimSize) : id(8)
{
	walkable = true;  // Primary walkability mechanism
	
	dimension.x = dimX;
	dimension.y = dimY;
	dimension.w = dimSize;
	dimension.h = dimSize;

	type = WALKABLE;  // Legacy field, kept for compatibility

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
	// Note: type field is maintained separately and not used for walkability
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
	// Validate type parameter against TileType enum values
	if (newType >= WALKABLE && newType <= WATER)
	{
		type = newType;
	}
	else
	{
		cerr << "Warning: Invalid tile type " << newType 
		     << ". Valid types are: WALKABLE(" << WALKABLE 
		     << "), NONWALKABLE(" << NONWALKABLE 
		     << "), WATER(" << WATER << ")" << endl;
		// Set to default safe value
		type = WALKABLE;
	}
}

// stack<Item>* Tile::getStack()
// {
// 	return items;
// }

