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
	};
	//todo: error
	//TCPsocket
	socket = SDLNet_TCP_Open(&ip);
	if (socket == NULL)
	{
		error("SDLNet_TCP_Open");
	}
	//todo: error
	//SDLNet_SocketSet
	socketSet = SDLNet_AllocSocketSet(1);
	//todo: error
	SDLNet_TCP_AddSocket(socketSet, socket);
}

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

#ifndef __TEXTURE_RENDERING__
				graphics.updateCurrentSurface();
#else
				graphics.render(&gameMap, &creature, &input);
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
