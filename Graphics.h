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


#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#include <iostream>

#include "SDL2/SDL.h"
#include "SDL2_image/SDL_image.h"

#include "Constants.h"
#include "Creature.h"
#include "Input.h"


using namespace std;

class LTexture
{
public:
	LTexture();
	~LTexture();
	
	bool loadFromFile(string filename, SDL_Renderer* gRenderer);
	
	void free();
	void render(int x, int y, SDL_Renderer* gRenderer, SDL_Rect* clip = NULL);
	
	void setColor(Uint8 r, Uint8 g, Uint8 b);
	void setBlendMode(SDL_BlendMode blending);
	void setAlpha(Uint8 alpha);

	
	int getWidth() const;
	int getHeight() const;
private:
	SDL_Texture* mTexture;
	int mWidth;
	int mHeight;
};

class Graphics
{
public:
	Graphics();
	~Graphics();
	
	/**
	 * Loads the necessary graphical resources such as images, etc.
	 */
	bool loadMedia();
	/**
	 * Sets up the game window.
	 */
	void setUpWindow();
	
	void displayImage();
	void render(GameMap* gameMap, Creature* creature, Input* input);
	//void testRender(Creature* creature, Input* input);
	void testRender(Creature* creature, Input* input);
	
	/**
	 * Loads an image into an SDL_Surface, given path
	 */
	SDL_Surface* loadSurface(string filename);
	/**
	 * Loads an image into an SDL_Texture, given path
	 */
	SDL_Texture* loadTexture(string filename);
	
	/**
	 * Renders fixed-id tiles using primitive shape rendering functions
	 */
	void drawPrimitiveTiles();
	/**
	 * Renders the tiled game map based on data from a GameMap object.
	 */
	void drawGameMap(GameMap* gameMap);
	/**
	 * Renders fixed-id tiles using data from a Tileset object.
	 */
	void drawTiles(Tileset* tileset, int index);
	/**
	 * Renders the creature on the screen.
	 */
	void drawCreature(Creature* c);
	/**
	 * Renders the cursor
	 */
	void drawCursor(Cursor* c);
	void setViewport();
	void drawColorKeyExample();
	
	/**
	 * Returns the renderer
	 */
	SDL_Renderer* getRenderer();
	
	// Alpha blending <~test~>
	LTexture* getMyBackground();
	LTexture* getMyForeground();
	
private:
	SDL_Surface* screenSurface;
	SDL_Window* window;
	
	/**
	 * A renderer is necessary if working with textures
	 */
	SDL_Renderer* gRenderer;
	SDL_Texture* gTexture;
	
	SDL_Surface* image;
	//SDL_Texture* tileSheet;
	LTexture tileSheet;
	
	// Alpha blending <~test~>
	LTexture myBackground;
	LTexture myForeground;
	
	// Image Loading Different Types <~test~>
	SDL_Texture* image_jpg;
	SDL_Texture* image_tiff;
	SDL_Texture* image_bmp;
};

#endif
