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
#include <SDL/SDL.h>
#include <SDL_image/SDL_image.h>
#include <SDL_ttf/SDL_ttf.h>
#include <SDL_mixer/SDL_mixer.h>

#include "constants.h"
#include "sdl.h"
//#include "Main.h"

#include "System.h"
#include "Graphics.h"
#include "Creature.h"
#include "Map.h"
#include "Message.h"
#include "Player.h"
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


// int main(int argc, char* argv[])
// {
// 	/**
// 	 * Variables needed
// 	 */

// 	// A boolean to determine the quit status of our program
// 	bool quit = false;

// 	bool showCoordinates = false;

// 	bool chatmode = false;
// 	char character;
// 	stringstream chatinput;

// 	// Determines whether or not to control framerate
// 	bool framecap = false;
// 	int frame = 0;
// 	Timer fps;

// 	// A timer to control the update of the fps display
// 	Timer fpstimer;

// 	stringstream stopwatch;
// 	stopwatch << setprecision(2) << fixed;
// 	stringstream caption;
// 	caption << setprecision(2) << fixed;
// 	stringstream coordinates;

// 	// Surfaces to be used in the program
// 	SDL_Surface* background = NULL;

// 	// Holds an event structure
// 	SDL_Event event;

// 	/**
// 	 * Initialize systems
// 	 */
// 	System system;


// 	if (!system.isRunning())
// 		exit(1);

// 	Graphics graphics;

// 	graphics.init();

// 	if (!graphics.isRunning())
// 		quit = true;

// 	Sound sound;

// 	Message message;
	
// 	Map map;
// 	//Map map(2048, 1920, 32);

// 	Tile tile;
// 	//Tile tile(0, 32, TILESIZE);


// 	Timer timer;
// 	Player* player = new Player;
// //	Creature creature;// = new Creature;
// //	Monster monster;

// //	list<Creature> creatures;

	

// 	// cannot seem to know a way to move this to the Creature class:
// 	SDL_Rect charsetclip[4][3];
// 	// Set up the clipping
// 	for (int j = 0; j < 4; j++)
// 	{
// 		for (int i = 0; i < 3; i++)
// 		{
// 			charsetclip[j][i].x = i * 32;
// 			charsetclip[j][i].y = j * 64;
// 			charsetclip[j][i].w = 32;
// 			charsetclip[j][i].h = 64;
// 		}
// 	}


// 	/**
// 	 * Loading process
// 	 */

// 	// Seed random numbers with the current time
// 	srand(time(NULL));

// 	//map.fill(tile);

// 	/**
// 	 * Game Introduction: displays game logo then goes on to start the game
// 	 */
// 	SDL_Surface* gameLogo = graphics.getGameLogo();
// 	graphics.drawToBuffer(0, 0, gameLogo);
// 	graphics.displayBuffer();
// 	SDL_Delay(2000);

// 	/**
// 	 * New game
// 	 */

// 	graphics.displayBuffer();
// 	SDL_Delay(2000);

// //	map.loadTxt("map.txt");
// //	map.loadTxtWithItemStack("data/maps/collisionmap.txt");
// 	map.load("data/maps/gen_map.txt", "txt");

// 	// whole map surface is first updated on Map constructor

// 	// update whole map surface
// //	map.paintSurface();
// 	background = map.getSurface();

// 	graphics.drawMap(map);

// 	fps.start();
// 	fpstimer.start();

// 	/**
// 	 * Game Loop
// 	 */

// 	// Keeps going through the loop until it's time to quit;
// 	// i.e. when the user presses the X window button,
// 	while (!quit)
// 	{
// 		fps.reset();

// 		// Events section //
// 		// Handle input

// 		// Goes through the event queue
// 		while (SDL_PollEvent(&event))
// 		{
// 			if (!chatmode)
// 			// Handle player's input
// 			player->checkMovement(event);

// 			// The X window button is pressed, signaling
// 			// us to prepare to exit the program
// 			if (event.type == SDL_QUIT)
// 				quit = true;
// 			else if (event.type == SDL_KEYDOWN)
// 			{
// 				if (!chatmode)
// 				{
// 				switch(event.key.keysym.sym)
// 				{
// 					case SDLK_c:
// 						chatmode = true;
// 						break;
// 					case SDLK_s:
// 						if(!timer.isOn())
// 						{
// 							//timerstart = SDL_GetTick();
// 							//timer = true;
// 							timer.start();
// 						}
// 						else
// 						{
// 							//timer = false;
// 							timer.stop();
// 						}	
// 						break;
// 					case SDLK_p:
// 						if(!timer.isPaused())
// 							timer.pause();
// 						else
// 							timer.start();
// 						break;
// 					case SDLK_r:
// 						timer.reset();
// 						break;
// 					case SDLK_f:
// 						framecap = (!framecap);
// 						break;
// 					case SDLK_o:
// 						showCoordinates = !showCoordinates;
// 						break;
// 					case SDLK_q:
// 						quit = true;
// 						break;
// 				}
// 				}
// 				else
// 				{
// 				switch(event.key.keysym.sym)
// 				{
// 					case SDLK_RETURN:
// 						//message.display(chatinput, buffer);
// 						chatinput.str("");
// 						chatmode = false;
// 						break;
// 					case SDLK_ESCAPE:
// 						chatmode = false;
// 						break;
// 					case SDLK_SPACE:
// 						character = ' ';
// 						break;
// 					case SDLK_a:
// 						character = 'a';
// 						break;
// 					case SDLK_b:
// 						character = 'b';
// 						break;
// 					case SDLK_c:
// 						character = 'c';
// 						break;
// 					case SDLK_d:
// 						character = 'd';
// 						break;
// 					case SDLK_e:
// 						character = 'e';
// 						break;
// 					case SDLK_f:
// 						character = 'f';
// 						break;
// 					case SDLK_g:
// 						character = 'g';
// 						break;
// 					case SDLK_h:
// 						character = 'h';
// 						break;
// 					case SDLK_i:
// 						character = 'i';
// 						break;
// 					case SDLK_j:
// 						character = 'j';
// 						break;
// 					case SDLK_k:
// 						character = 'k';
// 						break;
// 					case SDLK_l:
// 						character = 'l';
// 						break;
// 					case SDLK_m:
// 						character = 'm';
// 						break;
// 					case SDLK_n:
// 						character = 'n';
// 						break;
// 					case SDLK_o:
// 						character = 'o';
// 						break;
// 					case SDLK_p:
// 						character = 'p';
// 						break;
// 					case SDLK_q:
// 						character = 'q';
// 						break;
// 					case SDLK_r:
// 						character = 'r';
// 						break;
// 					case SDLK_s:
// 						character = 's';
// 						break;
// 					case SDLK_t:
// 						character = 't';
// 						break;
// 					case SDLK_u:
// 						character = 'u';
// 						break;
// 					case SDLK_v:
// 						character = 'v';
// 						break;
// 					case SDLK_w:
// 						character = 'w';
// 						break;
// 					case SDLK_x:
// 						character = 'x';
// 						//chatinput << character << endl;
// 						break;
// 					case SDLK_y:
// 						character = 'y';
// 						break;
// 					case SDLK_z:
// 						character = 'z';
// 						break;
// 					default:
// 						character = '\0';
// 						break;
						
// 				}
// 				// How to make characters keep being typed when holding key down?
// 				// will do if ANY key is pressed down in chatmode
// 				if (character != '\0')
// 				chatinput << character;
// 				}
// 				//if (character != '\0')
// 				//chatinput << character;
// 			}
// /*			else if (event.type == SDL_MOUSEMOTION) // if the mouse was moved
// 			{
// 				int mx = event.motion.x;
// 				int my = event.motion.y;

// 				// if mouse is over the player's sprite,
// 				// then allow him to speak
// 				if ( (mx > pos.x) && (mx < pos.x + pos.w) && (my > pos.y) && (my < pos.y + pos.h) )
// 					speak = true;
// 			}
// 			else if (event.type == SDL_MOUSEBUTTONDOWN) // if the mouse was clicked
// 			{
// 				// left-click
// 				if (event.button.button == SDL_BUTTON_LEFT)
// 				{
// 					int mx = event.button.x;
// 					int my = event.button.y;

// 					// Moves the player to the clicked position's tile
// 					pos.x = mx / 32 * 32;
// 					pos.y = my / 32 * 32;
// 				}
// 			} */
         
// 		}

// 		// Logics section //
// 		// Apply movements

// 		//player->move(map);

// 		map.moveCreature(player);

// 		//player->updateCamera(map);//, camera);

// 		map.updateCreatureCamera(*player);

// 		//map.randomPositionCreature();
// 		//map.paintSurface();

// 		///////////////////


// 		// Rendering section //
// 		// Draws the stuff


// 		// Apply map to the screen

// 		//applySurface(0, 0, background, screen, player->getCamera());

// 		// uncomment for primitive map drawing
// 		// okay, now we just draw the map surface. But if changes were made to any tiles in the map, the surface
// 		// would have to be redrawn by whatever changed it. This is better than looping through every tile all the time.
// 		graphics.drawToBuffer(0, 0, map.getSurface(), player->getCamera());
// //		graphics.drawMapToBuffer(0, 0, player->getCamera());
		
// 		// Copy map to main buffer
// //		graphics.copyMapToBuffer();

// 		//map.paintSurfaceArea(player->getCamera(), graphics.getBuffer());



// /*		applySurface(player->getPosition().x - camera.x,
// 				     player->getPosition().y - camera.y,
// 				  	 charset, screen, &charsetclip[player->getDirection()]);*/
// 	/*	graphics.drawToBuffer(player->getPosition().x - player->getCamera()->x, 
// 					 player->getPosition().y - player->getCamera()->y,
// 					 player->getCharset(), &charsetclip[player->getDirection()]);*/
// 		graphics.drawToBuffer(player->getPosX() - player->getCamera()->x, 
// 					 player->getPosY() - player->getCamera()->y,
// 					 player->getCharset(), &charsetclip[player->getDirection()][player->getStep() + 1]);
// //					 player->getCharset(), &charsetclip[player->getDirection()*((player->getStep() + 1) + 4)]);

// 		if (showCoordinates)
// 		{
// 			// Clears the stream
// 			coordinates.str("");
// 			coordinates << "offset: x = " << player->getPosX() << " / y = " << player->getPosY() << endl;
// 			coordinates << "map size: " << map.getWidth() << " by " << map.getHeight() << endl;
// 			coordinates << "camera: x = " << player->getCamera()->x << " / " << "y = " << player->getCamera()->y << endl;

// 			// A mini map that is not implemented yet
// 			graphics.drawToBuffer(SCREEN_WIDTH - MINIMAP_WIDTH, 0,//SCREEN_HEIGHT - MINIMAP_HEIGHT,
// 						 map.getMiniMapSurface());

// 			message.displayTopLeft(coordinates, graphics);
// 		}

// 		if (chatmode)
// 			message.displayLine(32, 32, chatinput.str(), graphics);

// 		if (!sound.isPlaying())
// 			sound.play();


// 		// Clears the stream. You could also use str("")
// 		stopwatch.str(string());
// 		stopwatch << "Timer: " << timer.getTicks() / 1000.0 << " s";

//             // off for now
// 		//message.displayBottomCenter(stopwatch, graphics);

// 		// Display player's name over its sprite
// 		player->displayName(graphics.getBuffer());//, camera);

// 		if (player->getStats())
// 			player->displayStats(&message, graphics.getBuffer());

// 		graphics.displayBuffer();
// 		//SDL_Flip(screen);


// 		frame++;

// 		if (framecap)
// 		{
// 			// Time taken for each frame, in milliseconds
// 			int frameduration = static_cast<int>(1000.0 / FRAMES_PER_SECOND);
// 			int ticks = fps.getTicks();
// 			if (ticks < frameduration)
// 				SDL_Delay(frameduration - ticks);
// 		}

// 		if (fpstimer.getTicks() > 1000)
// 		{
// 			caption.str("");
// 			//caption << "My program :: Average FPS: " << frame / (fps.getTicks() / 1000.0) ;
// 		caption << "My program :: FPS: " << 1 / (fps.getTicks() / 1000.0)
// 			    << " // Random Number: " << rand();

// 			SDL_WM_SetCaption(caption.str().c_str(), NULL);

// 			fpstimer.reset();
// 		}

// 		if (frame >= FRAMES_PER_SECOND)
// 			frame = 0;
// 	}


// 	delete player;

// 	// Frees the memory used by the surfaces
// 	SDL_FreeSurface(background);

//     return 0;
// }

