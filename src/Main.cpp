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

// NetworkClient class for proper resource management
class NetworkClient
{
private:
	SDLNet_SocketSet socketSet;
	TCPsocket socket;
	bool initialized;

public:
	NetworkClient() : socketSet(nullptr), socket(nullptr), initialized(false) {}
	
	~NetworkClient()
	{
		cleanup();
	}
	
	bool connect()
	{
		std::cout << "Networking." << std::endl;
		IPaddress ip;
		if (SDLNet_ResolveHost(&ip, "localhost", 8099) == -1) {
			std::cerr << "SDLNet_ResolveHost error: " << SDLNet_GetError() << std::endl;
			return false;
		}
		
		socket = SDLNet_TCP_Open(&ip);
		if (socket == NULL)
		{
			std::cerr << "SDLNet_TCP_Open error: " << SDLNet_GetError() << std::endl;
			return false;
		}
		
		socketSet = SDLNet_AllocSocketSet(1);
		if (socketSet == NULL)
		{
			std::cerr << "SDLNet_AllocSocketSet error: " << SDLNet_GetError() << std::endl;
			SDLNet_TCP_Close(socket);
			socket = nullptr;
			return false;
		}
		
		if (SDLNet_TCP_AddSocket(socketSet, socket) == -1)
		{
			std::cerr << "SDLNet_TCP_AddSocket error: " << SDLNet_GetError() << std::endl;
			cleanup();
			return false;
		}
		
		initialized = true;
		return true;
	}
	
	TCPsocket& getSocket() { return socket; }
	SDLNet_SocketSet getSocketSet() const { return socketSet; }
	bool isInitialized() const { return initialized; }
	
	void cleanup()
	{
		if (socket && socketSet)
		{
			SDLNet_TCP_DelSocket(socketSet, socket);
		}
		if (socketSet)
		{
			SDLNet_FreeSocketSet(socketSet);
			socketSet = nullptr;
		}
		if (socket)
		{
			SDLNet_TCP_Close(socket);
			socket = nullptr;
		}
		initialized = false;
	}
};

void networking() {
	// This function is now deprecated - use NetworkClient class instead
}
// Project includes for classes used directly in main()
#include "Graphics.h"  // Used for Graphics class
#include "Widget.h"    // Used for Widget class

using namespace std;

// Static variable to track movement direction for deterministic behavior
static int movementDirection = 0;

int main(int argc, char* args[])
{
	// Variable declarations
	System system;
	Graphics graphics;
	GameMap gameMap;
	Creature creature;
	//todo: Network class
	if (SDLNet_Init() == -1)
	{
		std::cerr << "SDLNet_Init failed: " << SDLNet_GetError() << std::endl;
		return 1;  // Exit with error code
	}
	std::cout << "SDLNet initialized successfully" << std::endl;
	
	NetworkClient networkClient;
	if (!networkClient.connect())
	{
		std::cerr << "Failed to initialize networking" << std::endl;
		return 2;
	}

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
				if (input.get(&gameMap, &creature, networkClient.getSocket()))
				{
					quit = true;
				}

				int socketCheckResult = SDLNet_CheckSockets(networkClient.getSocketSet(), 0);
				if (socketCheckResult == -1)
				{
					std::cerr << "SDLNet_CheckSockets error: " << SDLNet_GetError() << std::endl;
					quit = true;
				}
				else if (SDLNet_SocketReady(networkClient.getSocket()))
				{
					char message[MAX_PACKET];
					int bytesReceived = SDLNet_TCP_Recv(networkClient.getSocket(), message, MAX_PACKET);

					if (bytesReceived <= 0)
					{
						// TCP Connection is broken. (because of error or closure)
						networkClient.cleanup();
						quit = true;
					}
					else
					{
						printf("Received: %d bytes\n", bytesReceived);

						// Check if we received a 4-byte integer command
						if (bytesReceived == sizeof(int))
						{
							int command = *((int*)message);  // Cast received bytes to int
							printf("Received command: %d\n", command);

							if (command == NetworkCommands::MOVE)  // Use named constant
							{
								// Cycle through movement directions: 0=right, 1=down, 2=left, 3=up
								switch (movementDirection % 4)
								{
									case 0: // Move right
										creature.shiftPosX(32);
										break;
									case 1: // Move down
										creature.shiftPosY(32);
										break;
									case 2: // Move left
										creature.shiftPosX(-32);
										break;
									case 3: // Move up
										creature.shiftPosY(-32);
										break;
								}
								movementDirection++; // Cycle to next direction
							}
						}
						else
						{
							// Handle string messages (null terminate for safety)
							if (bytesReceived < MAX_PACKET_SIZE - 1)
							{
								message[bytesReceived] = 0;
								printf("Received text: \"%s\"\n", message);
								
								// Handle text-based commands here if needed
							}
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
