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
	
	bool loadMedia();
	void setUpWindow();
	
	void displayImage();
	void render(GameMap* gameMap, Creature* creature, Input* input);
	//void testRender(Creature* creature, Input* input);
	void testRender(Creature* creature, Input* input);
	
	SDL_Surface* loadSurface(string filename);
	SDL_Texture* loadTexture(string filename);
	
	void drawPrimitiveTiles();
	void drawTiles(Tileset* tileset, int index);
	void drawCreature(Creature* c);
	void setViewport();
	void drawColorKeyExample();
	
	SDL_Renderer* getRenderer();
	
	// Alpha blending <~test~>
	LTexture* getMyBackground();
	LTexture* getMyForeground();
	
private:
	SDL_Surface* screenSurface;
	SDL_Window* window;
	
	SDL_Renderer* gRenderer;
	SDL_Texture* gTexture;
	
	SDL_Surface* image;
	//SDL_Texture* tileSheet;
	LTexture tileSheet;
	
	// Alpha blending <~test~>
	LTexture myBackground;
	LTexture myForeground;
};

#endif
