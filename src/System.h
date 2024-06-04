/** @file
 * System core class.
 * Responsible for initializing SDL. Should not be responsible for
 * initializing sound, graphics, fonts, etc.
 */

#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#include <SDL/SDL.h>
#include <SDL_image/SDL_image.h>
#include <SDL_ttf/SDL_ttf.h>
#include <SDL_mixer/SDL_mixer.h>
#include <iostream>

/*#ifdef __APPLE__
	#include "TargetConditionals.h"
	#if TARGET_OS_MAC
		#include "SDL2/SDL.h"
	#endif*/
#ifdef __OSX__
	#include "SDL2/SDL.h"
#elif __linux
	// Linux
#elif _WIN32
	// WIndows
#endif

#include "Constants.h"
#include "Graphics.h"

using namespace std;

/**
 * Responsible for initializing the SDL API
 * and setting up the game
 */
class System
{
public:
	/**
	 * Constructor and destructor
	 */
	System();
	~System();
	
	/**
	 * Initializes SDL and its subsystems
	 */
	int init(Graphics* graphics);

	bool isRunning() const;
	void run();
	

private:

	enum GameState
	{
		INTRO,
		NEWGAME,
		INGAME,
		GAMEOVER
	};

//	SDL_Surface* buffer;

	bool running;
};

#endif
