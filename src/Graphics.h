/**
 * Graphics core
 * Responsible for setting up the buffer and provides functions to 
 * manipulate graphics.
 * @file
 */

#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#include <iostream>

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

#include "Constants.h"
#include "Creature.h"
#include "Input.h"

#include <iostream>
#include <string>
#include "Map.h"

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

/**
 * The Graphics class deals with everything related 
 * to loading, rendering, and displaying images.
 */
class Graphics
{
public:
	/**
	 * Constructor and destructor
	 */
	Graphics();
	~Graphics();
	
	/**
	 * Sets up the game window.
	 */
	void setUpWindow();
	
	/**
	 * Loads the necessary graphical resources such as images, etc.
	 */
	bool loadMedia();
	
	void displayImage();
	void render(GameMap* gameMap, Creature* creature, Input* input);
	//void testRender(Creature* creature, Input* input);
	void testRender(Creature* creature, Input* input);
	
	/**
	 * Loads an image into an SDL_Surface, given path
	 *
	 * @param	filename	the name of the image to be loaded
	 * @return	a pointer to the loaded surface
	 */
	SDL_Surface* loadSurface(string filename);
	/**
	 * Loads an image into an SDL_Texture, given path
	 *
	 * @param	filename	the name of the image to be loaded
	 * @return	a pointer to the loaded surface
	 */
	SDL_Texture* loadTexture(string filename);
	
	/**
	 * Renders fixed-id tiles using primitive shape rendering functions
	 */
	void drawPrimitiveTiles();
	/**
	 * Renders the tiled game map based on data from a GameMap object.
	 *
	 * @param	gameMap	a pointer to a GameMap object
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
	void drawSquareCreature(Creature* c);
	/**
	 * Renders the cursor
	 */
	void drawCursor(Cursor* c);
	void setViewport();
	void drawColorKeyExample();
	
	/**
	 * @return the renderer
	 */
	SDL_Renderer* getRenderer();
	
	// Alpha blending <~test~>
	LTexture* getMyBackground();
	LTexture* getMyForeground();

	bool init();
	bool isRunning() const { return running; }

	void drawToBuffer(int x, int y, SDL_Surface* source, SDL_Rect* clip = NULL);
	void drawMapToBuffer(int x, int y, SDL_Rect* clip = NULL);
	void displayBuffer();
	
	void drawMap(Map& map);
	void copyMapToBuffer();

	SDL_Surface* getBuffer() { return buffer; };
	SDL_Surface* getGameLogo() const { return logo; } ;


	SDL_Surface* loadImage(string filename);

	// void setColorKey();
	// resize
	
private:
	SDL_Surface* screenSurface;
	/**
	 * The game window
	 */
	SDL_Window* window;
	
	/**
	 * A renderer is necessary if working with textures
	 */
	SDL_Renderer* gRenderer;
	SDL_Texture* gTexture;
	
	SDL_Surface* image;
	SDL_Surface* gameIcon;
	SDL_Surface* gameLogo;
	//SDL_Texture* tileSheet;
	/**
	 * An image with all the tiles necessary to draw a map.
	 */
	LTexture tileSheet;
	
	// Alpha blending <~test~>
	LTexture myBackground;
	LTexture myForeground;
	
	// Image Loading Different Types <~test~>
	SDL_Texture* image_jpg;
	SDL_Texture* image_tiff;
	SDL_Texture* image_bmp;
	
	LTexture playerCharset;

	bool running;

	// This will be changed as the game goes
	SDL_Surface* buffer;
	// This is a copy of the buffer to keep the map surface
	SDL_Surface* buffer2;
	//SDL_Surface* map_buffer;

	SDL_Surface* logo;
};

#endif
