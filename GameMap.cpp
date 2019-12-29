#include "GameMap.h"

GameMap::GameMap()
{
	cout << "Constructing GameMap object. . ." << endl;
	width = SCREEN_WIDTH;
	height = SCREEN_HEIGHT;
	cout << "GameMap dimensions: " << width << " by " << height << endl;
	
	tiles_across = SCREEN_WIDTH / TILESIZE;
	tiles_down = SCREEN_HEIGHT / TILESIZE;
	
	cout << "Allocating tiles . . ." << endl;
	tiles = new Tile[tiles_across * tiles_down];
	
	
	cout << tiles_across * tiles_down << " tiles allocated." << endl;
}

GameMap::~GameMap()
{
	cout << "Deconstructing GameMap object. . ." << endl;
	cout << "Deallocating tiles . . ." << endl;
	delete [] tiles;
	cout << tiles_across * tiles_down << " tiles deallocated." << endl;
}

int GameMap::getWidth() const
{
	return width;
}

int GameMap::getHeight() const
{
	return height;
}

int GameMap::getTilesAcross() const
{
	return tiles_across;
}

int GameMap::getTilesDown() const
{
	return tiles_down;
}

Tileset* GameMap::getTileset()
{
	return &tileset;
}

Tile* GameMap::getTile(int index)
{
	return &tiles[index];
}

void GameMap::setWidth(int w)
{
	width = w;
}

void GameMap::setHeight(int h)
{
	height = h;
}

bool GameMap::loadTxt(string filename)
{
	ifstream ifs;
	int total_tiles;
	int tileId;
	
	cout << "Opening " << filename << " to load game map. . ." << endl;
	
	ifs.open(filename.c_str());
	
	if (ifs.is_open())
	{
		// Get map dimensions
		ifs >> width;
		ifs >> height;
		
		total_tiles = tiles_across * tiles_down;
		
		for (int i = 0; i < total_tiles; i++)
		{
			ifs >> tileId;
			tiles[i].setId(tileId);
		}
		
		ifs.close(); // watch out here
		
		cout << "Map loaded." << endl;
	}
	else
	{
		cerr << "Could not load game map file" << endl;
		return false;
	}
	
	return true;
}

bool GameMap::saveTxt(string filename)
{
	ofstream ofs;
	int total_tiles;
	int tileId;
	
	cout << "Opening " << filename << " to save game map. . ." << endl;
	
	if (fileExists(filename))
		cout << "Warning: map file already exists and will be overwritten" << endl;
	
	ofs.open(filename.c_str());
	
	if (ofs.is_open())
	{
		// Save map dimensions
		ofs << width << " ";
		ofs << height << endl;
		
		// Note: ofs << does not add any spaces whatsoever
		
		total_tiles = tiles_across * tiles_down;
		
		for (int i = 0; i < tiles_down; i++)
			for (int j = 0; j < tiles_across; j++)
			{
				tileId = tiles[i * tiles_across + j].getId();
				ofs << tileId;
				if (j == tiles_across - 1) // last one in a row, add a new line to file
					ofs << "\n";
				else
					ofs << " ";
			}
		
		ofs.close(); // watch out here
		
		cout << "Map saved." << endl;
	}
	else
	{
		cerr << "Could not save game map file" << endl;
		return false;
	}
	
	return true;
}
