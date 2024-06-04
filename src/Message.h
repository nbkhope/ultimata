#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#include <iostream>
#include <string>
#include <sstream>
#include <stack>

#include <SDL/SDL.h>
#include "sdl.h"

#include "Graphics.h"

using namespace std;

class Message
{
public:
	Message();
	~Message();

	// why not string& ?
	void displayLine(int x, int y, string msg, Graphics& graphics);

	void display(stringstream& msg, SDL_Surface* surface);

	void displayTopLeft(stringstream& msg, Graphics& graphics);
	void displayBottomLeft(stringstream& msg, Graphics& graphics);
	void displayBottomCenter(stringstream& msg, Graphics& graphics);

	void display(stringstream& msg, SDL_Surface* surface, int x, int y);


private:
	TTF_Font* font;
	SDL_Color color;
	SDL_Surface* textsurface;
};

#endif
