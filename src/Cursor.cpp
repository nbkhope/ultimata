#include "Cursor.h"

Cursor::Cursor()
{
	pos.x = 0;
	pos.y = 0;
	pos.w = TILESIZE;
	pos.h = TILESIZE;
	
	cursorTileIndex = 0;
	cursorTileId = -1;
}

Cursor::~Cursor()
{
	
}

void Cursor::updateTileInfo(GameMap* gameMap)
{
	cursorTileIndex = pos.x / TILESIZE + (pos.y / TILESIZE) * gameMap->getTilesAcross();
	cursorTileId = gameMap->getTile(cursorTileIndex)->getId();
}

int Cursor::getPosX() const
{
	return pos.x;
}

int Cursor::getPosY() const
{
	return pos.y;
}

int Cursor::getWidth() const
{
	return pos.w;
}

int Cursor::getHeight() const
{
	return pos.h;
}

int Cursor::getCursorTileIndex() const
{
	return cursorTileIndex;
}

int Cursor::getCursorTileId() const
{
	return cursorTileId;
}

void Cursor::setPosX(int x)
{
	pos.x = x;
}

void Cursor::setPosY(int y)
{
	pos.y = y;
}

void Cursor::setCursorTileIndex(int index)
{
	cursorTileIndex = index;
}

void Cursor::setCursorTileId(int id)
{
	cursorTileId = id;
}
