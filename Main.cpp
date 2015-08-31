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

#include "Main.h"

int main(int argc, char* args[])
{
	// Variable declarations
	System system;
	Graphics graphics;
	GameMap gameMap;
	Creature creature;
	Input input;
	int error;
	bool quit;
	
	//SDL_Event e;
	
	// Variable definitions
	quit = false;
	
	error = system.init(&graphics);
	
	
	if(error > 0) // error
	{
		return error;
	}
	else // no errors
	{
		if (!graphics.loadMedia())
		{
			cerr << "Could not load media:" << endl;
			cerr << SDL_GetError();
			return 43;
		}
		else // Ready for the main thing after all initializing and loading
		{
#ifndef __TEXTURE_RENDERING__
			graphics.displayImage();
#endif
			while (!quit)
			{
				// Handle Input
				if(input.get(&gameMap, &creature))
				{
					quit = true;
				}
				
#ifndef __TEXTURE_RENDERING__
				graphics.updateCurrentSurface();
#else
				graphics.render(&creature);
#endif
			}
		}
	}
	
	cout << "Finishing program. . ." << endl;
	
	return 0;
}
