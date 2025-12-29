/** @file
 * Moving player game
 *
 * @author	nbkhope
 * @version	0.2
 */

#include "Main.h"

// 255


#include "SDL2/SDL_net.h"
#include "SDL2/SDL_ttf.h"

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
#include "Sound.h"
#include "Monster.h"

using namespace std;

// Static variable to track movement direction for deterministic behavior
static int movementDirection = 0;

// Renders a single line of centered menu text; returns false if render fails
static bool renderMenuLine(SDL_Renderer* renderer, TTF_Font* font, const std::string& text, int y, SDL_Color color)
{
	SDL_Surface* textSurface = TTF_RenderText_Blended(font, text.c_str(), color);
	if (!textSurface)
	{
		std::cerr << "Menu text render failed: " << TTF_GetError() << std::endl;
		return false;
	}

	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
	if (!textTexture)
	{
		std::cerr << "Menu texture create failed: " << SDL_GetError() << std::endl;
		SDL_FreeSurface(textSurface);
		return false;
	}

	int textW = textSurface->w;
	int textH = textSurface->h;
	SDL_FreeSurface(textSurface);

	SDL_Rect dst = { (SCREEN_WIDTH - textW) / 2, y, textW, textH };
	SDL_RenderCopy(renderer, textTexture, nullptr, &dst);
	SDL_DestroyTexture(textTexture);
	return true;
}

// Settings submenu for resolution changes; returns false if user wants to go back
static void showSettingsMenu(Graphics& graphics)
{
	SDL_Renderer* renderer = graphics.getRenderer();
	if (!renderer)
	{
		return;
	}

	TTF_Font* font = TTF_OpenFont("data/fonts/cour.ttf", 24);
	if (!font)
	{
		std::cerr << "Could not load settings font: " << TTF_GetError() << std::endl;
		return;
	}

	SDL_Event e;
	bool backRequested = false;
	int selectedOption = 0;  // 0 = Normal (720x480), 1 = Large (1280x720)

	while (!backRequested)
	{
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
			{
				backRequested = true;
			}
			else if (e.type == SDL_KEYDOWN)
			{
				switch (e.key.keysym.sym)
				{
					case SDLK_UP:
						selectedOption = (selectedOption - 1 + 2) % 2;
						break;
					case SDLK_DOWN:
						selectedOption = (selectedOption + 1) % 2;
						break;
					case SDLK_RETURN:
					case SDLK_KP_ENTER:
						// Apply selected resolution
						if (selectedOption == 0)
						{
							graphics.resizeWindow(ResolutionPresets::WIDTH_NORMAL, ResolutionPresets::HEIGHT_NORMAL);
						}
						else
						{
							graphics.resizeWindow(ResolutionPresets::WIDTH_LARGE, ResolutionPresets::HEIGHT_LARGE);
						}
						break;
					case SDLK_ESCAPE:
						backRequested = true;
						break;
					default:
						break;
				}
			}
		}

		SDL_SetRenderDrawColor(renderer, 18, 20, 28, 255);
		SDL_RenderClear(renderer);

		SDL_Color titleColor = { 200, 230, 255, 255 };
		SDL_Color normalColor = { 210, 210, 210, 255 };
		SDL_Color selectedColor = { 255, 255, 100, 255 };

		renderMenuLine(renderer, font, "Settings - Resolution", 100, titleColor);
		
		std::string option0 = selectedOption == 0 ? "> Normal (720x480)" : "  Normal (720x480)";
		std::string option1 = selectedOption == 1 ? "> Large (1280x720)" : "  Large (1280x720)";
		
		renderMenuLine(renderer, font, option0, 180, selectedOption == 0 ? selectedColor : normalColor);
		renderMenuLine(renderer, font, option1, 220, selectedOption == 1 ? selectedColor : normalColor);
		
		renderMenuLine(renderer, font, "Press Enter to apply, Esc to go back", 300, normalColor);

		SDL_RenderPresent(renderer);
		SDL_Delay(16);
	}

	TTF_CloseFont(font);
}

// Simple blocking main menu loop; returns true if user chose to play, false if quit
static bool showMainMenu(Graphics& graphics)
{
	SDL_Renderer* renderer = graphics.getRenderer();
	if (!renderer)
	{
		// If renderer is not ready, skip menu and fall back to starting immediately
		return true;
	}

	TTF_Font* font = TTF_OpenFont("data/fonts/cour.ttf", 28);
	if (!font)
	{
		std::cerr << "Could not load menu font: " << TTF_GetError() << std::endl;
		return true;
	}

	Sound menuMusic;
	if (menuMusic.isReady() && !menuMusic.isPlaying())
	{
		menuMusic.play();
	}

	SDL_Event e;
	bool playRequested = false;
	bool quitRequested = false;
	int selectedOption = 0;  // 0 = Play, 1 = Settings, 2 = Quit

	while (!playRequested && !quitRequested)
	{
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
			{
				quitRequested = true;
			}
			else if (e.type == SDL_KEYDOWN)
			{
				switch (e.key.keysym.sym)
				{
					case SDLK_UP:
						selectedOption = (selectedOption - 1 + 3) % 3;
						break;
					case SDLK_DOWN:
						selectedOption = (selectedOption + 1) % 3;
						break;
					case SDLK_RETURN:
					case SDLK_KP_ENTER:
						if (selectedOption == 0)
						{
							playRequested = true;
						}
						else if (selectedOption == 1)
						{
							showSettingsMenu(graphics);
						}
						else if (selectedOption == 2)
						{
							quitRequested = true;
						}
						break;
					case SDLK_ESCAPE:
						quitRequested = true;
						break;
					default:
						break;
				}
			}
		}

		SDL_SetRenderDrawColor(renderer, 18, 20, 28, 255);
		SDL_RenderClear(renderer);

		// Simple menu layout
		SDL_Color titleColor = { 200, 230, 255, 255 };
		SDL_Color normalColor = { 210, 210, 210, 255 };
		SDL_Color selectedColor = { 255, 255, 100, 255 };
		
		renderMenuLine(renderer, font, "Ultimata", 120, titleColor);
		
		std::string playText = selectedOption == 0 ? "> Play (connect to server)" : "  Play (connect to server)";
		std::string settingsText = selectedOption == 1 ? "> Settings" : "  Settings";
		std::string quitText = selectedOption == 2 ? "> Quit" : "  Quit";
		
		renderMenuLine(renderer, font, playText, 200, selectedOption == 0 ? selectedColor : normalColor);
		renderMenuLine(renderer, font, settingsText, 240, selectedOption == 1 ? selectedColor : normalColor);
		renderMenuLine(renderer, font, quitText, 280, selectedOption == 2 ? selectedColor : normalColor);
		renderMenuLine(renderer, font, "Use Arrow Keys and Enter", 360, normalColor);

		SDL_RenderPresent(renderer);
		SDL_Delay(16); // ~60 FPS cap for the menu loop
	}

	if (menuMusic.isPlaying())
	{
		Mix_HaltMusic();
	}

	TTF_CloseFont(font);
	return playRequested;
}

int main(int argc, char* args[])
{
	// Variable declarations
	System system;
	Graphics graphics;
	GameMap gameMap;
	Creature creature;
	Monster monsters[MAX_MONSTERS];
	int monsterCount = 0;

	Input input;
	int error;
	bool quit;
	int gameMode;
	bool networkReady = false;

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

		// Show main menu before connecting to the server
		bool playSelected = showMainMenu(graphics);
		if (!playSelected)
		{
			std::cout << "Exited from main menu before connecting." << std::endl;
			return 0;
		}

		// Initialize networking only after the player opts in
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
		networkReady = true;

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
						if (bytesReceived >= sizeof(int))
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
							else if (command == NetworkCommands::MONSTER_UPDATE)
							{
								// Parse monster positions
								// Format: [command(4)][count(4)][x1(4)][y1(4)][x2(4)][y2(4)]...
								if (bytesReceived >= sizeof(int) * 2)
								{
									int* data = (int*)message;
									int count = data[1];
									monsterCount = (count < MAX_MONSTERS) ? count : MAX_MONSTERS;
									
									printf("Received %d monster positions\n", monsterCount);
									printf("Current monsterCount variable: %d\n", monsterCount);
									
									for (int i = 0; i < monsterCount; i++)
									{
										int x = data[2 + i * 2];
										int y = data[2 + i * 2 + 1];
										monsters[i].setPos(x, y);
										printf("Monster %d set to (%d, %d), size: %dx%d\n", i, x, y, monsters[i].getWidth(), monsters[i].getHeight());
									}
								}
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
				graphics.render(&gameMap, &creature, &input, &widget, monsters, monsterCount);
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
	if (networkReady)
	{
		SDLNet_Quit();
	}

	cout << "Finishing program. . ." << endl;

	return 0;
}
