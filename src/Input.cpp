#include "Input.h"

Input::Input()
{
	// Alpha blending <~test~>
	alpha = 255;
	for (int i = 0; i < 3; i++)
		color[i] = 255;

	// Change tile <~test~>
	testTileId = 17;

	// Image Loading Different Types <~test~>
	testImageLoad = 0;
	
	// Dialogue system
	dialogueToggle = false;
	
	// Equipment overlay
	equipmentToggle = false;
	
	// Set initial cursor position
	cursor.setPosX(0);
	cursor.setPosY(0);
	//	cursor.setWidth(TILESIZE);
	//	cursor.setHeight(TILESIZE);
}

Input::~Input()
{

}

/**
 * Handles input for a non-scrolling, fixed-screen map.
 */
bool Input::get(GameMap* gameMap, Creature *c, TCPsocket& socket)
{
	int mouse_x, mouse_y;
	//int cursorTileIndex;
	//int cursorTileId;
	//int tileId;
	bool quit = false; // Using this to send quit signal back, for now . . . at least.

//	cursorTileIndex = cursor.getPosX() / TILESIZE + (cursor.getPosY() / TILESIZE) * gameMap->getTilesAcross();
//	cursorTileId = gameMap->getTile(cursorTileIndex)->getId();

	// For a complete list of keyboard enum, check:
	// http://wiki.libsdl.org/SDL_Keycode?highlight=%28\bCategoryEnum\b%29|%28CategoryKeyboard%29
	// https://www.libsdl.org/release/SDL-1.2.15/docs/html/sdlkey.html

	// Handle events on queue
	// Keeps doing while there are still events left
	while (SDL_PollEvent(&e))
	{
		/**
		 * Note: checkPlayerMovement() checks e.type for SDL_KEYDOWN and SDL_KEYUP
		 */
		checkPlayerMovement(gameMap, c, socket);
		// issue here ... ( don't want to do switch check again)

		if (e.type == SDL_QUIT)
		{
			cout << "SDL_QUIT event!" << endl;
			quit = true;

			// position changed
			//todo: throttling. this is called many times due to speed...
			char message[MAX_MESSAGE_SIZE];
			strncpy(message, NetworkMessages::DISCONNECT, MAX_MESSAGE_SIZE - 1);
			message[MAX_MESSAGE_SIZE - 1] = '\0';  // Ensure null termination
			int len = strlen(message);
			// int len = sprintf(message, "player position is (%d, %d)", c->getPosX(), c->getPosY());
			std::cout << message << std::endl;
			//note terminating null (+1)...
			int bytesSent = SDLNet_TCP_Send(socket, message, len);
			if (bytesSent != len)  // Check if exactly the expected number of bytes were sent
			{
				//todo: use error lib
				std::cerr << "Network send failed: " << SDLNet_GetError() << std::endl;
			}
			//todo: error sprintf
		}
		else if (e.type == SDL_KEYDOWN)
		{
			//cout << "SDL_KEYDOWN event!" << endl;

			switch (e.key.keysym.sym)
			{
				case SDLK_q: // quit
					quit = true;
					break;

				case SDLK_t: // change tile <~test~>
					if (testTileId < 256)
						testTileId++;
					else
						testTileId = 0;
					cout << "testTileId = " << testTileId << endl;
					break;

				case SDLK_y:
					if (testTileId < 256)
						testTileId += 16;
					else
						testTileId = 0;
					cout << "testTileId = " << testTileId << endl;
					break;
				case SDLK_TAB: // Image Loading Different Types <~test~>
					if (testImageLoad < 3)
						testImageLoad++;
					else
						testImageLoad = 0;
					cout << "testImageLoad = " << testImageLoad << endl;
					break;
				case SDLK_LEFTBRACKET:
					if (cursor.getCursorTileId() != 0)
					{
						//tileId = gameMap->getTile(cursor.getCursorTileIndex())->getId();
						//gameMap->getTile(cursor.getCursorTileIndex())->setId(tileId - 1);
						gameMap->getTile(cursor.getCursorTileIndex())->decrementId();
					}
					break;
				case SDLK_RIGHTBRACKET:
					if (cursor.getCursorTileId() != 255)
					{
						//tileId = gameMap->getTile(cursor.getCursorTileIndex())->getId();
						//gameMap->getTile(cursor.getCursorTileIndex())->setId(tileId + 1);
						gameMap->getTile(cursor.getCursorTileIndex())->incrementId();
					}
					break;

				/**
				 * Deals with timer
				 */
				case SDLK_PERIOD: // start/stop timer
					if (timer.isOn() && !timer.isPaused())
						timer.stop();
					else
						timer.start();
					break;
				case SDLK_COMMA: // pause/unpause timer
					if (timer.isPaused())
						timer.start();
					else
						timer.pause();
					break;
				case SDLK_SLASH: // reset timer
					timer.reset();
					break;
				case SDLK_SPACE: // toggle dialogue box
					dialogueToggle = !dialogueToggle;
					cout << "Dialogue " << (dialogueToggle ? "shown" : "hidden") << endl;
					break;
				case SDLK_e: // toggle equipment overlay
					equipmentToggle = !equipmentToggle;
					cout << "Equipment overlay " << (equipmentToggle ? "shown" : "hidden") << endl;
					break;
				default:
					// do nothing
					break;
			}

		}
		else if (e.type == SDL_MOUSEBUTTONDOWN) // buggy
		{
			//cout << "SDL_MOUSEBUTTONDOWN event!" << endl;

			SDL_GetMouseState(&mouse_x, &mouse_y);
			//switch (e.button.button == SDL_BUTTON_LEFT)
			//{
				/*	c->setPosX(mouse_x - mouse_x % TILESIZE);
					c->setPosY(mouse_y - mouse_y % TILESIZE); */
			//}

			// Adjust cursor region
			cursor.setPosX(mouse_x - mouse_x % TILESIZE);
			cursor.setPosY(mouse_y - mouse_y % TILESIZE);
			cursor.updateTileInfo(gameMap);
		}
	}

	return quit;
}

/**
 * Same as Input::get(), but for testing purposes.
 * Handles input for a non-scrolling, fixed-screen map.
 */
bool Input::testGet(GameMap* gameMap, Creature *c)
{
	int pos_x, pos_y;
	int mouse_x, mouse_y;
	bool quit = false; // Using this to send quit signal back, for now . . . at least.

	pos_x = c->getPosX();
	pos_y = c->getPosY();

	// For a complete list of keyboard enum, check:
	// http://wiki.libsdl.org/SDL_Keycode?highlight=%28\bCategoryEnum\b%29|%28CategoryKeyboard%29
	// https://www.libsdl.org/release/SDL-1.2.15/docs/html/sdlkey.html

	// Handle events on queue
	// Keeps doing while there are still events left
	while (SDL_PollEvent(&e) != 0)
	{
		if (e.type == SDL_QUIT)
		{
			cout << "SDL_QUIT event!" << endl;
			quit = true;
		}
		else if (e.type == SDL_KEYDOWN)
		{
			cout << "SDL_KEYDOWN event!" << endl;

			switch (e.key.keysym.sym)
			{
				case SDLK_q: // quit
					quit = true;
					break;

				// Alpha blending <~test~>
				case SDLK_o: // increase alpha
					if (alpha + 32 < 255)
						alpha += 32;
					else
						alpha = 255;
					cout << "Alpha set to " << int(alpha) << endl; // we need to convert Uint8 to int
					break;
				case SDLK_p: // decrease alpha
					if (alpha - 32 > 0)
						alpha -= 32;
					else
						alpha = 0;
					cout << "Alpha set to " << int(alpha) << endl;
					break;
				case SDLK_j: // decrease R color
					if (color[0] - 32 > 0)
						color[0] -= 32;
					else
						color[0] = 0;
					break;
				case SDLK_k: // decrease G color
					if (color[1] - 32 > 0)
						color[1] -= 32;
					else
						color[1] = 0;
					break;
				case SDLK_l: // decrease B color
					if (color[2] - 32 > 0)
						color[2] -= 32;
					else
						color[2] = 0;
					break;
				case SDLK_SEMICOLON: // reset color modulation
					for (int i = 0; i < 3; i++)
						color[i] = 255;
					break;
				default:
					// do nothing
					break;
			}
			cout << "Texture color: (" << int(color[0]) << ", " << int(color[1]) << ", "
				 << int(color[2]) << ")" << endl;
 		}
		else if (e.type == SDL_MOUSEBUTTONDOWN) // buggy
		{
			cout << "SDL_MOUSEBUTTONDOWN event!" << endl;

			SDL_GetMouseState( &mouse_x, &mouse_y );
			//switch (e.button.button == SDL_BUTTON_LEFT)
			//{
			c->setPosX(mouse_x - mouse_x % TILESIZE);
			c->setPosY(mouse_y - mouse_y % TILESIZE);
			//}
		}
	}

	return quit;
}

// Alpha blending <~test~>
Uint8 Input::getAlpha() const
{
	return alpha;
}

void Input::setAlpha(Uint8 a)
{
	alpha = a;
}

Uint8* Input::getColor()
{
	return color;
}

void Input::setColor(Uint8* newColor)
{
	color[0] = newColor[0];
	color[1] = newColor[1];
	color[2] = newColor[2];
}

int Input::getTestTileId() const
{
	return testTileId;
}

int Input::getTestImageLoad() const
{
	return testImageLoad;
}

Cursor* Input::getCursor()
{
	return &cursor;
}

Timer* Input::getTimer()
{
	return &timer;
}

/**
 * For free movement, will increase/decrease the creature movement speed accordingly.
 * Then, the creature's position will be changed.
 *
 * @param	gameMap	the game map object
 * @param c	the creature object
 */
void Input::checkPlayerMovement(GameMap* gameMap, Creature* c, TCPsocket& socket)
{
	int pos_x, pos_y;
	int xspeed, yspeed;
	int direction;

	pos_x = c->getPosX();
	pos_y = c->getPosY();
	xspeed = c->getSpeedX();
	yspeed = c->getSpeedY();
	direction = c->getDirection();

	if (e.type == SDL_KEYDOWN)
	{
		// Which key was pressed down?
		switch (e.key.keysym.sym)
		{
	#ifdef __TILE_MOVEMENT__
			case SDLK_UP:
				/**
				 * Note: if the player image is higher than a tile, you have to make sure he can
				 * still go up to the first row of tiles, where y = 0.
				 *
				 * The player image position is determine from the top left. If a player is tall,
				 * then he will still be able to move to the first horizontal row of tiles, as
				 * shown below by allowing the y index to go negative. Only the lower body of the player
				 * image will be displayed. Question: when graphics renders the player image,
				 * would the negative position be an issue? (out of bounds)
				 * Answer: so far, no problems. It works.
				 */
				if (pos_y != TILESIZE - c->getHeight()) // if not trying to go out of bounds
				{
					c->setPosY(pos_y - TILESIZE);
					c->setDirection(DIRECTION_NORTH);
				}
				else
					cout << "SDLK_UP: Attempt to go out of bounds!!" << endl;
				break;
			case SDLK_DOWN:
				/**
				 * Note: if the player image is higher than a tile, you have to make sure he canno
				 * go down beyond the map boundaries in the last horizontal row of tiles,
				 * where y = MAP_HEIGHT - TILESIZE.
				 */
				if (pos_y != gameMap->getHeight() - c->getHeight()) // if not trying to go out of bounds
				{
					c->setPosY(pos_y + TILESIZE);
					c->setDirection(DIRECTION_SOUTH);
				}
				else
					cout << "SDLK_DOWN: Attempt to go out of bounds!!" << endl;
				break;
			case SDLK_LEFT:
				if (pos_x != 0) // if not trying to go out of bounds
				{
					c->setPosX(pos_x - TILESIZE);
					c->setDirection(DIRECTION_WEST);
				}
				else
					cout << "SDLK_LEFT: Attempt to go out of bounds!!" << endl;
				break;
			case SDLK_RIGHT:
				if (pos_x != gameMap->getWidth() - c->getWidth()) // if not trying to go out of bounds
				{
					c->setPosX(pos_x + TILESIZE);
					c->setDirection(DIRECTION_EAST);
				}
				else
					cout << "SDLK_RIGHT: Attempt to go out of bounds!!" << endl;
				break;

	#else // Free Movement
			case SDLK_UP:
				if (yspeed == 0)
					yspeed = -PLAYER_MOVE_SPEED;
				if (direction != DIRECTION_NORTH) // Do we really need this if statement?
				{
					c->setDirection(DIRECTION_NORTH);
				}
				break;
			case SDLK_DOWN:
				if (yspeed == 0)
					yspeed = PLAYER_MOVE_SPEED;
				if (direction != DIRECTION_SOUTH)
				{
					c->setDirection(DIRECTION_SOUTH);
				}
				break;
			case SDLK_LEFT:
				if (xspeed == 0)
					xspeed = -PLAYER_MOVE_SPEED;
				if (direction != DIRECTION_WEST) // if not trying to go out of bounds
				{
					c->setDirection(DIRECTION_WEST);
				}
				break;
			case SDLK_RIGHT:
				if (xspeed == 0)
					xspeed = PLAYER_MOVE_SPEED;
				if (direction != DIRECTION_EAST) // if not trying to go out of bounds
				{
					c->setDirection(DIRECTION_EAST);
				}
				break;
	#endif
		}
	}
	#ifndef __TILE_MOVEMENT__
	else if (e.type == SDL_KEYUP)
	{
		/**
		 * Which key was released?
		 * Depending on the key, decrease player speed
		 */
		switch (e.key.keysym.sym) {
			case SDLK_UP:
				yspeed = 0;
				break;
			case SDLK_DOWN:
				yspeed = 0;
				break;
			case SDLK_RIGHT:
				xspeed = 0;
				break;
			case SDLK_LEFT:
				xspeed = 0;
				break;
		}
	}

	if ((pos_x == 0 && xspeed < 0) || (pos_x == gameMap->getWidth() - c->getWidth() && xspeed > 0))
		// don't allow him to go out of bounds
		xspeed = 0;
	else if (xspeed != 0)
		c->shiftPosX(xspeed);

	if ((pos_y == TILESIZE - c->getHeight() && yspeed < 0) || (pos_y == gameMap->getHeight() - c->getHeight() && yspeed > 0))
		yspeed = 0;
	else if (yspeed != 0)
		c->shiftPosY(yspeed);

	// Finally updates player speed
	c->setSpeed(xspeed, yspeed);

	#endif /** Type of Movement **/

	/**
	 * Handles animation
	 */
	if (xspeed != 0 || yspeed != 0)
		c->shiftStep();

	//networking
	//networking stuff
	if (pos_x != c->getPosX() || pos_y != c->getPosY())
	{
		// position changed
		//todo: throttling. this is called many times due to speed...
		char message[MAX_MESSAGE_SIZE];
		int len = snprintf(message, MAX_MESSAGE_SIZE, "player position is (%d, %d)", c->getPosX(), c->getPosY());
		std::cout << message << std::endl;
		//note terminating null (+1)...
		int bytesSent = SDLNet_TCP_Send(socket, message, len + 1);
		if (bytesSent != len + 1)  // Check if all bytes including null terminator were sent
		{
			//todo: use error lib
			std::cerr << "Network send failed: " << SDLNet_GetError() << std::endl;
		}
		//todo: error sprintf
	}

}

/*
void Input::movePlayer(Creature* c)
{
	int pos_x, pos_y;
	int xspeed, yspeed;
	int direction;
	pos_x = c->getPosX();
	pos_y = c->getPosY();
	xspeed = c->getSpeedX();
	yspeed = c->getSpeedY();
	direction = c->getDirection();

	if (xspeed != 0 || yspeed != 0)
	{
		if (!newDirection)
		{
			if (c->getStep() != 0)
			{
				c->shiftStep();
			}
			else
			{
				c->setStep(1);
			}
		}
		else
			c->setNewDirection(false);
	}
}
 */

/*void Input::get()
{
	// Go through all the events, one by one
	while (SDL_PollEvent(&event))
	{
		//if (event.type == SDL_QUIT)
		//	quit = true;
		if (event.type == SDL_KEYDOWN) // A key was pressed
		{
			switch (event.key.keysym.sym)  // Fixed: SDL2 uses 'sym' not 'key'
			{
				default:
			}
		}
	}
}
*/

bool Input::getDialogueToggle() const
{
	return dialogueToggle;
}

void Input::setDialogueToggle(bool toggle)
{
	dialogueToggle = toggle;
}

bool Input::getEquipmentToggle() const
{
	return equipmentToggle;
}

void Input::setEquipmentToggle(bool toggle)
{
	equipmentToggle = toggle;
}
