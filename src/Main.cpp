/** @file
 * Moving player game
 *
 * @author	nbkhope
 * @version	0.2
 */

// Some basic libraries
#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <iomanip>
#include <ctime>
#include <list>
#include <vector>

// Necessary for SDL
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include "constants.h"
//#include "Main.h"

#include "System.h"
#include "Graphics.h"
#include "Creature.h"
#include "Sound.h"
#include "Tile.h"
#include "Timer.h"

using namespace std;

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
	int gameMode;

	//SDL_Event e;
	
	// Variable definitions
	//gameMode = GAME_MODE_TEST;
	gameMode = GAME_MODE_NORMAL;
	quit = false;
	
	error = system.init(&graphics);
	
	
	if(error > 0) // error
	{
		return error;
	}
	else // no errors
	{
		// Ready for the main thing after all initializing and loading
		
#ifndef __TEXTURE_RENDERING__
		graphics.displayImage();
#endif
		
		// Load Game Map
		gameMap.loadTxt("data/maps/map.txt");
		
		// Set up cursor
		input.getCursor()->updateTileInfo(&gameMap);
		
		/**
		 * Begins game loop
		 */
		while (!quit)
		{
			
			if (gameMode == GAME_MODE_TEST)
			{
				// Handle Input
				if (input.testGet(&gameMap, &creature))
				{
					quit = true;
				}
					
				graphics.testRender(&creature, &input);
			}
			else
			{
				// Handle Input
				if (input.get(&gameMap, &creature))
				{
					quit = true;
				}
				
#ifndef __TEXTURE_RENDERING__
				graphics.updateCurrentSurface();
#else
				graphics.render(&gameMap, &creature, &input);
#endif
				
			}
			
		}
		/**
		 * Game Loop End
		 */
			
		// Save Game Map
		gameMap.saveTxt("data/maps/map.txt");
	}
	
	cout << "Finishing program. . ." << endl;
	
	return 0;
}
