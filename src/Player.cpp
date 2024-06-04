#include "Player.h"

#include <string>
#include <sstream>

using namespace std;

Player::Player() : Creature()
{
/*
	name = "Unknown";
	health = healthmax = 50;
	
	pos.x = 0;
	pos.y = 0;
	pos.w = TILESIZE;
	pos.h = TILESIZE;
	direction = SOUTH;
	xspeed = yspeed = 0;

	charset = NULL;
	charset = loadImage("charset.png");*/
	stats = false;
}

Player::~Player()
{
    // you don't need this because the destructor for Creature already does it
	//SDL_FreeSurface(charset);
}

void Player::checkMovement(SDL_Event &event)
{
    // xcode fix for now
//#define __STIFFTILEMOVE__
#ifdef __STIFFTILEMOVE__
	// The code below allows the player to move one tile for each
	// key pressed & released. Note keeping the key pressed
	// will not make it move in constant speed.

	// Handles a key pressed event
	if (event.type == SDL_KEYDOWN)
	{
		switch(event.key.keysym.sym)
		{
			case SDLK_UP:
				if (pos.y != 0)
					pos.y -= 32;
				direction = NORTH;
				break;
			case SDLK_DOWN:
				if (pos.y != SCREEN_HEIGHT - 64)
					pos.y += 32;
				direction = SOUTH;
				break;
			case SDLK_LEFT:
				if (pos.x != 0)
					pos.x -= 32;
				direction = WEST;
				break;
			case SDLK_RIGHT:
				if (pos.x != SCREEN_WIDTH - 32)
					pos.x += 32;
				direction = EAST;
				break;
		}
	}
	//else if (event.type == SDL_KEYUP)
#else

	// The code below allows the player to move at constant speed when
	// having a movement key pressed. Diagonal movement also becomes available.
	// Note that by the time we release a key, a bunch of frames will have
	// been rendered and so the player would have moved several tiles.

	// I replaced TILESIZE movements with PLAYER_MOVEMENT,
	// causing the character to move a little less, but this
	// causes them to not stay aligned with the map tiles.
	if (event.type == SDL_KEYDOWN)
	{
		switch (event.key.keysym.sym)
		{
			case SDLK_UP: // Recall going up means to decrease the y-axis
				yspeed -= PLAYER_MOVEMENT;
				if (direction != NORTH)
				{
					direction = NORTH;
					step = 0;
					newdirection = true;
				}
				break;
			case SDLK_DOWN:
				if (direction != SOUTH)
				{
					direction = SOUTH;
					step = 0;
					newdirection = true;
				}
				yspeed += PLAYER_MOVEMENT;

		
				break;

			case SDLK_RIGHT:
				if (direction != EAST)
				{
					direction = EAST;
					step = 0;
					newdirection = true;
				}
				xspeed += PLAYER_MOVEMENT;
		
				break;

			case SDLK_LEFT:
				xspeed -= PLAYER_MOVEMENT;
				if (direction != WEST)
				{
					direction = WEST;
					step = 0;
					newdirection = true;
				}

		
				break;

			case SDLK_b:
				stats = !stats;
				break;
			//default:
				//
		}
		// will do if ANY key is pressed
		// How to make character remain shifting steps when key is still pressed down?
		//if (xspeed != 0 || yspeed != 0) // moving
/*		if(!newdirection)
		{
			if (step != 0)
			{
				step = -step;
			}
			else
			{
				step = 1;
			}
//		}
//		else
			newdirection = false;*/

	}
	else if (event.type == SDL_KEYUP)
	{
		switch (event.key.keysym.sym)
		{
			case SDLK_UP:
				yspeed += PLAYER_MOVEMENT;
//				direction = NORTH;
				break;
			case SDLK_DOWN:
				yspeed -= PLAYER_MOVEMENT;
//				direction = SOUTH;
				break;
			case SDLK_RIGHT:
				xspeed -= PLAYER_MOVEMENT;
//				direction = EAST;
				break;
			case SDLK_LEFT:
				xspeed += PLAYER_MOVEMENT;
//				direction = WEST;
				break;
		}
	}

#endif

}
/*
void Player::move(Map &map) //, SDL_Rect &block)
{
	pos.x += xspeed;

//	if (pos.x < 0 || pos.x > (SCREEN_WIDTH - pos.w) || hasCollided(pos, block) )
	if (pos.x < 0 || pos.x > (map.getWidth() - pos.w))// || hasCollided(pos, block) )
		pos.x -= xspeed;

	pos.y += yspeed;
	
///	if (pos.y < 0 || pos.y > (SCREEN_HEIGHT - pos.h) || hasCollided(pos, block) )
	if (pos.y < 0 || pos.y > (map.getHeight() - pos.h))// || hasCollided(pos, block) )
		pos.y -= yspeed;

	// problem: shifting steps too fast
	if (xspeed != 0 || yspeed != 0)
		if(!newdirection)
		{
			if (step != 0)
			{
				step = -step;
			}
			else
			{
				step = 1;
			}
		}
		else
			newdirection = false;
}
*/
void Player::setStats()
{
	stats = !stats;
}

// Use this code as an example to create a font/msg system
void Player::displayStats(Message* message, SDL_Surface* surface) const
{
	TTF_Font* font;
	SDL_Surface* txt;
	stringstream ss;
	string sentence;

	int lines = 0;

	ss << "HP: " << health << "/" << healthmax << endl;
	ss << "MP: " << mana << "/" << manamax << endl;
	ss << "Attack: " << attack << " \nDefense: " << defense << endl;

	message->display(ss, surface);
/*
	font = TTF_OpenFont(DEFAULT_FONT.c_str(), FONTSIZE - 4);

	SDL_Color txtcolor = { 0, 0, 0 }; // black

	for (int i = 0; i < ss.str().length(); i++)
		if (ss.str()[i] == '\n')
			lines++;

	for (int i = lines; i > 0; i--)
	{
		getline(ss, sentence);
		txt = TTF_RenderText_Solid(font, sentence.c_str(), txtcolor);
		applySurface(4, SCREEN_HEIGHT - i * txt->h - 32, txt, surface);
	}

	
	TTF_CloseFont(font);
	SDL_FreeSurface(txt);
*/
}


