#pragma once
#include <string>
#include <vector>
#include <cstdint>
#include <functional>

// Abstract networking interface
class INetworkManager {
public:
    virtual ~INetworkManager() = default;
    
    // Server management
    virtual bool initialize() = 0;
    virtual void shutdown() = 0;
    virtual bool startServer(uint16_t port) = 0;
    virtual void stopServer() = 0;
    
    // Connection management
    virtual int acceptConnection() = 0; // Returns client ID or -1 if none
    virtual void closeConnection(int clientId) = 0;
    virtual bool isConnectionActive(int clientId) = 0;
    virtual std::vector<int> getActiveConnections() = 0;
    
    // Data transfer
    virtual bool sendData(int clientId, const void* data, size_t size) = 0;
    virtual bool broadcastData(const void* data, size_t size) = 0;
    virtual int receiveData(int clientId, void* buffer, size_t maxSize) = 0;
    virtual void processAllMessages(std::function<void(int clientId, const unsigned char* data, size_t size)> callback) = 0;
    
    // Polling/event handling
    virtual void processEvents() = 0; // Check for new connections, data, etc.
    virtual std::string getLastError() = 0;
    
    // Configuration
    virtual void setMaxClients(int maxClients) = 0;
    virtual int getMaxClients() const = 0;
};

// Factory function type
using NetworkManagerFactory = INetworkManager* (*)();

// Factory functions (defined in implementation files)
INetworkManager* createSDLNetManager();
INetworkManager* createBoostAsioManager(); // Future implementation