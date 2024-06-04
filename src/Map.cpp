#include "Map.h"

#include "constants.h"
#include <iostream>

// to do: change all TILESIZE to tileset.getTile(0).getDimension().w ?
//		  or is it too much overhead?

Map::Map() : width(DEFAULT_MAP_WIDTH), height(DEFAULT_MAP_HEIGHT)//, tilesize(TILESIZE)
{
	int total_tiles;
	
    cout << "Constructing Map . . ." << endl;
    
    // This is very inefficient, the frame rate drops considerably
    // Let's disable this for now... and think of a new way to keep track of tiles
    // and stacks of items
	//tiles = new Tile[width / TILESIZE * height / TILESIZE];
	
	tiles_across = width / TILESIZE;
	tiles_down = height / TILESIZE;
	total_tiles = tiles_across * tiles_down;
	
	tiles = new Tile[total_tiles];

	creatures = new list<Creature*>;

	// This is actually unnecessary, because we should assume the Tile
	// class constructor will set default values.
	for (int i = 0; i < total_tiles; i++)
		tiles[i].setId(0);

	itemset = loadImage("data/graphics/itemset.png");
	monstersprites = loadImage("data/graphics/monsters64.png");

	// Sets up the map surface
	surface = SDL_CreateRGBSurface(SDL_SWSURFACE, width, height, SCREEN_BPP, 0, 0, 0, 0);
	//surface = new SDL_Surface;
    
    if(surface == NULL) {
        cerr << "SDL_CreateRGBSurface(): " << SDL_GetError() << endl;
    }
    else
        cout << "SDL_CreateRGBSurface(): Success" << endl;


	SDL_FillRect(surface, &surface->clip_rect, SDL_MapRGB(surface->format, 0xFF, 0xFF, 0xFF));

	minimapsurface = SDL_CreateRGBSurface(SDL_SWSURFACE, MINIMAP_WIDTH, MINIMAP_HEIGHT, SCREEN_BPP, 0, 0, 0, 0);
	// this wont resize the img :). sdl by default does not have a resize fn, you hv
	// to make ur own or use someone's fn.
	//	minimapsurface = SDL_ConvertSurface(surface, minimapsurface->format, minimapsurface->flags);
	//minimapsurface = shrinkSurface(surface, width / MINIMAP_WIDTH, height / MINIMAP_HEIGHT);

/** experiment with list of creatures	

	Monster monster;
	Monster* monsterptr = new Monster;

	list<Creature>::iterator it = creatures->begin();
	
	//creatures.insert(it, creature);	creatures.insert(it, creature);	creatures.insert(it, creature);	creatures.insert(it, creature);
	creatures->insert(it, monster);
	creatures->insert(it, monster);
	creatures->insert(it, monster);
	creatures->insert(it, monster);
	creatures->insert(it, monster);
	creatures->insert(it, *monsterptr);

	randomPositionCreature();
**/
	//paintSurface();



}

Map::Map(int w, int h, int tsize) : width(w), height(h)//, tilesize(tsize)
{
    // very inefficient, disable for now
	//tiles = new Tile[w / TILESIZE * h / TILESIZE];

	creatures = new list<Creature*>;

	itemset = loadImage("data/graphics/itemset.png");
	monstersprites = loadImage("data/graphics/monsters64.png");

	surface = SDL_CreateRGBSurface(SDL_SWSURFACE, width, height, SCREEN_BPP, 0, 0, 0, 0);
	//surface = new SDL_Surface;
	SDL_FillRect(surface, &surface->clip_rect, SDL_MapRGB(surface->format, 0xFF, 0xFF, 0xFF));

	minimapsurface = SDL_CreateRGBSurface(SDL_SWSURFACE, MINIMAP_WIDTH, MINIMAP_HEIGHT, SCREEN_BPP, 0, 0, 0, 0);
	//minimapsurface = shrinkSurface(surface, width / MINIMAP_WIDTH, height / MINIMAP_HEIGHT);

	//paintSurface();
}

Map::Map(const Map& map)
{
	width = map.width;
	height = map.height;
	//tilesize = map.tilesize;

	// copy tiles
	//SDL_BlitSurface(map.surface, NULL, surface, NULL);
	surface = SDL_ConvertSurface(map.getSurface(), map.getSurface()->format, map.getSurface()->flags);
	//minimapsurface = shrinkSurface(surface, width / MINIMAP_WIDTH, height / MINIMAP_HEIGHT);
//	minimapsurface = SDL_ConvertSurface(map.getMiniMapSurface(), map.getMiniMapSurface()->format, map.getMiniMapSurface()->flags);
}

Map::~Map()
{
    // very ineff; disable for now
	// the brackets are important... don't forget them
	//delete [] tiles;

	delete creatures;

	SDL_FreeSurface(itemset);
	SDL_FreeSurface(monstersprites);
	SDL_FreeSurface(surface);
	SDL_FreeSurface(minimapsurface);
}


/*void Map::paintSurfaceArea(SDL_Rect* area, SDL_Surface* surf)
{ // we are assuming our area is within the map bounds
	SDL_Rect clipping;

	int areaw = area->w;
	int areah = area->h;
	int areax = area->x;
	int areay = area->y;


	// one of the problems here was that I did not take the floor of the camera coordinates...
/*
	for (int i = area->y; i < areah; i += TILESIZE)
		for (int j = area->x; j < areaw; j += TILESIZE)
		{
			clipping.x = 32; //tiles[i * width/TILESIZE + j].getId();// % 8 * TILESIZE;
			clipping.y = 0; // tiles[i * width/TILESIZE + j].getId();// / 8 * TILESIZE;
			clipping.w = clipping.h = TILESIZE;
			applySurface(j - areax, i - areay, tileset.getSheet(), surf, &clipping);
		}
*/ /*
	for (int i = 0; i < SCREEN_HEIGHT; i += TILESIZE)
		for (int j = 0; j < SCREEN_WIDTH; j += TILESIZE)
		{
			clipping.x = tiles[(areay / TILESIZE + i/TILESIZE) * width/TILESIZE + (areax / TILESIZE + j/TILESIZE)].getId() % 8 * TILESIZE;
			clipping.y = tiles[(areay / TILESIZE + i/TILESIZE) * width/TILESIZE + (areax / TILESIZE + j/TILESIZE)].getId() / 8 * TILESIZE;
			clipping.w = clipping.h = TILESIZE;
			applySurface(j, i, tileset.getSheet(), surf, &clipping);
		}		 
}*/
/*
void Map::paintTile(int x, int y, SDL_Rect* area, SDL_Surface* surf)
{
	SDL_Rect clipping;

	int tileindex = (area->y / TILESIZE) * width/TILESIZE + (area->x / TILESIZE);

	clipping.x = tiles[tileindex].getId() % 8 * TILESIZE;
	clipping.y = tiles[tileindex].getId() / 8 * TILESIZE;
	clipping.w = clipping.h = TILESIZE;

	applySurface(x, y, tileset.getSheet(), surf, &clipping);
}
*/

// to do: try tile.draw() with only partial map being drawn-

// creating many, many tile objects is very inefficient . . .
// disable for now.
/*void Map::paintSurface()
{
	Item item;
	stack<Item> auxStack;

	int indextile;

	SDL_Rect clipping;
	clipping.w = clipping.h = TILESIZE;
	//int tileindex = i * width/TILESIZE + j;

	for (int i = 0; i < height / TILESIZE; i++)
		for (int j = 0; j < width / TILESIZE; j++)
		{
			indextile = i * width/TILESIZE + j;
			clipping.x = tiles[indextile].getId() % 8 * TILESIZE;
			clipping.y = tiles[indextile].getId() / 8 * TILESIZE;

/* stack test
			tiles[indextile].getStack()->push(item);
			item.setId(5);
			tiles[indextile].getStack()->push(item);

*/
/*			applySurface(j * TILESIZE, i * TILESIZE, tileset.getSheet(), surface, &clipping);

			while (!tiles[indextile].getStack()->empty())
			{
				item = tiles[indextile].getStack()->top();
				tiles[indextile].getStack()->pop();
				auxStack.push(item);
				clipping.x = item.getId() % 16 * TILESIZE;
				clipping.y = item.getId() / 16 * TILESIZE;
				applySurface(j * TILESIZE, i * TILESIZE, itemset, surface, &clipping);
			}

			while (!auxStack.empty())
			{
				item = auxStack.top();
				auxStack.pop();
				tiles[indextile].getStack()->push(item);
			}
		}

	//paintMonsters();
}
*/
/*void Map::paintSurface2()
{
    Item item;
    stack<Item> auxStack;
    
    int indextile;
    
    SDL_Rect clipping;
    clipping.w = clipping.h = TILESIZE;
    //int tileindex = i * width/TILESIZE + j;
    
    for (int i = 0; i < height / TILESIZE; i++)
        for (int j = 0; j < width / TILESIZE; j++)
        {
            indextile = i * width/TILESIZE + j;
            clipping.x = tiles[indextile].getId() % 8 * TILESIZE;
            clipping.y = tiles[indextile].getId() / 8 * TILESIZE;
            
            /* stack test
             tiles[indextile].getStack()->push(item);
             item.setId(5);
             tiles[indextile].getStack()->push(item);
             
             */ /*
            applySurface(j * TILESIZE, i * TILESIZE, tileset.getSheet(), surface, &clipping);

            while (!tiles[indextile].getStack()->empty())
            {
                item = tiles[indextile].getStack()->top();
                tiles[indextile].getStack()->pop();
                auxStack.push(item);
                clipping.x = item.getId() % 16 * TILESIZE;
                clipping.y = item.getId() / 16 * TILESIZE;
                applySurface(j * TILESIZE, i * TILESIZE, itemset, surface, &clipping);
            }
            
            while (!auxStack.empty())
            {
                item = auxStack.top();
                auxStack.pop();
                tiles[indextile].getStack()->push(item);
            }
        }
    
    //paintMonsters();
}*/

/*
void Map::paintMonsters()
{
	SDL_Rect clipping;

	list<Creature*>::iterator it = creatures->begin();

	cout << "Creature list:" << endl;

	for (it = creatures->begin(); it != creatures->end(); it++)
	{
		// needs a fix:
		clipping.x = (it->getId()*TILESIZE % 1600/64) *TILESIZE;
		clipping.y = (it->getId() / 1280/64) *TILESIZE ;
		clipping.w = it->getWidth();
		clipping.h = it->getHeight();
		applySurface(it->getPosX(), it->getPosY(), monstersprites, surface, &clipping);
		cout << it->getName() << " has id " << it->getId() <<  endl;
	}

	cout << "=====" << endl;
}
*/
void Map::applyTile(const Tile &tile, int x, int y)
{
	//SDL_Surface* tile_set = tileset.
//	SDL_Surface* tile_set = loadImage("tileset.png");

	// Fills the spot with tile
	applySurface(x, y, tileset.getSheet(), surface, tile.getDimension());

//	SDL_FreeSurface(tile_set);
}

// a little flawed... sometimes the tiles are misplaced (dont stay aligned with the grid)
void Map::applyRandomTileRandomPosition()
{

	SDL_Rect tileoffset;

	tileoffset.x = (rand() % (TILESET_SIZE - TILESIZE + 1)) / TILESIZE * TILESIZE;
	tileoffset.y = (rand() % (TILESET_SIZE - TILESIZE + 1)) / TILESIZE * TILESIZE;

	int randx = (rand() % (width - TILESIZE + 1)) / TILESIZE * TILESIZE;
	int randy = (rand() % (height - TILESIZE + 1)) / TILESIZE * TILESIZE;

	// Fills the random spot with the random tile
	applySurface(randx, randy, tileset.getSheet(), surface, &tileoffset);
}

void Map::fill(const Tile &tile)
{
	//SDL_Surface* tile_set = tileset.
	SDL_Surface* tile_set = loadImage("tileset.png");

	// Fills the map with tile
	for (int i = 0; i < height; i += TILESIZE)
		for (int j = 0; j < width; j += TILESIZE)
			applySurface(j, i, tileset.getSheet(), surface, tile.getDimension());
			//applySurface(j, i, tile_set, surface, tile.getDimension());

	SDL_FreeSurface(tile_set);
}

bool Map::loadTxt(string filename)
{
	//SDL_FreeSurface(surface);

	ifstream input;
	int tileid;
	//Tile tile;
	int i = 0, j = 0;

//	SDL_Surface* tile_set = loadImage("tileset.png");

	input.open(filename.c_str());


/*
	// Rectangles to hold each tile
	SDL_Rect tile[101];

	// Holds the tile identification, i.e. array subscript
	int id = 0;

	// Adjust each tile's attribute
	for (int y = 0; y < 320; y += 32)
	{
		for (int x = 0; x < 320; x += 32)
		{
			tile[id].x = x;
			tile[id].y = y;
			tile[id].w = 32;
			tile[id].h = 32;
			id++;
		}
	}
*/


	// a lot flawed
	if (input.is_open())
	{
		// determine new size just by looking at the first number on the file,
		// very flawed
//		if	(input >> tileid)
//			width = height = tileid;
		input >> tileid;
		width = tileid;
		input >> tileid;
		height = tileid;	

		// Readjusts surface attributes
		surface = SDL_CreateRGBSurface(SDL_SWSURFACE, width, height, SCREEN_BPP, 0, 0, 0, 0);

		while (input >> tileid)
		{
			applySurface(j * TILESIZE, i * TILESIZE, tileset.getSheet(), surface, tileset.getTile(tileid).getDimension());
			//applySurface(j * TILESIZE, i * TILESIZE, tile_set, surface, &tile[tileid]);
			
			// changed to map width, not screen's
			if (j < width/TILESIZE - 1)
				j++;
			else if (i < height/TILESIZE - 1)
			{
				j = 0;
				i++; // note we don't check for the end of rows -- now we do
			}
		}
		input.close();

		//minimapsurface = shrinkSurface(surface, width / MINIMAP_WIDTH, height / MINIMAP_HEIGHT);

//		SDL_FreeSurface(tile_set);
		return true;

	}
	else
	{
//		SDL_FreeSurface(tile_set);
		return false;
	}
}
/*
void Map::loadTxtWithItemStack(string filename)
{
	ifstream file(filename.c_str());
	string str;
	stringstream ss;
	int tileindex = 0;
	int n;
	Item item;



	//if (!file.is_open())
	//	return;

	/**
	 * You have to readjust the surface !
	 */ /*
	file >> width;
	file >> height;

	// Readjusts surface attributes
	surface = SDL_CreateRGBSurface(SDL_SWSURFACE, width, height, SCREEN_BPP, 0, 0, 0, 0);

	// and the tiles dynamic array!
	// the brackets are necessary since tiles is a dynamic array

	delete [] tiles;

    // very inefficient; disable for now
	//tiles = new Tile[width / TILESIZE * height / TILESIZE];

	cerr << "width: " << width << " / height: " << height << endl;

	// quick fix 
	getline(file, str);

	while (getline(file, str))
	{
		//getline(file, str);
		ss << str;

		cout << str << endl;
	
		ss >> n;
		cout << n << endl;

		tiles[tileindex].setId(n);

		// COLLISION TEST
		if (n == 11) // test: set type of water (11) to WATER
			tiles[tileindex].setType(WATER); 

		cout << "tile[" << tileindex << "].getId() = " << tiles[tileindex].getId() << endl;

		cout << "stream: " << ss.str() << endl;

		while (ss >> n)
		{
			cout << "n is now " << n << endl;
			item.setId(n);
	
			tiles[tileindex].getStack()->push(item);
		}
		
		// reset the stream settings
		ss.clear();
		// wipe the stream
		ss.str("");

		tileindex++;
	}

	file.close();

	// Test collision
	tiles[2 + width/32].setType(NONWALKABLE);
	tiles[5 + width/32].setType(NONWALKABLE);
	tiles[3 + 3*width/32].setType(NONWALKABLE);
	
} */

void Map::loadTxt2(string filename)
{
    ifstream file(filename.c_str());
    string str;
    stringstream ss;
    int tileindex = 0;
    int n;
    Item item;
    
    
    
    //if (!file.is_open())
    //	return;
    
    /**
     * You have to readjust the surface !
     */
    file >> width;
    file >> height;
    
    // Readjusts surface attributes
    surface = SDL_CreateRGBSurface(SDL_SWSURFACE, width, height, SCREEN_BPP, 0, 0, 0, 0);
    
    // and the tiles dynamic array!
    // the brackets are necessary since tiles is a dynamic array
    
    //delete [] tiles;
    
    // very inefficient; disable for now
    //tiles = new Tile[width / TILESIZE * height / TILESIZE];
    
    cerr << "width: " << width << " / height: " << height << endl;
    
    // quick fix
    getline(file, str);
    
    while (getline(file, str))
    {
        //getline(file, str);
        ss << str;
        
        cout << str << endl;
        
        ss >> n;
        cout << n << endl;
        
        //tiles[tileindex].setId(n);
    
        
        cout << "stream: " << ss.str() << endl;
        
        while (ss >> n)
        {
            cout << "n is now " << n << endl;
            item.setId(n);
            
            //tiles[tileindex].getStack()->push(item);
        }
        
        // reset the stream settings
        ss.clear();
        // wipe the stream
        ss.str("");
        
        tileindex++;
    }
    
    file.close();
    
    
}

int Map::getWidth() const
{
	return width;
}

int Map::getHeight() const
{
	return height;
}

/*int Map::getTilesize() const
{
	return tilesize;
}*/

int Map::getTilesAcross() const
{
	return tiles_across;
}

int Map::getTilesDown() const
{
	return tiles_down;
}

SDL_Surface* Map::getSurface() const
{
	return surface;
}

SDL_Surface* Map::getMiniMapSurface() const
{
	// I decided to return a real time representation of the map m- failed
	//minimapsurface = SDL_ConvertSurface(surface, minimapsurface->format, minimapsurface->flags);

	return minimapsurface;
}

// why can't I return a reference ?? use const before tileset
Tileset* Map::getTileset()
{
	return &tileset;
}

int Map::getTileId(int index)
{
	return tiles[index].getId();
}

void Map::setWidth(int w)
{
	width = w;
}

void Map::setHeight(int h)
{
	height = h;
}

/*int Map::setTilesize(int size)
{
	tilesize = size;
}*/

void Map::setTileId(int index, int id)
{
	tiles[index].setId(id);
}

void Map::updateCreatureCamera(Creature& creat)//, SDL_Rect &camera)
{
	// Ceneter camera around player
//	camera.x = (pos.x + pos.w)/2 - SCREEN_WIDTH/2;
//	camera.y = (pos.y + pos.h)/2 - SCREEN_HEIGHT/2;

	creat.getCamera()->x = (creat.getPosX() + creat.getWidth()/2) - creat.getCamera()->w/2;
	creat.getCamera()->y = (creat.getPosY() + creat.getHeight()/2) - creat.getCamera()->h/2;

	if (creat.getCamera()->x < 0)
		creat.getCamera()->x = 0;

	// OMG! All the trouble because of a unseen semicolor right after this
	// if statement condition...
	if (creat.getCamera()->y < 0)
		creat.getCamera()->y = 0;

	if (creat.getCamera()->x > (width - creat.getCamera()->w))
		creat.getCamera()->x = (width - creat.getCamera()->w);

	if (creat.getCamera()->y > (height - creat.getCamera()->h))
		creat.getCamera()->y = (height - creat.getCamera()->h);

// Problematic: wrong..
/*	if (camera.x > (pos.x + pos.w/2) - camera.w/2)
		camera.x = map.getWidth() - camera.w;

	if (camera.y > (pos.y + pos.h/2) - camera.h/2)
		camera.y = (map.getHeight() - camera.h);*/
}
/*
void Map::randomPositionCreature(Creature& creat)
{

	creat.setPos(rand() % (width - creat.getWidth()), creat.getPosY() = rand() % (height - creat.getHeight()));

}
*/
// Not really random
/*void Map::randomPositionCreature()
{

	list<Creature>::iterator it = creatures->begin();

	for (it = creatures->begin(); it != creatures->end(); it++)
	{
		cout << "Gen: " << rand() % (width - it->getPosition().w) << " and " << (width - it->getPosition().w) << endl;
		it->setPosition(rand() % (width - it->getPosition().w), rand() % (height - it->getPosition().h));
		cout << "Random: " << it->getPosition().x << " and " << it->getPosition().y << endl;
	}


}*/

#ifndef __TILEMOVE__
// Error after placing more ||
// To be implemented: collision detection
// Problem: free movement and detection... multiple tiles...
void Map::moveCreature(Creature* creature) //, SDL_Rect &block)
{
	int posx, posy;

	int xspeed = creature->getMovSpeedX();
	int yspeed = creature->getMovSpeedY();

	int xfix, yfix;
	int indextile;

	creature->getPos(posx, posy);
	xfix = posx % TILESIZE;
	yfix = posy % TILESIZE;
	//indextile = posy/32 * width/TILESIZE + posx/32;
	
	creature->shiftPos(xspeed, yspeed);

	creature->getPos(posx, posy);

	indextile = (posy - posy%32)/32 * width/TILESIZE + (posx - posx%32)/32;

//	if (pos.x < 0 || pos.x > (SCREEN_WIDTH - pos.w) || hasCollided(pos, block) )
	if (posx < 0 || posx > (width - creature->getWidth()) 
		     //|| hasCollided(creature, tiles[indextile + 1]) 
//		     || hasCollided(creature, tiles[indextile + 1 - width/32]) 
		     //|| hasCollided(creature, tiles[indextile + 1 + width/32]) 



	   )
		creature->shiftPosX(-xspeed);

	
///	if (pos.y < 0 || pos.y > (SCREEN_HEIGHT - pos.h) || hasCollided(pos, block) )
	if (posy < 0 || posy > (height - creature->getHeight()) 
//		     || hasCollided(creature, tiles[indextile + width/32])
//		     || hasCollided(creature, tiles[indextile + width/32 + 1])
//		     || hasCollided(creature, tiles[indextile + width/32 - 1])
	   )
		creature->shiftPosY(-yspeed);

	// PROBLEM: shifting steps too fast
	if (creature->getMovSpeedX() != 0 || creature->getMovSpeedY() != 0)
		if(!creature->getNewDirection())
		{
			if (creature->getStep() != 0)
			{
				creature->shiftStep();
			}
			else
			{
				creature->setStep(1);
			}
		}
		else
			creature->setNewDirection(false);
}

#else
// Tile based movement
/*
void Map::moveCreature(Creature* c)
{
	int posx, posy;
	int xspeed, yspeed;
	int tileindex;

	c->getMovSpeed(xspeed, yspeed);

	c->shiftPos(xspeed, yspeed);

	c->getPos(posx, posy);
	
	// Okay, the problem is the tileindex. If we are at the end boundary of  a map and try to move
	// the index will be something we don't want... negative if we try go up the upper boundary...
	//tileindex = ((posy * width)/TILESIZE + posx)/TILESIZE;


	// will determine the tile that we stepped on... we use this as the index to check collision 
	// the height of the player has to be taken into acc.. 64.. translate tileindex 1 row up...
	tileindex = (posy+32)/TILESIZE * width/TILESIZE + posx / TILESIZE;

	cout << "Tile Index is " << tileindex << endl;

	// the statement below will cause a segfault for an invalid tileindex...

	//cout << "Dimension is " << tiles[tileindex].getDimension()->x << endl;

	// something is wrong with the hasCollided function; it crashes when we try to go out of map bounds...
	if (posx < 0 || posx > width - c->getWidth())// || hasCollided(c, tiles[tileindex + xspeed/TILESIZE]))
		c->shiftPosX(-xspeed);
	else // in the map
	{
		if (posx != width - c->getWidth() && xspeed > 0 && hasCollided(c, tiles[tileindex]))
			c->shiftPosX(-xspeed);
		else if (posx != 0 && xspeed < 0 && hasCollided(c, tiles[tileindex])) 
			c->shiftPosX(-xspeed);
	}
/*
	if (tileindex > 0 && tileindex < 2048 && posx != 0 && posx != width - c->getWidth() && hasCollided(c, tiles[tileindex + xspeed/TILESIZE]))
		c->shiftPosX(-xspeed);

*//*
	if (posy < 0 || posy > height - c->getHeight())// || hasCollided(c, tiles[tileindex + width/TILESIZE*yspeed/TILESIZE]))
		c->shiftPosY(-yspeed);
	else // in the map
	{
		if (posy != height - c->getHeight() && yspeed > 0 && hasCollided(c, tiles[tileindex]))
			c->shiftPosY(-yspeed);
		else if (posy != 0 && yspeed < 0 && hasCollided(c, tiles[tileindex]))
			c->shiftPosY(-yspeed);
	}
/*

	if (tileindex > 0 && tileindex < 1024 && posy != 0 && posy != height - c->getHeight() && hasCollided(c, tiles[tileindex + width/TILESIZE*yspeed/TILESIZE]))
		c->shiftPosY(-yspeed);
*//*
	if (xspeed != 0 || yspeed != 0)
		if(!c->getNewDirection())
		{
			if (c->getStep() != 0)
			{
				c->shiftStep();
			}
			else
			{
				c->setStep(1);
			}
		}
		else
			c->setNewDirection(false);
}*/

void Map::moveCreature(Creature* c)
{
	int posx, posy;
	int xspeed, yspeed;
	int tileindex;

	c->getMovSpeed(xspeed, yspeed);

	c->shiftPos(xspeed, yspeed);

	c->getPos(posx, posy);

	tileindex = (posy+32)/TILESIZE * width/TILESIZE + posx / TILESIZE;
	
	if (posx < 0 || posx > width - c->getWidth())
		c->shiftPosX(-xspeed);
	else if(tiles[tileindex].getType() != WALKABLE)
		c->shiftPosX(-xspeed);

	if (posy < 0 || posy > height - c->getHeight())
		c->shiftPosY(-yspeed);
	else if(tiles[tileindex].getType() != WALKABLE)
		c->shiftPosY(-yspeed);

}
		
	
	
#endif

void Map::addCreature(Creature* c)
{
	creatures->push_back(c);
}

// xcode fix for now
//#define __TILEMOVE__
#ifndef __TILEMOVE__
// not working
bool Map::hasCollided(Creature* c, const Tile& tile)
{
	int posx, posy;
	int w, h;

	cout << "I am alive! " << endl;

	if(tile.getType() == WALKABLE)
		return false;

	cout << "I am alive! 2" << endl;

	c->getPos(posx, posy);

	// there is a segfault on the following statements...

	

	if (tile.getDimension()->x - (posx + c->getWidth()) < 0)
		return true;

	cout << "I am alive! 3" << endl;

	if (tile.getDimension()->y - (posy + c->getHeight()) < 0)
		return true;

	cout << "I am alive! 4" << endl;
	return false;
	
	//return true;
}

#else
bool Map::hasCollided(Creature* c, const Tile& tile)
{
	int posx, posy;
	int w, h;
	c->getPos(posx, posy);

	w = c->getWidth();
	h = c->getHeight();

	if(tile.getType() == WALKABLE)
		return false;
	
	if(tile.getId() == 11)
		return true; // cannot walk onto water

//if our movement is tile based, we do not need to check collisions like this... just check the tile, whether it is blockable or not!!! duuuuh
/*
	int tilex = tile.getDimension()->x;
	int tiley = tile.getDimension()->y;
	int tilew = tile.getDimension()->w;
	int tileh = tile.getDimension()->h;

	if (posx + w <= tilex)
		return false;
	if (posy + h <= tiley)
		return false;
	if (posx >= tilex + tilew)
		return false;
	if (posy >= tiley + tileh)
		return false;
*/
	return true;
}
#endif


// New Stuff from here on
int Map::load(string filename, string format)
{
	int tileId;
	int i, j;
	int total_tiles;
	//int tiles_across, tiles_down;
	ifstream file;
	
	cout << "Loading map . . ." << endl;
	file.open(filename);
	
	
	if (file.is_open())
	{
		file >> width;
		file >> height;
		cout << "Map dimensions: " << width << " by " << height << endl;
		
		// Determine the number of tiles across and down
		tiles_across = width / TILESIZE;
		tiles_down = height / TILESIZE;
		total_tiles = tiles_across * tiles_down;
		
		// Everything could be accomplished with a single for loop
		// Fix this later.
/*		for (i = 0; i < tiles_down; i++)
			for (j = 0; j < tiles_across; j++)
			{
				// Get tile id from map file
				file >> tileId;
				
				// Print tile onto map surface, based on tile id
				paintTile(tileId, i * TILESIZE, j * TILESIZE);
 			}
				*/
		for (i = 0; i < total_tiles; i++)
		{
			file >> tileId;
			tiles[i].setId(tileId);
		}
		
		cout << "Map successfully loaded." << endl;
	}
	else
	{
		cerr << "Map::load: Could not open file" << endl;
		return 1;
	}
	
	file.close();
	return 0;
}

int Map::paintTile(int tileId, int pos_x, int pos_y)
{
	
	SDL_Rect clipping;
	clipping.w = TILESIZE;
	clipping.h = TILESIZE;
	
	applySurface(pos_x, pos_y, tileset.getSheet(), surface, &clipping);

	return 0;
}
