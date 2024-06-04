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

	sheet = loadImage("data/graphics/tileset.png");

	for (int i = 0; i < TILESET_TILES; i++)
		tiles[i].setDimension(i % 8 * TILESIZE, i / 8 * TILESIZE);
	loadTileDescriptionsTxt("data/tileset_description.txt");
}

Tileset::~Tileset()
{
	SDL_FreeSurface(sheet);
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

SDL_Surface* Tileset::getSheet() const
{
	return sheet;
}

// cant return reference, why? use const before tile
const Tile& Tileset::getTile(int id) const
{
	return tiles[id];
}

