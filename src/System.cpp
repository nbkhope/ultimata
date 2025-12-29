#include "System.h"

using namespace std;

System::System()
{
	running = false;
	ttfInitialized = false;
}

System::~System()
{
	// Terminates the font system (only if it was successfully initialized)
	if (ttfInitialized)
	{
		TTF_Quit();
	}

	// Terminates the audio system
	//Mix_CloseAudio();

	// Terminates SDL, including freeing the screen surface.
	// Shutdown SDL
	SDL_Quit();
}

/**
 * Returns 0 if successful initialization
 */
int System::init(Graphics* graphics)
{
	int error_code;
	int imgFlags = IMG_INIT_PNG|IMG_INIT_JPG; // load support for png and jpg
	// * see https://www.libsdl.org/projects/SDL_image/docs/SDL_image.html
	
	cout << "Ultimata 2: Running on " << SDL_GetPlatform() << endl;
	
	// assume no errors (code 0)
	error_code = 0;
	
	cout << "Initializing SDL. . ." << endl;
	
	// Initialize only the required SDL subsystems for better performance
	// VIDEO: Required for windows, rendering, surfaces, textures
	// TIMER: Required for SDL_GetTicks() and SDL_Delay() 
	// EVENTS: Required for input handling (keyboard, mouse)
	// AUDIO: Required for music playback in the main menu
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_EVENTS | SDL_INIT_AUDIO) < 0)
	{
		cerr << "SDL could not initialize:" << endl;
		// SDL_GetError() returns the latest error produced by a SDL function
		cerr << SDL_GetError();
		error_code = 1;
	}
	else // SDL initialized successfully
	{
		cout << "Initializing SDL_image" << endl;
		// Initialize SDL_image
		// * See http://sdl.beuc.net/sdl.wiki/SDL_image
		// IMG_Init() returns the flags that loaded successfully
		// (a bitmask of all the currently initted image loaders)
		if (!(IMG_Init(imgFlags) & imgFlags))
		{
			cerr << "SDL_image could not initialize:" << endl;
			cerr << IMG_GetError();
		}
		else // all ok
		{
			graphics->setUpWindow();
			
			/**
			 * Load media after setting up game window
			 */
			if (!graphics->loadMedia())
			{
				cerr << "Could not load media:" << endl;
				cerr << SDL_GetError();
				return 43;
			}
		}

		// Initializes font system
		cout << "Initializing SDL_ttf" << endl;
		if (TTF_Init() == -1)
		{
			cerr << "SDL_ttf could not initialize:" << endl;
			cerr << TTF_GetError() << endl;
			return 44;  // Return early on TTF failure to prevent crashes
		}
		else
		{
			ttfInitialized = true;  // Mark TTF as successfully initialized
		}

		// Initializes sound mixer, with arguments:
		// frequency,
		// sound format,
		// channels,
		// and sample rate.
		//Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);

		running = true;
	}
		
	return error_code;
}

void System::run()
{
	//
}

bool System::isRunning() const
{
	return running;
}

