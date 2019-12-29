/** @file
 */

#ifndef __TILE_H__
#define __TILE_H__

/**
 * A tile object represents a map tile. It has information 
 * about the tile id and whether the tile is walkable. The 
 * Tile object has nothing to do with tile images.
 */
class Tile
{
public:
	Tile();
	~Tile();
	
	/**
	 * @return the tile id. Useful to determine which part of the tileset to extract.
	 */
	int getId() const;
	/**
	 * Returns whether the tile is walkable
	 */
	bool isWalkable() const;
	
	void setId(int newId);
	void setWalkable(bool canWalk);
	
	void incrementId();
	void decrementId();
private:
	int id;
	
	bool walkable;
	// you could: check whether each tile is walkable with this
	// or let the tileset define which tile ids are walkable
	// we will use the first approach
};

#endif
