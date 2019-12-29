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
}

Tileset::~Tileset()
{
	//
}

SDL_Rect* Tileset::getSprite(int index)
{
	return &sprite[index];
}
