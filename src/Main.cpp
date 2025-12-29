/** @file
 * Moving player game
 *
 * @author	nbkhope
 * @version	0.2
 */

#include "Main.h"

// 255
#define MAX_PACKET 0xFF

#include "SDL2/SDL_net.h"

void error(std::string functionName)
{
	std::cerr << functionName << " error: " << SDLNet_GetError() << std::endl;
}

SDLNet_SocketSet socketSet;
TCPsocket socket;
void networking() {
	std::cout << "Networking." << std::endl;
	IPaddress ip;
	if (SDLNet_ResolveHost(&ip, "localhost", 8099) == -1) {
		error("SDLNet_ResolveHost");
		return;
	}
	//todo: error
	//TCPsocket
	socket = SDLNet_TCP_Open(&ip);
	if (socket == NULL)
	{
		error("SDLNet_TCP_Open");
		return;
	}
	//todo: error
	//SDLNet_SocketSet
	socketSet = SDLNet_AllocSocketSet(1);
	if (socketSet == NULL)
	{
		error("SDLNet_AllocSocketSet");
		return;
	}
	
	if (SDLNet_TCP_AddSocket(socketSet, socket) == -1)
	{
		error("SDLNet_TCP_AddSocket");
		SDLNet_FreeSocketSet(socketSet);
		return;
	}
}
// Project includes for classes used directly in main()
#include "Graphics.h"  // Used for Graphics class
#include "Widget.h"    // Used for Widget class

using namespace std;

int main(int argc, char* args[])
{
	// Variable declarations
	System system;
	Graphics graphics;
	GameMap gameMap;
	Creature creature;
	//todo: Network class
	SDLNet_Init();
	//todo : error
	networking();

	Input input;
	int error;
	bool quit;
	int gameMode;

	//SDL_Event e;

	// Variable definitions
	//gameMode = GAME_MODE_TEST;
	gameMode = GAME_MODE_NORMAL;
	quit = false;

	error = system.init(&graphics);


	if(error > 0) // error
	{
		return error;
	}
	else // no errors
	{
		// Ready for the main thing after all initializing and loading

#ifndef __TEXTURE_RENDERING__
		graphics.displayImage();
#endif

		// Load Game Map
		gameMap.loadTxt("data/maps/map.txt");

		// Set up cursor
		input.getCursor()->updateTileInfo(&gameMap);

		// Create dialogue box widget at bottom of screen
		int dialogueHeight = SCREEN_HEIGHT / 4;  // 1/4 of screen height
		int dialogueWidth = SCREEN_WIDTH - 32;   // Full width with 16px margin on each side
		int dialogueX = 16;                      // 16px margin from left
		int dialogueY = SCREEN_HEIGHT - dialogueHeight - 16; // 16px margin from bottom
		Widget widget(dialogueX, dialogueY, dialogueWidth, dialogueHeight);
		widget.setTitle("Dialogue Box");
		
		/**
		 * Begins game loop
		 */
		while (!quit)
		{

			if (gameMode == GAME_MODE_TEST)
			{
				// Handle Input
				if (input.testGet(&gameMap, &creature))
				{
					quit = true;
				}

				graphics.testRender(&creature, &input);
			}
			else
			{
				// Handle Input
				if (input.get(&gameMap, &creature, socket))
				{
					quit = true;
				}

				SDLNet_CheckSockets(socketSet, 0);
				//todo: error
				if (SDLNet_SocketReady(socket))
				{
					char message[MAX_PACKET];
					int bytesReceived = SDLNet_TCP_Recv(socket, message, MAX_PACKET);

					if (bytesReceived <= 0)
					{
						// TCP Connection is broken. (because of error or closure)
						//todo: close socket and shutdown
						SDLNet_TCP_DelSocket(socketSet, socket);
						// todo: error
						SDLNet_FreeSocketSet(socketSet);
						SDLNet_TCP_Close(socket);
						quit = true;
					}
					else
					{
						// add null terminator to set the end of message
						message[bytesReceived] = 0;
						printf("Received: \"%s\"\n", message);

						if (strcmp(message, "move") == 0)
						{
							if (SDL_GetTicks() % 2 == 0)
							{
								creature.shiftPosX(32);
							}
							else
							{
								creature.shiftPosY(32);
							};


						}
					}
				};

				
				// Update widget visibility based on dialogue state
				widget.setVisible(input.getDialogueToggle());
				
#ifndef __TEXTURE_RENDERING__
				graphics.updateCurrentSurface();
#else
				graphics.render(&gameMap, &creature, &input, &widget);
#endif

			}

		}
		/**
		 * Game Loop End
		 */

		// Save Game Map
		gameMap.saveTxt("data/maps/map.txt");
	}

	//networking
	SDLNet_Quit();

	cout << "Finishing program. . ." << endl;

	return 0;
}
