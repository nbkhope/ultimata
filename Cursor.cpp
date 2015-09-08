//	Copyright (C) 2010-2011, 2015  Renan Martins
//
//	This program is free software; you can redistribute it and/or
//	modify it under the terms of the GNU General Public License
//	as published by the Free Software Foundation; either version 2
//	of the License, or (at your option) any later version.
//
//	This program is distributed in the hope that it will be useful,
//	but WITHOUT ANY WARRANTY; without even the implied warranty of
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//	GNU General Public License for more details.
//
//	You should have received a copy of the GNU General Public License
//	along with this program; if not, write to the Free Software
//	Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

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
