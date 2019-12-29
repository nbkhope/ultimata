/** @file
 * Contains the declaration of the Cursor class
 */

#ifndef __CURSOR_H__
#define __CURSOR_H__

#include "SDL2/SDL.h"

#include "Constants.h"
#include "GameMap.h"

/**
 * A cursor keeps track of where you are on the screen
 */
class Cursor
{
public:
	Cursor();
	~Cursor();
	
	void updateTileInfo(GameMap* gameMap);
	
	/**
	 * To get the cursor position
	 */
	int getPosX() const;
	int getPosY() const;
	
	/**
	 * To get the cursor area dimensions
	 */
	int getWidth() const;
	int getHeight() const;
	
	/**
	 * To get the position index of the tile within the cursor area.
	 */
	int getCursorTileIndex() const;
	/**
	 * To get the id of the tile within the cursor area.
	 * The tile id is used to determine which tile in the tileset to use.
	 */
	int getCursorTileId() const;
	
	/**
	 * Sets the cursor position
	 */
	void setPosX(int x);
	void setPosY(int y);
	
	/**
	 * Sets the cursor tile position index value.
	 */
	void setCursorTileIndex(int index);
	/**
	 * Sets the cursor tile id value
	 */
	void setCursorTileId(int index);
private:
	/**
	 * Rectangle structure to hold the position and area of the cursor.
	 */
	SDL_Rect pos;
	
	int cursorTileIndex;
	int cursorTileId;
};

#endif
