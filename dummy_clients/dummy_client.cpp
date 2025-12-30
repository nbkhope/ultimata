#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include "SDL2/SDL.h"
#include "SDL2/SDL_net.h"

// Network constants (matching the main game)
const int PLAYER_STATE_CMD = 6;

class DummyClient {
private:
    TCPsocket socket;
    std::string playerName;
    int x, y;
    int direction;
    bool connected;
    
public:
    DummyClient(const std::string& name, int startX, int startY) 
        : playerName(name), x(startX), y(startY), direction(0), connected(false), socket(nullptr) {
    }
    
    ~DummyClient() {
        if (socket) {
            SDLNet_TCP_Close(socket);
        }
    }
    
    bool connect() {
        IPaddress ip;
        if (SDLNet_ResolveHost(&ip, "localhost", 8099) == -1) {
            std::cerr << "SDLNet_ResolveHost error: " << SDLNet_GetError() << std::endl;
            return false;
        }
        
        socket = SDLNet_TCP_Open(&ip);
        if (socket == NULL) {
            std::cerr << "SDLNet_TCP_Open error: " << SDLNet_GetError() << std::endl;
            return false;
        }
        
        connected = true;
        std::cout << "Dummy client '" << playerName << "' connected to server" << std::endl;
        return true;
    }
    
    void sendPlayerState() {
        if (!connected || !socket) return;
        
        unsigned char packet[64];
        int offset = 0;
        
        // [command(4)][id(4)][x(4)][y(4)][direction(4)][nameLen(1)][name]
        int cmd = PLAYER_STATE_CMD;
        int id = 0; // Server will assign proper ID
        
        memcpy(packet + offset, &cmd, 4); offset += 4;
        memcpy(packet + offset, &id, 4); offset += 4;
        memcpy(packet + offset, &x, 4); offset += 4;
        memcpy(packet + offset, &y, 4); offset += 4;
        memcpy(packet + offset, &direction, 4); offset += 4;
        
        unsigned char nameLen = (unsigned char)std::min((int)playerName.length(), 32);
        packet[offset++] = nameLen;
        memcpy(packet + offset, playerName.c_str(), nameLen);
        
        int total = offset + nameLen;
        int bytesSent = SDLNet_TCP_Send(socket, packet, total);
        
        if (bytesSent < total) {
            std::cerr << "Failed to send player state for " << playerName << std::endl;
            connected = false;
        } else {
            std::cout << playerName << " sent position (" << x << ", " << y << ")" << std::endl;
        }
    }
    
    void simulateMovement() {
        // Simple random movement pattern
        int moveX = (rand() % 3 - 1) * 12; // -12, 0, or 12 pixels
        int moveY = (rand() % 3 - 1) * 12;
        
        x += moveX;
        y += moveY;
        
        // Keep within reasonable bounds
        if (x < 0) x = 0;
        if (y < 0) y = 0;
        if (x > 720) x = 720;
        if (y > 480) y = 480;
        
        // Update direction based on movement
        if (moveX > 0) direction = 2; // East
        else if (moveX < 0) direction = 3; // West
        else if (moveY > 0) direction = 0; // South
        else if (moveY < 0) direction = 1; // North
    }
    
    bool isConnected() const { return connected; }
};

int main(int argc, char* argv[]) {
    // Parse command line arguments
    std::string playerName = "DummyPlayer";
    int startX = 100;
    int startY = 100;
    int moveInterval = 2000; // milliseconds between moves
    
    if (argc >= 2) playerName = argv[1];
    if (argc >= 3) startX = std::atoi(argv[2]);
    if (argc >= 4) startY = std::atoi(argv[3]);
    if (argc >= 5) moveInterval = std::atoi(argv[4]);
    
    std::cout << "Starting dummy client: " << playerName << " at (" << startX << ", " << startY << ")" << std::endl;
    
    // Initialize SDL networking
    if (SDL_Init(0) == -1) {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << std::endl;
        return 1;
    }
    
    if (SDLNet_Init() == -1) {
        std::cerr << "SDLNet_Init failed: " << SDLNet_GetError() << std::endl;
        return 2;
    }
    
    srand(time(nullptr));
    
    // Create and connect dummy client
    DummyClient client(playerName, startX, startY);
    
    if (!client.connect()) {
        std::cerr << "Failed to connect to server" << std::endl;
        SDLNet_Quit();
        SDL_Quit();
        return 3;
    }
    
    // Send initial position
    client.sendPlayerState();
    
    // Main loop - send position updates periodically
    Uint32 lastMoveTime = SDL_GetTicks();
    
    std::cout << "Dummy client running. Press Ctrl+C to stop." << std::endl;
    
    while (client.isConnected()) {
        Uint32 currentTime = SDL_GetTicks();
        
        if (currentTime - lastMoveTime >= moveInterval) {
            client.simulateMovement();
            client.sendPlayerState();
            lastMoveTime = currentTime;
        }
        
        SDL_Delay(100); // Small delay to avoid busy waiting
    }
    
    std::cout << "Dummy client " << playerName << " disconnected" << std::endl;
    
    SDLNet_Quit();
    SDL_Quit();
    return 0;
}