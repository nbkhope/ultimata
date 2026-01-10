#include <csignal>
#include <cstring>
#include <iostream>
#include <sstream>
#include <memory>

#include "MessageProtocol.h"
#include "SDL2/SDL.h" // Still needed for SDL_Delay
#include "AsioNetworkManager.h"

// Server-local networking constants
const int SERVER_MAX_PACKET = 0xFF; // 255-byte packet size for server buffers
const int MAX_SOCKETS = 0x10;        // 16 maximum connections

// Message buffer constants
const int MESSAGE_BUFFER_SIZE = SERVER_MAX_PACKET;       
const int MESSAGE_RECV_SIZE = MESSAGE_BUFFER_SIZE - 1;   

struct Position {
    int x;
    int y;
};

struct PlayerState {
    bool active;
    int id;
    int x, y;
    int direction;
    char name[33]; // MAX_PLAYER_NAME+1
};

// Global state
PlayerState playerStates[MAX_SOCKETS];
std::unique_ptr<NetworkManager> g_network;
int playersOnline = 0;
bool shutdownRequested = false;

// Monster system
const int MAX_MONSTERS_SERVER = 4;
struct Monster {
    bool active;
    Position position;
};
Monster monsters[MAX_MONSTERS_SERVER];

// Forward declarations
void runServerLoop();
void processClientMessages();
void handleClientMessage(int clientId, const unsigned char* buffer, int size);
void initializeMonsters();
void registerSignalHandlers();
void sigintHandler(int signum);

// Logging functions
void error(std::string message) {
    std::cerr << message << std::endl;
}

void info(std::string message) {
    std::cout << message << std::endl;
}

// Helper: broadcast all player states to all clients
void broadcastPlayerStates() {
    for (int j = 0; j < MAX_SOCKETS; ++j) {
        if (playerStates[j].active) {
            unsigned char packet[64];
            int offset = 0;
            int cmd = 6; // PLAYER_STATE
            memcpy(packet + offset, &cmd, 4); offset += 4;
            memcpy(packet + offset, &playerStates[j].id, 4); offset += 4;
            memcpy(packet + offset, &playerStates[j].x, 4); offset += 4;
            memcpy(packet + offset, &playerStates[j].y, 4); offset += 4;
            memcpy(packet + offset, &playerStates[j].direction, 4); offset += 4;
            unsigned char nameLen = (unsigned char)strnlen(playerStates[j].name, 32);
            packet[offset++] = nameLen;
            memcpy(packet + offset, playerStates[j].name, nameLen);
            int total = offset + nameLen;
            g_network->broadcastData(packet, total);
        }
    }
}

void initializeMonsters() {
    // Spawn 2 monsters at fixed positions
    monsters[0].active = true;
    monsters[0].position.x = 200;
    monsters[0].position.y = 150;
    
    monsters[1].active = true;
    monsters[1].position.x = 400;
    monsters[1].position.y = 300;
    
    monsters[2].active = false;
    monsters[3].active = false;
    
    info("Monsters initialized.");
}

void processClientMessages() {
    // Process all received messages in arrival order (best practice for async servers)
    g_network->processAllMessages([](int clientId, const unsigned char* data, size_t size) {
        // Process the message using existing logic
        handleClientMessage(clientId, data, size);
    });
    
    // Check for disconnected clients
    auto activeClients = g_network->getActiveConnections();
    for (int clientId : activeClients) {
        if (!g_network->isConnectionActive(clientId)) {
            // Connection closed
            info("Client " + std::to_string(clientId) + " disconnected");
            
            // Broadcast disconnection to other clients
            if (playerStates[clientId].active) {
                unsigned char packet[8];
                int cmd = 7; // PLAYER_DISCONNECT
                memcpy(packet, &cmd, 4);
                memcpy(packet + 4, &clientId, 4);
                g_network->broadcastData(packet, 8);
            }
            
            // Clear player state
            memset(&playerStates[clientId], 0x00, sizeof(PlayerState));
            playerStates[clientId].active = false;
            playersOnline--;
            
            g_network->closeConnection(clientId);
        }
    }
}

void handleClientMessage(int clientId, const unsigned char* buffer, int size) {
    try {
        // Use MessageReader to parse the message
        MessageReader reader(reinterpret_cast<const char*>(buffer), size);
        
        // Read message type
        MessageType msgType = reader.readMessageType();
        
        switch (msgType) {
            case MessageType::CHAT: {
                // Read chat message
                std::string chatMessage = reader.readString();
                std::string playerName = playerStates[clientId].name;
                
                info("Chat from " + playerName + ": " + chatMessage);
                
                // Broadcast chat message to all clients
                MessageBuilder builder;
                builder.start(MessageType::CHAT);
                builder.writeUint32(clientId); // sender ID
                builder.writeString(playerName);
                builder.writeString(chatMessage);
                auto msgData = builder.build();
                
                g_network->broadcastData(msgData.data(), msgData.size());
                break;
            }
            
            case MessageType::PLAYER_MOVE: {
                // Read movement data
                int x = reader.readUint32();
                int y = reader.readUint32();
                
                playerStates[clientId].x = x;
                playerStates[clientId].y = y;
                info("Player " + std::to_string(clientId) + " moved to (" + std::to_string(x) + "," + std::to_string(y) + ")");
                
                // Broadcast position to other clients
                MessageBuilder builder;
                builder.start(MessageType::PLAYER_MOVE);
                builder.writeUint32(clientId);
                builder.writeUint32(x);
                builder.writeUint32(y);
                auto msgData = builder.build();
                
                g_network->broadcastData(msgData.data(), msgData.size());
                break;
            }
            
            default:
                info("Unknown message type from client " + std::to_string(clientId));
                break;
        }
    } catch (const std::exception& e) {
        error("Error parsing message from client " + std::to_string(clientId) + ": " + e.what());
    }
}

void runServerLoop() {
    info("Starting main server loop.");
    
    while (!shutdownRequested) {
        // Process network events (new connections, disconnections)
        g_network->processEvents();
        
        // Handle new connections
        int newClientId = g_network->acceptConnection();
        if (newClientId >= 0) {
            info("New client connected: " + std::to_string(newClientId));
            playerStates[newClientId].active = true;
            playerStates[newClientId].id = newClientId;
            playerStates[newClientId].x = 100;
            playerStates[newClientId].y = 100;
            playerStates[newClientId].direction = 0;
            snprintf(playerStates[newClientId].name, 32, "Player%d", newClientId);
            playersOnline++;
            
            // Send monsters to new client
            for (int i = 0; i < MAX_MONSTERS_SERVER; ++i) {
                if (monsters[i].active) {
                    unsigned char packet[16];
                    int cmd = 8; // MONSTER_STATE
                    memcpy(packet, &cmd, 4);
                    memcpy(packet + 4, &i, 4);
                    memcpy(packet + 8, &monsters[i].position.x, 4);
                    memcpy(packet + 12, &monsters[i].position.y, 4);
                    g_network->sendData(newClientId, packet, 16);
                }
            }
        }
        
        // Process messages from all clients
        processClientMessages();
        
        // Broadcast player states
        broadcastPlayerStates();
        
        // Small delay to prevent busy-waiting
        SDL_Delay(16); // ~60 FPS
    }
    
    info("Server loop ended.");
}

void sigintHandler(int signum) {
    std::stringstream ss;
    ss << "Interrupt signal (" << signum << ") received. Shutdown requested." << std::endl;
    info(ss.str());
    shutdownRequested = true;
}

void registerSignalHandlers() {
    signal(SIGINT, sigintHandler);
    info("Signal handlers registered.");
}

int SDL_main(int argc, char* argv[]) {
    // Create network manager
    g_network = std::make_unique<NetworkManager>();
    
    registerSignalHandlers();
    
    // Initialize network
    if (!g_network->initialize()) {
        error("Failed to initialize network: " + g_network->getLastError());
        return 1;
    }
    
    info("Network initialized successfully.");
    
    // Start server
    if (!g_network->startServer(8099)) {
        error("Failed to start server: " + g_network->getLastError());
        return 2;
    }
    
    info("Server started on port 8099.");
    
    initializeMonsters();
    
    // Initialize player states
    for (int i = 0; i < MAX_SOCKETS; ++i) {
        playerStates[i].active = false;
    }
    
    // Main server loop
    runServerLoop();
    
    // Cleanup
    g_network.reset();
    return 0;
}