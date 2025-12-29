#include "Tileset.h"

Tileset::Tileset()
{
	int i, j;
	int index;
	
	cout << "Constructing tileset . . ."  << endl;

	tiles_across = 16;
	tiles_down = 16;
	
	// Set up sprite regions
	for (i = 0; i < tiles_down; i++)
		for (j = 0; j < tiles_across; j++)
		{
			index = i * tiles_across + j;
			sprite[index].w = sprite[index].h = TILESIZE;
			sprite[index].x = j * TILESIZE;
			sprite[index].y = i * TILESIZE;
		}
	
	cout << "Tileset constructed." << endl;

	for (int i = 0; i < TILESET_TILES; i++)
		tiles[i].setDimension(i % 8 * TILESIZE, i / 8 * TILESIZE);
		
	if (!loadTileDescriptionsTxt("data/tileset_description.txt"))
	{
		cerr << "Warning: Failed to load tile descriptions from data/tileset_description.txt" << endl;
		cerr << "Tiles will use default type settings." << endl;
	}
}

Tileset::~Tileset()
{
}

SDL_Rect* Tileset::getSprite(int index)
{
	return &sprite[index];
}

bool Tileset::loadTileDescriptionsTxt(string filename)
{
	ifstream input;

	int id, type;

	input.open(filename.c_str());

	if (input.is_open())
	{
		while (input >> id && input >> type)
		{
			tiles[id].setType(type);
		}
		input.close();
		return true;	
	}

	return false;
}

// cant return reference, why? use const before tile
const Tile& Tileset::getTile(int id) const
{
	return tiles[id];
}

