#include "INetworkManager.h"
#include "ConnectionManager.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_net.h"
#include <iostream>
#include <sstream>
#include <cstring>

class SDLNetManager : public INetworkManager {
private:
    TCPsocket serverSocket;
    SDLNet_SocketSet socketSet;
    std::unique_ptr<ConnectionManager> connectionManager;
    int maxClients;
    std::string lastError;
    
    void setError(const std::string& function) {
        std::stringstream ss;
        ss << function << " failed: " << SDLNet_GetError();
        lastError = ss.str();
    }
    
public:
    SDLNetManager() : serverSocket(nullptr), socketSet(nullptr), maxClients(16) {
        connectionManager = std::make_unique<ConnectionManager>(maxClients);
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
        if (connectionManager) {
            connectionManager->closeAllConnections();
        }
        
        if (socketSet) {
            SDLNet_FreeSocketSet(socketSet);
            socketSet = nullptr;
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
        if (!SDLNet_SocketReady(serverSocket)) {
            return -1;  // No pending connections
        }
        
        TCPsocket newSocket = SDLNet_TCP_Accept(serverSocket);
        if (!newSocket) {
            return -1;  // Accept failed
        }
        
        int connectionId = connectionManager->addConnection(newSocket);
        if (connectionId == -1) {
            // Server full or error
            SDLNet_TCP_Close(newSocket);
            lastError = "Failed to add connection - server full";
            return -1;
        }
        
        // Add socket to socket set for monitoring
        if (SDLNet_TCP_AddSocket(socketSet, newSocket) == -1) {
            connectionManager->removeConnection(connectionId);
            setError("SDLNet_TCP_AddSocket");
            return -1;
        }
        
        return connectionId;
    }
    
    void closeConnection(int clientId) override {
        Connection* conn = connectionManager->getConnection(clientId);
        if (!conn) return;
        
        TCPsocket socket = conn->getSocket();
        if (socket) {
            SDLNet_TCP_DelSocket(socketSet, socket);
        }
        
        connectionManager->removeConnection(clientId);
    }
    
    bool isConnectionActive(int clientId) override {
        Connection* conn = connectionManager->getConnection(clientId);
        return conn && conn->isActive();
    }
    
    std::vector<int> getActiveConnections() override {
        return connectionManager->getActiveConnectionIds();
    }
    
    bool sendData(int clientId, const void* data, size_t size) override {
        Connection* conn = connectionManager->getConnection(clientId);
        if (!conn) {
            lastError = "Invalid connection ID";
            return false;
        }
        
        return conn->send(data, size);
    }
    
    int receiveData(int clientId, void* buffer, size_t maxSize) override {
        Connection* conn = connectionManager->getConnection(clientId);
        if (!conn) return -1;
        
        return conn->receive(buffer, maxSize);
    }
    
    void broadcastData(const void* data, size_t size) override {
        connectionManager->broadcastToAll(data, size);
    }
    
    void processEvents() override {
        connectionManager->update();
        
        if (!socketSet) return;
        
        int numReady = SDLNet_CheckSockets(socketSet, 0);
        if (numReady <= 0) return;
        
        // Check server socket for new connections
        if (SDLNet_SocketReady(serverSocket)) {
            acceptConnection();
        }
    }
    
    void setMaxClients(int max) override {
        maxClients = max;
        // Note: This doesn't resize existing connection manager
        // Would need to create a new one for dynamic resizing
    }
    
    std::string getLastError() const override {
        return lastError;
    }
};

// Factory function for creating SDLNetManager
extern "C" INetworkManager* createSDLNetworkManager() {
    return new SDLNetManager();
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