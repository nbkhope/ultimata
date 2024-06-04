#include "sdl.h"
#include <iostream>

using namespace std;

bool initialize()
{
	// Initializes SDL with all subsystems. 
	// Note we do not use all of the subsystem in this program.
	if (!SDL_Init(SDL_INIT_EVERYTHING))
	{
		// Initializes font system
		TTF_Init();

		// Initializes sound mixer, with arguments:
		// frequency,
		// sound format,
		// channels,
		// and sample rate.
		Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);

		return true;
	}
	else
		return false;

}

bool terminateSDL()
{
	bool state = true;

	// Terminates the font system
	TTF_Quit();

	// Terminates the audio system
	Mix_CloseAudio();

	// Terminates SDL, including freeing the screen surface.
	SDL_Quit();

	return state;
}

/**
 * Loads an image and optimizes it to the user video display
 * Note it sets color key to white, so any white part of the image
 * is set to transparent
 */
SDL_Surface* loadImage(string filename)
{
	// A surface to hold the original image
	SDL_Surface* loadedImage = NULL;

	// A surface to hold the converted image
	SDL_Surface* optimizedImage = NULL;

	// Loads the image
	//loadedImage = SDL_LoadBMP(filename.c_str());
	loadedImage = IMG_Load(filename.c_str());

	// If everything went well, we convert the image to fit the display's settings
	if (loadedImage != NULL)
	{
		// This function optimizes the given image to the user's display
		// and returns the address of the new image, leaving the original image
		// intact.
		optimizedImage = SDL_DisplayFormat(loadedImage);


		if (optimizedImage != NULL)
		{
			// Set our desired color key to white
			Uint32 colorkey = SDL_MapRGB(optimizedImage->format, 0xFF, 0xFF, 0xFF);
	
			// Apply color key to image.
			// Now white becomes transparent in the image
			SDL_SetColorKey(optimizedImage, SDL_SRCCOLORKEY, colorkey);
		}
	}
    else
        cerr << "loadImage(): failed" << endl;

    // Free the surface used by the original image
    SDL_FreeSurface(loadedImage);

	return optimizedImage;		
}

/**
 * Applies a surface to another surface
 */
void applySurface(int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip)
{
	// The BlitSurface function requires coordinates using a SDL_Rect,
	// which is a structure representing a rectangle of width x
	// and height y.
	SDL_Rect offset;
	offset.x = x;
	offset.y = y;

	// Blits the surface onto the destination. The position is given by a pointer
	// to a SDL_Rect, reason for the ampersand
	SDL_BlitSurface(source, clip, destination, &offset);
}


void setupScreen(SDL_Surface* &screen)
{
	// Video display set up
	screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE);

	// Adds a title to the program window
	SDL_WM_SetCaption("A SDL program", NULL);
}
