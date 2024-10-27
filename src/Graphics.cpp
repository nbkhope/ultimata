#include "Graphics.h"

LTexture* Graphics::getMyBackground()
{
	return &myBackground;
}

LTexture* Graphics::getMyForeground()
{
	return &myForeground;
}

// LTexture Functions
LTexture::LTexture()
{
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}
LTexture::~LTexture()
{
	free();
}

/**
 * Loads a texture into a LTexture object. All images are color keyed (i.e. colors that match the
 * color key will be made transparent)
 */
bool LTexture::loadFromFile(string filename, SDL_Renderer* gRenderer)
{
	SDL_Texture* newTexture = NULL;
	
	// Gets rid of whatever was loaded before
	free();
	
	
#ifdef __DEBUG_MODE__
	cout << "(LTexture) Loading texture " << filename << ". . ." << endl;
#endif
	SDL_Surface* loadedSurface = IMG_Load(filename.c_str());
	
	if (loadedSurface == NULL)
	{
		cerr << "Surface could not be loaded" << endl;
		cerr << IMG_GetError();
	}
	else
	{
		// Color key image before creating texture (Cyan: #00FFFF)
		// Purple color key: #C00080
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0xC0, 0x00, 0x80));
		
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		
		if (newTexture == NULL)
		{
			cerr << "Texture could not be created" << endl;
			cerr << SDL_GetError();
		}
		else
		{
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
			
#ifdef __DEBUG_MODE__
			cout << "Texture loaded." << endl;
#endif
		}
		
		SDL_FreeSurface(loadedSurface);
	}
	
	mTexture = newTexture;
	
	return mTexture != NULL;
}

/**
 * Deallocates texture
 */
void LTexture::free()
{
	if (mTexture != NULL)
	{
		SDL_DestroyTexture(mTexture);
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

/**
 * Renders a texture, given position and renderer.
 */
void LTexture::render(int x, int y, SDL_Renderer* gRenderer, SDL_Rect* clip)
{
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };
	
	if (clip != NULL)
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}
	
	SDL_RenderCopy(gRenderer, mTexture, clip, &renderQuad);
}

/**
 * Sets texture color modulation
 */
void LTexture::setColor(Uint8 r, Uint8 g, Uint8 b)
{
	SDL_SetTextureColorMod(mTexture, r, g, b);
}

/**
 * Sets texture blending
 */
void LTexture::setBlendMode(SDL_BlendMode blending)
{
	SDL_SetTextureBlendMode(mTexture, blending);
}

/**
 * Sets texture alpha modulation
 *
 * 100% alpha (255) -- opaque
 * 75% alpha (191)
 * 50% alpha (127)
 * 25% alpha (63)
 * 0% alpha (0) -- fully transparent
 */
void LTexture::setAlpha(Uint8 alpha)
{
	// Note: Uint8 is an unsigned, 8-bit integer, ranging from 0 to 255
	SDL_SetTextureAlphaMod(mTexture, alpha);
}

/**
 * @return the width of the texture
 */
int LTexture::getWidth() const
{
	return mWidth;
}

/**
 * @return the height of the texture
 */
int LTexture::getHeight() const
{
	return mHeight;
}

Graphics::Graphics()
{
	window = NULL;
	screenSurface = NULL;
	
	gRenderer = NULL;
	gTexture = NULL;

	gameIcon = NULL;
	gameLogo = NULL;
	image = NULL;
	
	// Image Loading Different Types <~test~>
	image_jpg = NULL;
	image_tiff = NULL;
	image_bmp = NULL;

	buffer = NULL;
	buffer2 = NULL;
	logo = NULL;
	running = false;
}

Graphics::~Graphics()
{
	SDL_FreeSurface(image);
	image = NULL;
	SDL_FreeSurface(gameIcon);
	SDL_FreeSurface(gameLogo);

	SDL_FreeSurface(buffer);
	SDL_FreeSurface(buffer2);
	SDL_FreeSurface(logo);
	
	// The issue was: if currentSurface is associated with the window, then it will automatically be
	// freed along with the window?
	//SDL_FreeSurface(currentSurface);
	//currentSurface = NULL;
	
	SDL_DestroyTexture(gTexture);
	SDL_DestroyRenderer(gRenderer);
	gTexture = NULL; // Is this really necessary?
	gRenderer = NULL;
	
	// Clean up (takes care of window and window surface (screenSurface)
	SDL_DestroyWindow(window);
	window = NULL; // Is this really necessary?
	
	// Image Loading Different Types <~test~>
	SDL_DestroyTexture(image_jpg);
	SDL_DestroyTexture(image_tiff);
	SDL_DestroyTexture(image_bmp);
}

void Graphics::setUpWindow()
{
	/**
	 * Sets up a window at the center
	 */
	window = SDL_CreateWindow("Ultimata 2",
							  SDL_WINDOWPOS_CENTERED,
							  SDL_WINDOWPOS_CENTERED,
							  SCREEN_WIDTH,
							  SCREEN_HEIGHT,
							  SDL_WINDOW_SHOWN);
	
	if (window == NULL) // if there is an error, function returns NULL
	{
		cerr << "Window could not be created:" << endl;
		cerr << SDL_GetError();
	}
	else // all ok
	{
		SDL_SetWindowIcon(window, gameIcon);

#ifndef __TEXTURE_RENDERING__
		// note: this surface will be freed when the window is destroyed
		// 		 (no need to free this later on)
		screenSurface = SDL_GetWindowSurface(window);
		// Necessary for loadSurface() function, but will cause problems if used with gRenderer

		// Paints the screen surface white (#FFFFFF)
		SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));
		
		// Updates the window surface after the changes made above
		SDL_UpdateWindowSurface(window);
		
		// Wait 2000 ms (2 s)
		SDL_Delay(2000);
#else
		// Use Texture Rendering
		gRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
		
		if (gRenderer == NULL)
		{
			cerr << "Could not create renderer:" << endl;
			cerr << SDL_GetError();
		}
		else
		{
			// Initialize renderer color
			// arguments: Renderer; R,G,B and Alpha (transparency)
			SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
			SDL_Delay(1000);
		}
#endif
	}
}

bool Graphics::loadMedia()
{
	bool success;
	
	success = true;
	
	cout << "Loading media. . ." << endl;
	
	Uint16 pixels[16*16] = {
		0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff,
		0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff,
		0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff,
		0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff,
		0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff,
		0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff,
		0x0fff, 0x0aab, 0x0789, 0x0bcc, 0x0eee, 0x09aa, 0x099a, 0x0ddd,
		0x0fff, 0x0eee, 0x0899, 0x0fff, 0x0fff, 0x1fff, 0x0dde, 0x0dee,
		0x0fff, 0xabbc, 0xf779, 0x8cdd, 0x3fff, 0x9bbc, 0xaaab, 0x6fff,
		0x0fff, 0x3fff, 0xbaab, 0x0fff, 0x0fff, 0x6689, 0x6fff, 0x0dee,
		0xe678, 0xf134, 0x8abb, 0xf235, 0xf678, 0xf013, 0xf568, 0xf001,
		0xd889, 0x7abc, 0xf001, 0x0fff, 0x0fff, 0x0bcc, 0x9124, 0x5fff,
		0xf124, 0xf356, 0x3eee, 0x0fff, 0x7bbc, 0xf124, 0x0789, 0x2fff,
		0xf002, 0xd789, 0xf024, 0x0fff, 0x0fff, 0x0002, 0x0134, 0xd79a,
		0x1fff, 0xf023, 0xf000, 0xf124, 0xc99a, 0xf024, 0x0567, 0x0fff,
		0xf002, 0xe678, 0xf013, 0x0fff, 0x0ddd, 0x0fff, 0x0fff, 0xb689,
		0x8abb, 0x0fff, 0x0fff, 0xf001, 0xf235, 0xf013, 0x0fff, 0xd789,
		0xf002, 0x9899, 0xf001, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0xe789,
		0xf023, 0xf000, 0xf001, 0xe456, 0x8bcc, 0xf013, 0xf002, 0xf012,
		0x1767, 0x5aaa, 0xf013, 0xf001, 0xf000, 0x0fff, 0x7fff, 0xf124,
		0x0fff, 0x089a, 0x0578, 0x0fff, 0x089a, 0x0013, 0x0245, 0x0eff,
		0x0223, 0x0dde, 0x0135, 0x0789, 0x0ddd, 0xbbbc, 0xf346, 0x0467,
		0x0fff, 0x4eee, 0x3ddd, 0x0edd, 0x0dee, 0x0fff, 0x0fff, 0x0dee,
		0x0def, 0x08ab, 0x0fff, 0x7fff, 0xfabc, 0xf356, 0x0457, 0x0467,
		0x0fff, 0x0bcd, 0x4bde, 0x9bcc, 0x8dee, 0x8eff, 0x8fff, 0x9fff,
		0xadee, 0xeccd, 0xf689, 0xc357, 0x2356, 0x0356, 0x0467, 0x0467,
		0x0fff, 0x0ccd, 0x0bdd, 0x0cdd, 0x0aaa, 0x2234, 0x4135, 0x4346,
		0x5356, 0x2246, 0x0346, 0x0356, 0x0467, 0x0356, 0x0467, 0x0467,
		0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff,
		0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff,
		0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff,
		0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff
	};
	gameIcon = SDL_CreateRGBSurfaceFrom(pixels, 16, 16, 16, 16*2,
										0x0f00, 0x00f0, 0x000f, 0xf000);
	//gameIcon = loadSurface("data/images/gameicon.png");
	
	gameLogo = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, 0x0f00, 0x00f0, 0x000f, 0xf000);
	
#ifndef __TEXTURE_RENDERING__
	image = loadSurface("data/images/image.bmp");
	
#endif
	
	//tileSheet = loadTexture("data/images/PathAndObjects.png");
	
	//if (tileSheet == NULL)
	if(!tileSheet.loadFromFile("data/images/PathAndObjects.png", gRenderer))
	{
		cerr << "Could not load tile sheet:" << endl;
		cerr << SDL_GetError();
		success = false;
	}
	
	if(!playerCharset.loadFromFile("data/images/player_charset.png", gRenderer))
	{
		cerr << "Could not load player charset:" << endl;
		cerr << SDL_GetError();
		success = false;
	}
	
	// Alpha blending <~test~>
	myBackground.loadFromFile("data/images/shakyamuni.png", gRenderer); // should check if failed
	myForeground.loadFromFile("data/images/kongokai.png", gRenderer);
	// Set blend mode; the background texture doesn't need to be set, only the foreground
	myForeground.setBlendMode(SDL_BLENDMODE_BLEND);
	
	// Image Loading Different Types <~test~>
	image_jpg = loadTexture("data/images/taizokai.jpg");
	image_tiff = loadTexture("data/images/taizokai.tiff");
	image_bmp = loadTexture("data/images/image.bmp");
	
	return success;
}

void Graphics::displayImage()
{
	cout << "Displaying image . . ." << endl;
	
	// Apply image
	SDL_BlitSurface(image, NULL, screenSurface, NULL);
	
	SDL_UpdateWindowSurface(window);
	
	SDL_Delay(2000);
}

void Graphics::render(GameMap* gameMap, Creature* creature, Input* input)
{
	// Make sure to re-set RenderDrawColor
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(gRenderer); // Fills the screen with the DrawColor
	//SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);
	
	// For the timer
	string title;
	title = "Ultimata :: " + to_string(input->getTimer()->getTicks() / 1000.);
	SDL_SetWindowTitle(window, title.c_str());
	
	//drawPrimitiveTiles();
	//drawTiles(gameMap->getTileset(), input->getTestTileId());
	drawGameMap(gameMap);
	
	// Set viewports. Viewports work with texture. Drawing primitive tiles using SDL_SetRenderXXXX does
	// not make use of any textures.
	//setViewport();
	
	//drawColorKeyExample();
	
	drawCursor(input->getCursor());

	drawCreature(creature);
	
	// Image Loading Different Types <~test~>
	switch (input->getTestImageLoad())
	{
		case 1:
			SDL_RenderCopy(gRenderer, image_bmp, NULL, NULL);
			break;
		case 2:
			SDL_RenderCopy(gRenderer, image_jpg, NULL, NULL);
			break;
		case 3:
			SDL_RenderCopy(gRenderer, image_tiff, NULL, NULL);
			break;
		default:
			// no image
			break;
	}
	
	SDL_RenderPresent(gRenderer);
}

/**
 * Same as Graphics::render(), but for testing purposes.
 */
void Graphics::testRender(Creature* creature, Input* input)
{
	Uint8 r, g, b;
	r = input->getColor()[0];
	g = input->getColor()[1];
	b = input->getColor()[2];
	
	// Make sure to re-set RenderDrawColor
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(gRenderer); // Fills the screen with the DrawColor
	//SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);
	
	//drawPrimitiveTiles();
	// Remember myBackground is a LTexture object
	myBackground.render(0, 0, gRenderer);
	
	// Set viewports. Viewports work with texture. Drawing primitive tiles using SDL_SetRenderXXXX does
	// not make use of any textures.
	//setViewport();
	
	//drawColorKeyExample();
	
	// Set foreground alpha
	myForeground.setAlpha(input->getAlpha());
	myForeground.setColor(r, g, b);
	
	//drawCreature(creature);
	myForeground.render(0, 0, gRenderer);
	
	SDL_RenderPresent(gRenderer);
}


/**
 * Loads an image into a SDL_Surface
 * Then converts to the screen display format
 */
SDL_Surface* Graphics::loadSurface(string filename)
{
	SDL_Surface* optimizedSurface = NULL;
	//SDL_Surface* loadedSurface = SDL_LoadBMP(filename.c_str());
	#ifdef __DEBUG_MODE__
	cout << "Loading surface . . ." << endl;
	#endif
	SDL_Surface* loadedSurface = IMG_Load(filename.c_str());
	
	if (loadedSurface == NULL)
	{
		cerr << "Surface could not be loaded" << endl;
		cerr << IMG_GetError();
	}
	else
	{
		SDL_Surface* surface = SDL_GetWindowSurface(window);
		optimizedSurface = SDL_ConvertSurface(loadedSurface, surface->format, 0);

		if (optimizedSurface == NULL)
		{
			cerr << "Surface could not be converted" << endl;
			cerr << SDL_GetError();
		}
		
		SDL_FreeSurface(loadedSurface);
	}
	
	#ifdef __DEBUG_MODE__
	cout << "Surface loaded." << endl;
	#endif
	
	return optimizedSurface;
}

/**
 * Loads a texture
 * Similar to loadSurface(), but returns a texture instead
 *
 *
 */
SDL_Texture* Graphics::loadTexture(string filename)
{
	SDL_Texture* newTexture = NULL;
	#ifdef __DEBUG_MODE__
	cout << "Loading texture . . . (" << filename << ")" << endl;
	#endif
	SDL_Surface* loadedSurface = IMG_Load(filename.c_str());

	if (loadedSurface == NULL)
	{
		cerr << "Surface could not be loaded" << endl;
		cerr << IMG_GetError();
	}
	else
	{
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		
		if (newTexture == NULL)
		{
			cerr << "Texture could not be created" << endl;
			cerr << SDL_GetError();
		}
		
		SDL_FreeSurface(loadedSurface);
	}
	
	#ifdef __DEBUG_MODE__
	cout << "Texture loaded." << endl;
	#endif
	return newTexture;
}

/**
 * Renders a tiled map made up of colored tiles using primitive rendering functions.
 */
void Graphics::drawPrimitiveTiles()
{
	int tiles_across;
	int tiles_down;
	int i, j;
	SDL_Rect tile_shape;

#ifdef __DEBUG_MODE__
	cout << "Drawing primitive tiles . . ." << endl;
	SDL_Delay(1000);
#endif
	
	tiles_across = SCREEN_WIDTH / TILESIZE;
	tiles_down = SCREEN_HEIGHT / TILESIZE;
	
	tile_shape.w = TILESIZE;
	tile_shape.h = TILESIZE;
	
	// #4C91A8
	//SDL_SetRenderDrawColor(gRenderer, 0x4C, 0x91, 0xA8, 0xFF);
	
	for (i = 0; i < tiles_down; i++)
	{
		tile_shape.y = i * TILESIZE;
		for (j = 0; j < tiles_across; j++)
		{
			SDL_SetRenderDrawColor(gRenderer, 0x4C, 0x91, 0xA8, 0xFF);
			tile_shape.x = j * TILESIZE;
			SDL_RenderFillRect(gRenderer, &tile_shape);
			
			// Add borders
			SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
			SDL_RenderDrawRect(gRenderer, &tile_shape);
		}
	}
}

// draw tiles based on gameMap
void Graphics::drawGameMap(GameMap* gameMap)
{
	int tiles_across;
	int tiles_down;
	int i, j;
	int spriteIndex;
	int tileIndex;
	Tileset* tileset;
	
	tileset = gameMap->getTileset();
	
#ifdef __DEBUG_MODE__
	// this is not good. Makes everything way too slow. (too many operations)
	//cout << "Drawing tiles from game map . . ." << endl;
	//SDL_Delay(1000);
#endif
	
	tiles_across = SCREEN_WIDTH / TILESIZE;
	tiles_down = SCREEN_HEIGHT / TILESIZE;
	
	
	for (i = 0; i < tiles_down; i++)
	{
		for (j = 0; j < tiles_across; j++)
		{
			tileIndex = i * tiles_across + j;
			spriteIndex = gameMap->getTile(tileIndex)->getId();
			tileSheet.render(j * TILESIZE, i * TILESIZE, gRenderer, tileset->getSprite(spriteIndex));
		}
	}
}

// draw tiles with the same id

void Graphics::drawTiles(Tileset* tileset, int index)
{
	int tiles_across;
	int tiles_down;
	int i, j;
	
#ifdef __DEBUG_MODE__
	//cout << "Drawing tiles . . ." << endl;
	//SDL_Delay(1000);
#endif
	
	tiles_across = SCREEN_WIDTH / TILESIZE;
	tiles_down = SCREEN_HEIGHT / TILESIZE;
	
	
	for (i = 0; i < tiles_down; i++)
	{
		for (j = 0; j < tiles_across; j++)
		{
			tileSheet.render(j * TILESIZE, i * TILESIZE, gRenderer, tileset->getSprite(index));
		}
	}
}

/**
 * Renders the creature on the screen
 */
void Graphics::drawCreature(Creature* c)
{
	SDL_Rect poseClip;
	
	/**
	 * Determines which part of the charset image to clip
	 * Note the player image has the width of a tile and the height of two tiles
	 */
	poseClip.x = c->getStep() * c->getWidth();
	poseClip.y = c->getDirection() * c->getHeight();
	poseClip.w = c->getWidth();
	poseClip.h = c->getHeight();
	
	/**
	 * TODO later: make charset class and generalize drawCreature() for any creature
	 */
	playerCharset.render(c->getPosX(), c->getPosY(), gRenderer, &poseClip);
}

/**
 * Renders the creature as a square on the screen
 */
void Graphics::drawSquareCreature(Creature* c)
{
	// why not just make a Creature::getPosRect() ??
	//int x, y;
	//int w, h;
	SDL_Rect rect;
	
	rect.x = c->getPosX();
	rect.y = c->getPosY();
	rect.w = c->getWidth();
	rect.h = c->getHeight();
	
	
	// we need this for the top image to blend and become transparent
	SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND);
	// tile: #CCCCCC, With some transparency (75%, 191)
	SDL_SetRenderDrawColor(gRenderer, 0xCC, 0xCC, 0xCC, 191);
	SDL_RenderFillRect(gRenderer, &rect);
	// set it back to NONE, just in case . . .
	SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_NONE);
	
	// Add borders
	SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
	SDL_RenderDrawRect(gRenderer, &rect);
}

/**
 * Renders the creature on the screen
 */
void Graphics::drawCursor(Cursor* c)
{
	// why not just make a Creature::getPosRect() ??
	//int x, y;
	//int w, h;
	SDL_Rect rect;
	
	rect.x = c->getPosX();
	rect.y = c->getPosY();
	rect.w = c->getWidth();
	rect.h = c->getHeight();
	
	
	// we need this for the top image to blend and become transparent
	SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND);
	// tile, with some transparency (25%, 63)
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 63);
	SDL_RenderFillRect(gRenderer, &rect);
	// set it back to NONE, just in case . . .
	SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_NONE);
	
	// Add borders
	SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
	SDL_RenderDrawRect(gRenderer, &rect);
}

void Graphics::setViewport()
{
	SDL_Rect rect;

	cout << "Setting viewports . . ." << endl;

	rect.x = 0;
	rect.y = 0;
	rect.w = SCREEN_WIDTH / 2;
	rect.h = SCREEN_HEIGHT / 2;

	// You need a texture to do viewport
	// Note rendering shapes using SDL_RenderXXXX will not use any texture, only the renderer
	gTexture = loadTexture("data/images/nen.png");

	// Set viewport, then render
	SDL_RenderSetViewport(gRenderer, &rect);
	SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);

	rect = (SDL_Rect) { SCREEN_WIDTH / 2, 0, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };

	SDL_RenderSetViewport(gRenderer, &rect);
	SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);

	rect = (SDL_Rect) { 0, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT / 2 };

	SDL_RenderSetViewport(gRenderer, &rect);
	SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);

	// Important to have this (to reset the viewport to whole screen)
	SDL_RenderSetViewport(gRenderer, NULL);
}

/**
 * Returns the renderer
 */
SDL_Renderer* Graphics::getRenderer()
{
	return gRenderer;
}

void Graphics::copyMapToBuffer() {
	buffer = SDL_ConvertSurface(buffer2, buffer2->format, SDL_SWSURFACE);
}
