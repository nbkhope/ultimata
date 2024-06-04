/**
 * Provides basic functions for using SDL. This file is not needed
 * if we have a game engine.
 */

#ifndef _SDL_H_
#define _SDL_H_

#include <string>

#include <SDL/SDL.h>
#include <SDL_image/SDL_image.h>
#include <SDL_ttf/SDL_ttf.h>
#include <SDL_mixer/SDL_mixer.h>

#include "constants.h"

using namespace std;

bool initialize();
bool terminateSDL();

SDL_Surface* loadImage(string filename);
void applySurface(int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL);

void setupScreen(SDL_Surface* &screen);

#endif
