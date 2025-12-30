#include <csignal>
#include <cstring>
#include <iostream>
#include <sstream>
#include "SDL2/SDL.h"
#include "SDL2/SDL_net.h"

// Server-local networking constants (do not share client headers)
const int SERVER_MAX_PACKET = 0xFF; // 255-byte packet size for server buffers

//todo: bug) messed up array acess of sockets and not being present in socket after disconnected.

// 16
// #define MAX_SOCKETS 0x10
const int MAX_SOCKETS = 0x10;

// Message buffer constants
const int MESSAGE_BUFFER_SIZE = SERVER_MAX_PACKET;       // Server-local packet size (255 bytes)
const int MESSAGE_RECV_SIZE = MESSAGE_BUFFER_SIZE - 1;   // Reserve 1 byte for null terminator

struct Position
{
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
PlayerState playerStates[MAX_SOCKETS];
// Helper: broadcast all player states to all clients
void broadcastPlayerStates(TCPsocket clientSockets[MAX_SOCKETS]) {
    for (int i = 0; i < MAX_SOCKETS; ++i) {
        if (!clientSockets[i]) continue;
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
                SDLNet_TCP_Send(clientSockets[i], packet, total);
            }
        }
    }
}
int playersOnline = 0;
int freeSocketIndex = 0;

bool shutdownRequested = false;

// class Logger
//problem: name conflict with math.h
// {
void error(std::string message)
{
    std::cerr << message << std::endl;
}

void info(std::string message)
{
    std::cout << message << std::endl;
}

// Monster system
const int MAX_MONSTERS_SERVER = 4;
struct Monster
{
    bool active;
    Position position;
};
Monster monsters[MAX_MONSTERS_SERVER];

void initializeMonsters()
{
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

void snerror(std::string functionName)
{
    std::stringstream ss;
    ss << functionName << " failed: " << SDLNet_GetError() << std::endl;
    error(ss.str());
}
    //todo: log level
// };
// namespace
// Logger log;

void initialize()
{
    info("Initializing libraries.");
    if (SDL_Init(0) == -1) {
        printf("SDL_Init: %s\n", SDL_GetError());
        exit(1);
    }
    info("SDL initialized successfully.");
    if (SDLNet_Init() == -1) {
        printf("SDLNet_Init: %s\n", SDLNet_GetError());
        printf("Make sure SDL2_net.dll is in the same directory as the server executable.\n");
        exit(2);
    }
    info("SDLNet initialized successfully.");
}

void openServerSocket(TCPsocket& serverSocket)
{
    info("Opening server socket.");
    IPaddress ip;

    if (SDLNet_ResolveHost(&ip, NULL, 8099) == -1)
    {
        snerror("SDLNet_ResolveHost");
        exit(3);
    };

    serverSocket = SDLNet_TCP_Open(&ip);
    if (!serverSocket)
    {
        snerror("SDLNet_TCP_Open");
        exit(4);
    }
}

void allocateSockets(SDLNet_SocketSet& socketSet, TCPsocket serverSocket)
{
    info("Allocating sockets and adding server socket.");
    socketSet = SDLNet_AllocSocketSet(MAX_SOCKETS + 1);
    if (!socketSet)
    {
        snerror("SDLNet_AllocSocketSet");
        exit(5);
    }

    //todo: error
    if (SDLNet_TCP_AddSocket(socketSet, serverSocket) == -1)
    {
        snerror("SDLNet_TCP_AddSocket");
        exit(6);
    };
}

void closeSocket(SDLNet_SocketSet& socketSet, TCPsocket clientSockets[MAX_SOCKETS], int socketIndex)
{
    TCPsocket clientSocket = clientSockets[socketIndex];

    if (!clientSocket) {
        // Socket already closed
        return;
    }

    // Try to remove from socket set - log and continue on failure to avoid leaks
    if (SDLNet_TCP_DelSocket(socketSet, clientSocket) == -1)
    {
        std::stringstream ss;
        ss << "SDLNet_TCP_DelSocket (client) failed: " << SDLNet_GetError();
        error(ss.str());
    }

    // Broadcast player disconnection to all remaining clients before clearing state
    if (playerStates[socketIndex].active) {
        for (int i = 0; i < MAX_SOCKETS; i++) {
            if (i != socketIndex && clientSockets[i] && playerStates[i].active) {
                unsigned char packet[8];
                int cmd = 7; // PLAYER_DISCONNECT
                int playerId = socketIndex;
                memcpy(packet, &cmd, 4);
                memcpy(packet + 4, &playerId, 4);
                SDLNet_TCP_Send(clientSockets[i], packet, 8);
            }
        }
        printf("Broadcasted disconnection of player %d to remaining clients\n", socketIndex);
    }

    // Clear player state when socket is closed
    memset(&playerStates[socketIndex], 0x00, sizeof(PlayerState));
    SDLNet_TCP_Close(clientSocket);
    clientSockets[socketIndex] = NULL;
}

void closeClientSockets(SDLNet_SocketSet socketSet, TCPsocket clientSockets[MAX_SOCKETS])
{
    info("Closing client sockets.");
    for (int socketIndex = 0; socketIndex < MAX_SOCKETS; socketIndex++)
    {
        TCPsocket clientSocket = clientSockets[socketIndex];
        if (clientSocket == NULL)
        {
            std::stringstream ss;
            ss << "Skipping closing of index " << socketIndex << " because it wasn't opened.";
            info(ss.str());
            continue;
        }

        closeSocket(socketSet, clientSockets, socketIndex);
        //unnecessary:
        //playersOnline--;
    }



    // todo: close non-server sockets
    // for (int i = 0; i < MAX_SOCKETS; ++i)
    // {
    //
    // }
}

void deallocateSockets(SDLNet_SocketSet& socketSet, TCPsocket serverSocket, TCPsocket clientSockets[MAX_SOCKETS])
{
    info("Deallocating sockets.");
    if (SDLNet_TCP_DelSocket(socketSet, serverSocket) == -1)
    {
        std::stringstream ss;
        ss << "SDLNet_TCP_DelSocket (server) failed: " << SDLNet_GetError();
        error(ss.str());
        // continue cleanup even if removal failed
    }
    SDLNet_TCP_Close(serverSocket);

    closeClientSockets(socketSet, clientSockets);

    SDLNet_FreeSocketSet(socketSet);
}

bool shouldServerQuit()
{
    return shutdownRequested;
    // SDL_Event event;
    // while (SDL_PollEvent(&event))
    // {
    //     switch (event.type)
    //     {
    //         case SDL_KEYDOWN:
    //             switch(event.key.keysym.sym)
    //             {
    //                 case SDLK_q:
    //                     return true;
    //                     break;
    //                 // default:
    //                     //
    //             }
    //             break;
    //         // default:
    //             //
    //     }
    // }
    //
    // return false;
}

void establishServer(SDLNet_SocketSet& socketSet, TCPsocket& serverSocket)
{
    openServerSocket(serverSocket);

    allocateSockets(socketSet, serverSocket);
}

// A serverSocket is ready, so we accept that new socket connection.
bool acceptSocket(SDLNet_SocketSet& socketSet, TCPsocket& serverSocket, TCPsocket clientSockets[MAX_SOCKETS], int socketIndex)
{
    if (clientSockets[socketIndex])
    {
        info("The socket index is pointing to an open socket. That open socket will be replaced by a new connection.");

        closeSocket(socketSet, clientSockets, socketIndex);
    }

    TCPsocket clientSocket = SDLNet_TCP_Accept(serverSocket);
    if (!clientSocket)
    {
        error("Failed to accept socket.");
        return false;
    }

    // Assign the new socket BEFORE adding it to the socket set
    clientSockets[socketIndex] = clientSocket;
    playerStates[socketIndex].active = true;
    playerStates[socketIndex].id = socketIndex;
    playerStates[socketIndex].x = 200; // Set reasonable default spawn position
    playerStates[socketIndex].y = 200; 
    playerStates[socketIndex].direction = 0;
    strcpy(playerStates[socketIndex].name, "Player");
    
    int socketsUsed = SDLNet_TCP_AddSocket(socketSet, clientSockets[socketIndex]);
    if (socketsUsed == -1)
    {
        error("Failed to add socket.");
        exit(12);
    }

    std::cout << "> Added client with index " << socketIndex << std::endl;
    playersOnline++;

    // Send initial monster positions immediately to new client
    struct MonsterPacket {
        int command;
        int count;
        Position positions[MAX_MONSTERS_SERVER];
    };
    
    MonsterPacket monsterData;
    monsterData.command = 4; // NetworkCommands::MONSTER_UPDATE
    monsterData.count = 0;
    
    for (int i = 0; i < MAX_MONSTERS_SERVER; i++)
    {
        if (monsters[i].active)
        {
            monsterData.positions[monsterData.count] = monsters[i].position;
            monsterData.count++;
        }
    }
    
    int packetSize = sizeof(int) * 2 + sizeof(Position) * monsterData.count;
    int monsterBytesSent = SDLNet_TCP_Send(clientSocket, &monsterData, packetSize);
    if (monsterBytesSent >= packetSize)
    {
        std::cout << "Sent initial " << monsterData.count << " monster positions to new client " << socketIndex << std::endl;
    }

    // Send existing player states to the new client immediately
    for (int i = 0; i < MAX_SOCKETS; i++) {
        if (i != socketIndex && playerStates[i].active) {
            unsigned char packet[64];
            int offset = 0;
            int cmd = 6; // PLAYER_STATE
            memcpy(packet + offset, &cmd, 4); offset += 4;
            memcpy(packet + offset, &playerStates[i].id, 4); offset += 4;
            memcpy(packet + offset, &playerStates[i].x, 4); offset += 4;
            memcpy(packet + offset, &playerStates[i].y, 4); offset += 4;
            memcpy(packet + offset, &playerStates[i].direction, 4); offset += 4;
            unsigned char nameLen = (unsigned char)strnlen(playerStates[i].name, 32);
            packet[offset++] = nameLen;
            memcpy(packet + offset, playerStates[i].name, nameLen);
            int total = offset + nameLen;
            SDLNet_TCP_Send(clientSocket, packet, total);
        }
    }
    
    printf("Sent existing player states to new client %d\n", socketIndex);

    // Immediately broadcast the new player to all existing clients so they see them right away
    for (int i = 0; i < MAX_SOCKETS; i++) {
        if (i != socketIndex && clientSockets[i] && playerStates[i].active) {
            unsigned char packet[64];
            int offset = 0;
            int cmd = 6; // PLAYER_STATE
            memcpy(packet + offset, &cmd, 4); offset += 4;
            memcpy(packet + offset, &playerStates[socketIndex].id, 4); offset += 4;
            memcpy(packet + offset, &playerStates[socketIndex].x, 4); offset += 4;
            memcpy(packet + offset, &playerStates[socketIndex].y, 4); offset += 4;
            memcpy(packet + offset, &playerStates[socketIndex].direction, 4); offset += 4;
            unsigned char nameLen = (unsigned char)strnlen(playerStates[socketIndex].name, 32);
            packet[offset++] = nameLen;
            memcpy(packet + offset, playerStates[socketIndex].name, nameLen);
            int total = offset + nameLen;
            SDLNet_TCP_Send(clientSockets[i], packet, total);
        }
    }
    printf("Broadcasted new player %d to existing clients\n", socketIndex);

    return true;
}

void listen(SDLNet_SocketSet& socketSet, TCPsocket serverSocket, TCPsocket clientSockets[MAX_SOCKETS])
{
    bool running = true;

    //todo: rename var
    Uint32 moveTimer = SDL_GetTicks();
    Uint32 lastStatusTime = SDL_GetTicks();
    const Uint32 STATUS_INTERVAL = 10000; // Print status every 10 seconds
    
    info("Listening...");
    while (running)
    {
        if (shouldServerQuit())
        {
            break;
        };

        // Periodic status update instead of constant spam
        Uint32 currentTime = SDL_GetTicks();
        if (currentTime - lastStatusTime > STATUS_INTERVAL) {
            std::cout << "Server status - Players online: " << playersOnline << std::endl;
            lastStatusTime = currentTime;
        }

        // Check sockets with a shorter timeout to reduce CPU usage
        int socketsReady = SDLNet_CheckSockets(socketSet, 10);

        if (socketsReady <= 0)
        {
            // -1 is returned on errors,
            //-1 is also returned for an empty set (nothing to check).
            // process other things
            // Add small delay to prevent CPU spinning
            SDL_Delay(1);

            // e.g. move character
            //todo: the server should be the source of truth for player position...
            Uint32 ticks = SDL_GetTicks();
            bool shouldMove = ticks - moveTimer > 5000;
            if (shouldMove) {
                for (int clientIndex = 0; clientIndex < MAX_SOCKETS; clientIndex++)
                {
                    if (clientSockets[clientIndex] == NULL)
                    {
                        //info("clients[clientIndex] == NULL");
                        //todo: should it be break?
                        continue;
                    }

                    TCPsocket clientSocket = clientSockets[clientIndex];

                    // char message[255] = "move";
                    //todo: uint8_t not working
                    int message = 1; // move - should use NetworkCommands::MOVE constant from Constants.h
                    //int is 4 bytes... in my sys
                    int messageLength = sizeof(message);
                    int bytesSent = SDLNet_TCP_Send(clientSocket, &message, messageLength);//4 + 1
                    if (bytesSent < messageLength)
                    {
                        std::cout << "Client " << clientIndex << " disconnected during send" << std::endl;
                        closeSocket(socketSet, clientSockets, clientIndex);
                        playersOnline--; // Decrement since we lost a player
                    }
                    // else - Sent move message successfully (no need to spam log)
                    
                    // Send monster positions (command + count + positions)
                    // Format: [MONSTER_UPDATE(4 bytes)][count(4 bytes)][x1(4)][y1(4)][x2(4)][y2(4)]...
                    struct MonsterPacket {
                        int command;  // MONSTER_UPDATE = 4
                        int count;    // Number of active monsters
                        Position positions[MAX_MONSTERS_SERVER];
                    };
                    
                    MonsterPacket monsterData;
                    monsterData.command = 4; // NetworkCommands::MONSTER_UPDATE
                    monsterData.count = 0;
                    
                    for (int i = 0; i < MAX_MONSTERS_SERVER; i++)
                    {
                        if (monsters[i].active)
                        {
                            monsterData.positions[monsterData.count] = monsters[i].position;
                            monsterData.count++;
                        }
                    }
                    
                    int packetSize = sizeof(int) * 2 + sizeof(Position) * monsterData.count;
                    int monsterBytesSent = SDLNet_TCP_Send(clientSocket, &monsterData, packetSize);
                    if (monsterBytesSent < packetSize)
                    {
                        std::cout << "Failed to send monster data to client " << clientIndex << std::endl;
                    }
                    // else - Monster data sent successfully (no need to spam log)
                }
                moveTimer = ticks;
            }
        }
        else {
            // process client requests or server

            // Handle server socket
            if (SDLNet_SocketReady(serverSocket) != 0)
            {
                // Accept Socket
                // if (playersOnline < MAX_SOCKETS)
                // {

                bool socketAccepted = acceptSocket(socketSet, serverSocket, clientSockets, freeSocketIndex);

                if (!socketAccepted)
                {
                    //socketsReady-- (makes no sense since var will be reset in next iteration)
                    //continue?
                }

                for (int i = 0; i < MAX_SOCKETS; ++i)
                {
                    int nextPossiblyFreeIndex = (i + freeSocketIndex + 1) % MAX_SOCKETS;
                    if (clientSockets[nextPossiblyFreeIndex] == NULL)
                    {
                        freeSocketIndex = nextPossiblyFreeIndex;
                        break;
                    }
                }

                // }
                // else
                // {
                //     //todo how to reject?
                //     std::cout << "There are too many players online (16). Can't accept client." << std::endl;
                // }
            }

            for (int playerIndex = 0; playerIndex < MAX_SOCKETS; ++playerIndex)
            {
                TCPsocket clientSocket = clientSockets[playerIndex];

                if (!clientSocket)
                {
                    // Skip empty slots; gaps occur when clients disconnect
                    continue;
                }

                if (SDLNet_SocketReady(clientSocket))
                {

                    char message[MESSAGE_BUFFER_SIZE];
                    int bytesReceived = SDLNet_TCP_Recv(clientSocket, message, MESSAGE_RECV_SIZE);
                    if (bytesReceived <= 0)
                    {
                        // TCP Connection is broken. (because of error or closure)
                        std::cout << "Client disconnected: " << SDLNet_GetError() << std::endl;
                        closeSocket(socketSet, clientSockets, playerIndex);
                        playersOnline--;
                    }
                    else
                    {
                        // Check if we received a binary packet (starts with a command)
                        if (bytesReceived >= sizeof(int))
                        {
                            int command = *((int*)message);
                            printf("Received command: %d from client %d\n", command, playerIndex);
                            
                            if (command == 6) // PLAYER_STATE
                            {
                                // Parse player state packet
                                // [command(4)][id(4)][x(4)][y(4)][direction(4)][nameLen(1)][name]
                                if (bytesReceived >= 21)
                                {
                                    int id = *((int*)(message + 4));
                                    int x = *((int*)(message + 8));
                                    int y = *((int*)(message + 12));
                                    int direction = *((int*)(message + 16));
                                    unsigned char nameLen = (unsigned char)message[20];
                                    
                                    // Update player state
                                    playerStates[playerIndex].active = true;
                                    playerStates[playerIndex].id = playerIndex; // Use socket index as ID
                                    playerStates[playerIndex].x = x;
                                    playerStates[playerIndex].y = y;
                                    playerStates[playerIndex].direction = direction;
                                    
                                    if (nameLen > 0 && nameLen <= 32 && bytesReceived >= 21 + nameLen)
                                    {
                                        memcpy(playerStates[playerIndex].name, message + 21, nameLen);
                                        playerStates[playerIndex].name[nameLen] = '\0';
                                    }
                                    else
                                    {
                                        strcpy(playerStates[playerIndex].name, "Player");
                                    }
                                    
                                    // Only log on first connection or significant changes
                                    static bool firstUpdate[MAX_SOCKETS] = {false};
                                    if (!firstUpdate[playerIndex]) {
                                        printf("Player %d connected: %s at (%d,%d)\n", 
                                               playerIndex, playerStates[playerIndex].name, x, y);
                                        firstUpdate[playerIndex] = true;
                                    } else {
                                        // Log movement updates occasionally for debugging
                                        static int moveCount[MAX_SOCKETS] = {0};
                                        moveCount[playerIndex]++;
                                        if (moveCount[playerIndex] % 10 == 0) {
                                            printf("Player %d moved to (%d,%d) [update #%d]\n", 
                                                   playerIndex, x, y, moveCount[playerIndex]);
                                        }
                                    }
                                    
                                    // Broadcast all player states to all clients
                                    broadcastPlayerStates(clientSockets);
                                }
                            }
                        }
                        else
                        {
                            // Handle text messages
                            message[bytesReceived] = 0;
                            // printf("Received text from client: \"%s\"\n", message); // Commented out to reduce spam

                            if (strcmp(message, "disconnect") == 0)
                            {
                                std::cout << "Client " << playerIndex << " asked to disconnect" << std::endl;
                                closeSocket(socketSet, clientSockets, playerIndex);
                                playersOnline--;
                            }
                        }
                    }
                }
                // else - socket not ready (normal, no need to spam log)
            }

        }
    }
}

void destablishServer(SDLNet_SocketSet& socketSet, TCPsocket serverSocket, TCPsocket clientSockets[MAX_SOCKETS])
{
    deallocateSockets(socketSet, serverSocket, clientSockets);
}

void shutdown()
{
    info("Shutting down libraries.");
    SDLNet_Quit();
    SDL_Quit();
}

void sigintHandler(int signum)
{
    std::stringstream ss;
    ss << "Interrupt signal (" << signum << ") received. Shutdown requested." << std::endl;
    info(ss.str());

    shutdownRequested = true;
}

void registerSignalHandlers()
{
    signal(SIGINT, sigintHandler);
}

// todo
// class Server
// {
//
// }

int SDL_main(int argc, char* argv[])
{
    initialize();
    registerSignalHandlers();

    SDLNet_SocketSet socketSet;
    TCPsocket serverSocket;
    TCPsocket clientSockets[MAX_SOCKETS] = { NULL };

    initializeMonsters();
    establishServer(socketSet, serverSocket);
    listen(socketSet, serverSocket, clientSockets);
    destablishServer(socketSet, serverSocket, clientSockets);

    shutdown();
    return 0;
}
