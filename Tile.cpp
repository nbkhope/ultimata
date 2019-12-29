#include "Tile.h"

Tile::Tile()
{
	id = 17;
	//id = 23;
	walkable = true;
}

Tile::~Tile()
{
	
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