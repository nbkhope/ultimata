#include <csignal>
#include <cstring>
#include <iostream>
#include <sstream>
#include "SDL2/SDL.h"
#include "SDL2/SDL_net.h"

//todo: bug) messed up array acess of sockets and not being present in socket after disconnected.

// 16
// #define MAX_SOCKETS 0x10
const int MAX_SOCKETS = 0x10;

struct Position
{
    int x;
    int y;
};


struct Client
{
    bool active;
    Position position;
};
Client clients[MAX_SOCKETS];
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

    if (clientSocket && SDLNet_TCP_DelSocket(socketSet, clientSocket) == -1)
    {
        snerror("closeSocket: SDLNet_TCP_DelSocket");
        exit(7);
    };

    // todo: can we remove memset here?
    memset(&clients[socketIndex], 0x00, sizeof(Client));
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
        snerror("SDLNet_TCP_DelSocket");
        exit(7);
    };
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

    clients[socketIndex].active = true;
    int socketsUsed = SDLNet_TCP_AddSocket(socketSet, clientSockets[socketIndex]);
    if (socketsUsed == -1)
    {
        error("Failed to add socket.");
        exit(12);
    }

    clientSockets[socketIndex] = clientSocket;

    std::cout << "> Added client with index " << socketIndex << std::endl;
    playersOnline++;

    SDL_Delay(3000);

    return true;
}

void listen(SDLNet_SocketSet& socketSet, TCPsocket serverSocket, TCPsocket clientSockets[MAX_SOCKETS])
{
    bool running = true;

    //todo: rename var
    Uint32 moveTimer = SDL_GetTicks();
    info("Listening...");
    while (running)
    {
        if (shouldServerQuit())
        {
            break;
        };

        std::stringstream ss;
        ss << "Checking sockets..." << " (Players online: " << playersOnline << ")";
        info(ss.str());
        int socketsReady = SDLNet_CheckSockets(socketSet, 1000);

        if (socketsReady <= 0)
        {
            // -1 is returned on errors,
            //-1 is also returned for an empty set (nothing to check).
            // process other things
            info("> No sockets ready.");

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
                    int message = 1; // move
                    //int is 4 bytes... in my sys
                    int messageLength = sizeof(message);
                    int bytesSent = SDLNet_TCP_Send(clientSocket, &message, messageLength);//4 + 1
                    if (bytesSent < messageLength)
                    {
                        snerror("listen: move character: SDLNet_TCP_Send");
                        info("Disconnecting client");
                        closeSocket(socketSet, clientSockets, clientIndex);
                    }
                    else
                    {
                        std::cout << "Sent message to client index " << clientIndex << ": " << message << std::endl;
                        std::cout << "Message length: " << messageLength << std::endl;
                    }
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

            int playerIndex = 0;
            while (playerIndex < playersOnline)
            {
                TCPsocket clientSocket = clientSockets[playerIndex];

                if (!clientSocket)
                {
                    std::cout << "> Client socket is NULL: " << playerIndex << std::endl;
                }
                else if (SDLNet_SocketReady(clientSocket))// != NULL
                {
                    std::cout << "> Working on client socket" << std::endl;

                    char message[255];
                    int bytesReceived = SDLNet_TCP_Recv(clientSocket, message, 255 - 1);
                    if (bytesReceived <= 0)
                    {
                        //TODO handling
                        // TCP Connection is broken. (because of error or closure)
                        int socketsUsed = SDLNet_TCP_DelSocket(socketSet, clientSocket);
                        if (socketsUsed == -1)
                        {
                            snerror("SDLNet_TCP_DelSocket");
                        }
                        //todo: error
                        SDLNet_TCP_Close(clientSocket);
                        //clientSocket = NULL;
                        clientSockets[playerIndex] = NULL;
                        //todo: big problem. Array is not dynamic. So it wont shift elements down...
                        playersOnline--;
                        std::cout << "Client disconnected: " << SDLNet_GetError() << std::endl;
                    }
                    else
                    {
                        message[bytesReceived] = 0;
                        printf("Received from client: \"%s\"", message);

                        if (strcmp(message, "disconnect") == 0)
                        {
                            std::cout << "Client " << playerIndex << " asked to disconnect" << std::endl;
                            closeSocket(socketSet, clientSockets, playerIndex);
                        }
                    }
                }
                else
                {
                    std::cout << "> Client socket " << playerIndex << " is not ready." << std::endl;
                }

                playerIndex++;
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

    establishServer(socketSet, serverSocket);
    listen(socketSet, serverSocket, clientSockets);
    destablishServer(socketSet, serverSocket, clientSockets);

    shutdown();
    return 0;
}
