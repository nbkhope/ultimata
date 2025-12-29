#include "Tileset.h"

Tileset::Tileset()
{
	int i, j;
	int index;
	
	cout << "Constructing tileset . . ."  << endl;

	tiles_across = 16;
	tiles_down = 16;
	sheet = NULL;  // Initialize to prevent undefined behavior
	
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
	// Free the sheet surface if it was allocated
	if (sheet != NULL)
	{
		SDL_FreeSurface(sheet);
	}
}

SDL_Rect* Tileset::getSprite(int index)
{
	// Bounds checking to prevent out-of-bounds access
	if (index >= 0 && index < 256)
	{
		return &sprite[index];
	}
	else
	{
		// Log the error for debugging
		cerr << "Warning: getSprite() called with invalid index " << index 
		     << " (valid range: 0-255)" << endl;
		
		// Return the first sprite as a safe fallback
		return &sprite[0];
	}
}

bool Tileset::loadTileDescriptionsTxt(const string& filename)
{
	ifstream input;

	int id, type;

	input.open(filename.c_str());

	if (input.is_open())
	{
		while (input >> id && input >> type)
		{
			// Validate id is within valid range
			if (id >= 0 && id < TILESET_TILES)
			{
				tiles[id].setType(type);
			}
			else
			{
				cerr << "Warning: Invalid tile id " << id << " in " << filename 
				     << " (valid range: 0-" << (TILESET_TILES-1) << ")" << endl;
			}
		}
		input.close();
		return true;	
	}

	return false;
}

// cant return reference, why? use const before tile
const Tile& Tileset::getTile(int id) const
{
	// Bounds checking to prevent out-of-bounds access
	if (id >= 0 && id < TILESET_TILES)
	{
		return tiles[id];
	}
	else
	{
		// Log the error for debugging
		cerr << "Warning: getTile() called with invalid id " << id 
		     << " (valid range: 0-" << (TILESET_TILES-1) << ")" << endl;
		
		// Return the first tile as a safe fallback
		return tiles[0];
	}
}

