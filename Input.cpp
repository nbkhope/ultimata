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
				default:
					// do nothing
					break;
			}
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
