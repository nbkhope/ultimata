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

#include "Graphics.h"

Graphics::Graphics()
{
	window = NULL;
	screenSurface = NULL;
	
	gRenderer = NULL;
	gTexture = NULL;

	image = NULL;
}

Graphics::~Graphics()
{
	SDL_FreeSurface(image);
	image = NULL;
	
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
}

bool Graphics::loadMedia()
{
	bool success;
	
	success = true;
	
	cout << "Loading media. . ." << endl;
	
#ifndef __TEXTURE_RENDERING__
	image = loadSurface("data/images/image.bmp");
	
#endif
	
	return success;
}

void Graphics::setUpWindow()
{
	window = SDL_CreateWindow("Ultimata 2",
							  SDL_WINDOWPOS_UNDEFINED,
							  SDL_WINDOWPOS_UNDEFINED,
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
		#ifndef __TEXTURE_RENDERING__
		// note: this surface will be freed when the window is destroyed
		// 		 (no need to free this later on)
		screenSurface = SDL_GetWindowSurface(window);
		
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

void Graphics::displayImage()
{
	cout << "Displaying image . . ." << endl;
	
	// Apply image
	SDL_BlitSurface(image, NULL, screenSurface, NULL);
	
	SDL_UpdateWindowSurface(window);
	
	SDL_Delay(2000);
}

void Graphics::render(Creature* creature)
{
	// Make sure to re-set RenderDrawColor
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(gRenderer); // Fills the screen with the DrawColor
	//SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);
	
	drawPrimitiveTiles();
	
	// Set viewports. Viewports work with texture. Drawing primitive tiles using SDL_SetRenderXXXX does
	// not make use of any textures.
	//setViewport();
	
	//drawColorKeyExample();

	drawCreature(creature);
	
	SDL_RenderPresent(gRenderer);
}

/**
 * Loads an image into a SDL_Surface
 * Then converts to the screen display format
 */
SDL_Surface* Graphics::loadSurface(string filename)
{
	SDL_Surface* optimizedSurface;
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
		optimizedSurface = SDL_ConvertSurface(loadedSurface, screenSurface->format, NULL);
		
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
 */
SDL_Texture* Graphics::loadTexture(string filename)
{
	SDL_Texture* newTexture = NULL;
	#ifdef __DEBUG_MODE__
	cout << "Loading texture . . ." << endl;
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

void Graphics::drawCreature(Creature* c)
{
	// why not just make a Creature::getPosRect() ??
	//int x, y;
	//int w, h;
	SDL_Rect rect;
	
	rect.x = c->getPosX();
	rect. y = c->getPosY();
	rect.w = c->getWidth();
	rect.h = c->getHeight();
	
	// tile: #CCCCCC, With some transparency
	SDL_SetRenderDrawColor(gRenderer, 0xCC, 0xCC, 0xCC, 0x80);
	SDL_RenderFillRect(gRenderer, &rect);
	
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
	
	rect = { SCREEN_WIDTH / 2, 0, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
	
	SDL_RenderSetViewport(gRenderer, &rect);
	SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);
	
	rect = { 0, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT / 2 };
	
	SDL_RenderSetViewport(gRenderer, &rect);
	SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);
	
	// Important to have this (to reset the viewport to whole screen)
	SDL_RenderSetViewport(gRenderer, NULL);
}

SDL_Renderer* Graphics::getRenderer()
{
	return gRenderer;
}

// LTexture Functions
LTexture::LTexture()
{
	mTexture = NULL;
	mWidth = NULL;
	mHeight = NULL;
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
	cout << "(LTexture) Loading texture . . ." << endl;
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
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));
		
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

void LTexture::render(int x, int y, SDL_Renderer* gRenderer)
{
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };
	
	SDL_RenderCopy(gRenderer, mTexture, NULL, &renderQuad);
}

int LTexture::getWidth() const
{
	return mWidth;
}

int LTexture::getHeight() const
{
	return mHeight;
}
