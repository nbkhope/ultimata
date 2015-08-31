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

#include "System.h"

using namespace std;

System::System()
{
}

System::~System()
{
	// Shutdown SDL
	SDL_Quit();
}

/**
 * Returns 0 if successful initialization
 */
int System::init(Graphics* graphics)
{
	int error_code;
	int imgFlags = IMG_INIT_PNG;
	
	// assume no errors (code 0)
	error_code = 0;
	
	cout << "Initializing SDL. . ." << endl;
	
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		cerr << "SDL could not initialize:" << endl;
		// SDL_GetError() returns the latest error produced by a SDL function
		cerr << SDL_GetError();
		error_code = 1;
	}
	else // SDL initialized successfully
	{
		// Initialize SDL_image
		// IMG_Init() returns the flags that loaded successfully
		if (!(IMG_Init(imgFlags) & imgFlags))
		{
			cerr << "SDL_image could not initialize:" << endl;
			cerr << IMG_GetError();
		}
		else // all ok
		{
			graphics->setUpWindow();
		}
			
	}
		
	return error_code;
}
