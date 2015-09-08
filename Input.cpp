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

#include "Input.h"

Input::Input()
{
	// Alpha blending <~test~>
	alpha = 255;
	for (int i = 0; i < 3; i++)
		color[i] = 255;
	
	// Change tile <~test~>
	testTileId = 17;
	
	// Image Loading Different Types <~test~>
	testImageLoad = 0;
	
	// Set initial cursor position
	cursor.setPosX(0);
	cursor.setPosY(0);
	//	cursor.setWidth(TILESIZE);
	//	cursor.setHeight(TILESIZE);
}

Input::~Input()
{
	
}

/**
 * Handles input for a non-scrolling, fixed-screen map.
 */
bool Input::get(GameMap* gameMap, Creature *c)
{
	int pos_x, pos_y;
	int mouse_x, mouse_y;
	//int cursorTileIndex;
	//int cursorTileId;
	//int tileId;
	bool quit = false; // Using this to send quit signal back, for now . . . at least.
	
	pos_x = c->getPosX();
	pos_y = c->getPosY();
	
//	cursorTileIndex = cursor.getPosX() / TILESIZE + (cursor.getPosY() / TILESIZE) * gameMap->getTilesAcross();
//	cursorTileId = gameMap->getTile(cursorTileIndex)->getId();
	
	// For a complete list of keyboard enum, check:
	// http://wiki.libsdl.org/SDL_Keycode?highlight=%28\bCategoryEnum\b%29|%28CategoryKeyboard%29
	// https://www.libsdl.org/release/SDL-1.2.15/docs/html/sdlkey.html
	
	// Handle events on queue
	// Keeps doing while there are still events left
	while (SDL_PollEvent(&e) != 0)
	{
		if (e.type == SDL_QUIT)
		{
			cout << "SDL_QUIT event!" << endl;
			quit = true;
		}
		else if (e.type == SDL_KEYDOWN)
		{
			cout << "SDL_KEYDOWN event!" << endl;
			
			switch (e.key.keysym.sym)
			{
				case SDLK_q: // quit
					quit = true;
					break;
				case SDLK_UP:
					if (pos_y != 0) // if not trying to go out of bounds
						c->setPosY(pos_y - TILESIZE);
					else
						cout << "SDLK_UP: Attempt to go out of bounds!!" << endl;
					break;
				case SDLK_DOWN:
					if (pos_y != SCREEN_HEIGHT - TILESIZE) // if not trying to go out of bounds
						c->setPosY(pos_y + TILESIZE);
					else
						cout << "SDLK_DOWN: Attempt to go out of bounds!!" << endl;
					break;
				case SDLK_LEFT:
					if (pos_x != 0) // if not trying to go out of bounds
						c->setPosX(pos_x - TILESIZE);
					else
						cout << "SDLK_LEFT: Attempt to go out of bounds!!" << endl;
					break;
				case SDLK_RIGHT:
					if (pos_x != SCREEN_WIDTH - TILESIZE) // if not trying to go out of bounds
						c->setPosX(pos_x + TILESIZE);
					else
						cout << "SDLK_RIGHT: Attempt to go out of bounds!!" << endl;
					break;
				case SDLK_t: // change tile <~test~>
					if (testTileId < 256)
						testTileId++;
					else
						testTileId = 0;
					cout << "testTileId = " << testTileId << endl;
					break;
					
				case SDLK_y:
					if (testTileId < 256)
						testTileId += 16;
					else
						testTileId = 0;
					cout << "testTileId = " << testTileId << endl;
					break;
				case SDLK_TAB: // Image Loading Different Types <~test~>
					if (testImageLoad < 3)
						testImageLoad++;
					else
						testImageLoad = 0;
					cout << "testImageLoad = " << testImageLoad << endl;
					break;
				case SDLK_LEFTBRACKET:
					if (cursor.getCursorTileId() != 0)
					{
						//tileId = gameMap->getTile(cursor.getCursorTileIndex())->getId();
						//gameMap->getTile(cursor.getCursorTileIndex())->setId(tileId - 1);
						gameMap->getTile(cursor.getCursorTileIndex())->decrementId();
					}
					break;
				case SDLK_RIGHTBRACKET:
					if (cursor.getCursorTileId() != 255)
					{
						//tileId = gameMap->getTile(cursor.getCursorTileIndex())->getId();
						//gameMap->getTile(cursor.getCursorTileIndex())->setId(tileId + 1);
						gameMap->getTile(cursor.getCursorTileIndex())->incrementId();
					}
					break;
				default:
					// do nothing
					break;
			}
			
		}
		else if (e.type == SDL_MOUSEBUTTONDOWN) // buggy
		{
			cout << "SDL_MOUSEBUTTONDOWN event!" << endl;
			
			SDL_GetMouseState(&mouse_x, &mouse_y);
			//switch (e.button.button == SDL_BUTTON_LEFT)
			//{
				/*	c->setPosX(mouse_x - mouse_x % TILESIZE);
					c->setPosY(mouse_y - mouse_y % TILESIZE); */
			//}
			
			// Adjust cursor region
			cursor.setPosX(mouse_x - mouse_x % TILESIZE);
			cursor.setPosY(mouse_y - mouse_y % TILESIZE);
			cursor.updateTileInfo(gameMap);
		}
	}
	
	return quit;
}

/**
 * Same as Input::get(), but for testing purposes.
 * Handles input for a non-scrolling, fixed-screen map.
 */
bool Input::testGet(GameMap* gameMap, Creature *c)
{
	int pos_x, pos_y;
	int mouse_x, mouse_y;
	bool quit = false; // Using this to send quit signal back, for now . . . at least.
	
	pos_x = c->getPosX();
	pos_y = c->getPosY();
	
	// For a complete list of keyboard enum, check:
	// http://wiki.libsdl.org/SDL_Keycode?highlight=%28\bCategoryEnum\b%29|%28CategoryKeyboard%29
	// https://www.libsdl.org/release/SDL-1.2.15/docs/html/sdlkey.html
	
	// Handle events on queue
	// Keeps doing while there are still events left
	while (SDL_PollEvent(&e) != 0)
	{
		if (e.type == SDL_QUIT)
		{
			cout << "SDL_QUIT event!" << endl;
			quit = true;
		}
		else if (e.type == SDL_KEYDOWN)
		{
			cout << "SDL_KEYDOWN event!" << endl;
			
			switch (e.key.keysym.sym)
			{
				case SDLK_q: // quit
					quit = true;
					break;
				case SDLK_UP:
					if (pos_y != 0) // if not trying to go out of bounds
						c->setPosY(pos_y - TILESIZE);
					else
						cout << "SDLK_UP: Attempt to go out of bounds!!" << endl;
					break;
				case SDLK_DOWN:
					if (pos_y != SCREEN_HEIGHT - TILESIZE) // if not trying to go out of bounds
						c->setPosY(pos_y + TILESIZE);
					else
						cout << "SDLK_DOWN: Attempt to go out of bounds!!" << endl;
					break;
				case SDLK_LEFT:
					if (pos_x != 0) // if not trying to go out of bounds
						c->setPosX(pos_x - TILESIZE);
					else
						cout << "SDLK_LEFT: Attempt to go out of bounds!!" << endl;
					break;
				case SDLK_RIGHT:
					if (pos_x != SCREEN_WIDTH - TILESIZE) // if not trying to go out of bounds
						c->setPosX(pos_x + TILESIZE);
					else
						cout << "SDLK_RIGHT: Attempt to go out of bounds!!" << endl;
					break;
				// Alpha blending <~test~>
				case SDLK_o: // increase alpha
					if (alpha + 32 < 255)
						alpha += 32;
					else
						alpha = 255;
					cout << "Alpha set to " << int(alpha) << endl; // we need to convert Uint8 to int
					break;
				case SDLK_p: // decrease alpha
					if (alpha - 32 > 0)
						alpha -= 32;
					else
						alpha = 0;
					cout << "Alpha set to " << int(alpha) << endl;
					break;
				case SDLK_j: // decrease R color
					if (color[0] - 32 > 0)
						color[0] -= 32;
					else
						color[0] = 0;
					break;
				case SDLK_k: // decrease G color
					if (color[1] - 32 > 0)
						color[1] -= 32;
					else
						color[1] = 0;
					break;
				case SDLK_l: // decrease B color
					if (color[2] - 32 > 0)
						color[2] -= 32;
					else
						color[2] = 0;
					break;
				case SDLK_SEMICOLON: // reset color modulation
					for (int i = 0; i < 3; i++)
						color[i] = 255;
					break;
				default:
					// do nothing
					break;
			}
			cout << "Texture color: (" << int(color[0]) << ", " << int(color[1]) << ", "
				 << int(color[2]) << ")" << endl;
 		}
		else if (e.type == SDL_MOUSEBUTTONDOWN) // buggy
		{
			cout << "SDL_MOUSEBUTTONDOWN event!" << endl;
			
			SDL_GetMouseState( &mouse_x, &mouse_y );
			//switch (e.button.button == SDL_BUTTON_LEFT)
			//{
			c->setPosX(mouse_x - mouse_x % TILESIZE);
			c->setPosY(mouse_y - mouse_y % TILESIZE);
			//}
		}
	}
	
	return quit;
}

// Alpha blending <~test~>
Uint8 Input::getAlpha() const
{
	return alpha;
}

void Input::setAlpha(Uint8 a)
{
	alpha = a;
}

Uint8* Input::getColor()
{
	return color;
}

void Input::setColor(Uint8* newColor)
{
	color[0] = newColor[0];
	color[1] = newColor[1];
	color[2] = newColor[2];
}

int Input::getTestTileId() const
{
	return testTileId;
}

int Input::getTestImageLoad() const
{
	return testImageLoad;
}

Cursor* Input::getCursor()
{
	return &cursor;
}
