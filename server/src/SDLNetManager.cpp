#include "INetworkManager.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_net.h"
#include <iostream>
#include <sstream>
#include <cstring>

class SDLNetManager : public INetworkManager {
private:
    TCPsocket serverSocket;
    SDLNet_SocketSet socketSet;
    std::vector<TCPsocket> clientSockets;
    int maxClients;
    std::string lastError;
    
    void setError(const std::string& function) {
        std::stringstream ss;
        ss << function << " failed: " << SDLNet_GetError();
        lastError = ss.str();
    }
    
public:
    SDLNetManager() : serverSocket(nullptr), socketSet(nullptr), maxClients(16) {
        clientSockets.resize(maxClients, nullptr);
    }
    
    ~SDLNetManager() {
        shutdown();
    }
    
    bool initialize() override {
        if (SDL_Init(0) == -1) {
            lastError = std::string("SDL_Init failed: ") + SDL_GetError();
            return false;
        }
        
        if (SDLNet_Init() == -1) {
            lastError = std::string("SDLNet_Init failed: ") + SDLNet_GetError();
            return false;
        }
        
        return true;
    }
    
    void shutdown() override {
        if (socketSet) {
            SDLNet_FreeSocketSet(socketSet);
            socketSet = nullptr;
        }
        
        for (int i = 0; i < maxClients; ++i) {
            if (clientSockets[i]) {
                SDLNet_TCP_Close(clientSockets[i]);
                clientSockets[i] = nullptr;
            }
        }
        
        if (serverSocket) {
            SDLNet_TCP_Close(serverSocket);
            serverSocket = nullptr;
        }
        
        SDLNet_Quit();
        SDL_Quit();
    }
    
    bool startServer(uint16_t port) override {
        IPaddress ip;
        if (SDLNet_ResolveHost(&ip, NULL, port) == -1) {
            setError("SDLNet_ResolveHost");
            return false;
        }
        
        serverSocket = SDLNet_TCP_Open(&ip);
        if (!serverSocket) {
            setError("SDLNet_TCP_Open");
            return false;
        }
        
        socketSet = SDLNet_AllocSocketSet(maxClients + 1);
        if (!socketSet) {
            setError("SDLNet_AllocSocketSet");
            return false;
        }
        
        if (SDLNet_TCP_AddSocket(socketSet, serverSocket) == -1) {
            setError("SDLNet_TCP_AddSocket");
            return false;
        }
        
        return true;
    }
    
    void stopServer() override {
        // Implementation would close server and cleanup
    }
    
    int acceptConnection() override {
        TCPsocket newSocket = SDLNet_TCP_Accept(serverSocket);
        if (!newSocket) return -1;
        
        // Find empty slot
        for (int i = 0; i < maxClients; ++i) {
            if (!clientSockets[i]) {
                clientSockets[i] = newSocket;
                if (SDLNet_TCP_AddSocket(socketSet, newSocket) == -1) {
                    SDLNet_TCP_Close(newSocket);
                    clientSockets[i] = nullptr;
                    setError("SDLNet_TCP_AddSocket");
                    return -1;
                }
                return i;
            }
        }
        
        // No slots available
        SDLNet_TCP_Close(newSocket);
        lastError = "Server full - no available client slots";
        return -1;
    }
    
    void closeConnection(int clientId) override {
        if (clientId < 0 || clientId >= maxClients || !clientSockets[clientId]) return;
        
        SDLNet_TCP_DelSocket(socketSet, clientSockets[clientId]);
        SDLNet_TCP_Close(clientSockets[clientId]);
        clientSockets[clientId] = nullptr;
    }
    
    bool isConnectionActive(int clientId) override {
        return (clientId >= 0 && clientId < maxClients && clientSockets[clientId] != nullptr);
    }
    
    std::vector<int> getActiveConnections() override {
        std::vector<int> active;
        for (int i = 0; i < maxClients; ++i) {
            if (clientSockets[i]) {
                active.push_back(i);
            }
        }
        return active;
    }
    
    bool sendData(int clientId, const void* data, size_t size) override {
        if (!isConnectionActive(clientId)) return false;
        
        int result = SDLNet_TCP_Send(clientSockets[clientId], data, static_cast<int>(size));
        if (result < static_cast<int>(size)) {
            setError("SDLNet_TCP_Send");
            return false;
        }
        return true;
    }
    
    bool broadcastData(const void* data, size_t size) override {
        bool allSuccess = true;
        for (int i = 0; i < maxClients; ++i) {
            if (clientSockets[i]) {
                if (!sendData(i, data, size)) {
                    allSuccess = false;
                }
            }
        }
        return allSuccess;
    }
    
    int receiveData(int clientId, void* buffer, size_t maxSize) override {
        if (!isConnectionActive(clientId)) return -1;
        
        return SDLNet_TCP_Recv(clientSockets[clientId], buffer, static_cast<int>(maxSize));
    }
    
    void processEvents() override {
        if (!socketSet) return;
        
        int numReady = SDLNet_CheckSockets(socketSet, 0);
        if (numReady <= 0) return;
        
        // Check server socket for new connections
        if (SDLNet_SocketReady(serverSocket)) {
            acceptConnection();
        }
    }
    
    std::string getLastError() override {
        return lastError;
    }
    
    void setMaxClients(int max) override {
        maxClients = max;
        clientSockets.resize(max, nullptr);
    }
    
    int getMaxClients() const override {
        return maxClients;
    }
};

INetworkManager* createSDLNetManager() {
    return new SDLNetManager();
}