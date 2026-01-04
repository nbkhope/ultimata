#pragma once
#include "Connection.h"
#include <vector>
#include <memory>
#include <unordered_map>
#include "SDL2/SDL_net.h"

class ConnectionManager {
private:
    std::unordered_map<int, std::unique_ptr<Connection>> connections;
    std::vector<int> freeIds;
    int nextId;
    int maxConnections;
    uint32_t connectionTimeout; // milliseconds
    
    // Statistics
    uint32_t totalConnections;
    uint32_t activeConnections;
    
public:
    ConnectionManager(int maxConnections = 16, uint32_t timeoutMs = 30000);
    ~ConnectionManager();
    
    // Connection management
    int addConnection(TCPsocket socket);
    void removeConnection(int connectionId);
    Connection* getConnection(int connectionId);
    
    // Bulk operations
    std::vector<int> getActiveConnectionIds() const;
    std::vector<Connection*> getActiveConnections() const;
    void closeAllConnections();
    
    // Maintenance
    void processTimeouts();
    void processDisconnecting();
    void update();
    
    // Broadcasting
    void broadcastToAll(const void* data, size_t size);
    void broadcastToAllExcept(const void* data, size_t size, int excludeId);
    
    // Statistics
    int getConnectionCount() const { return connections.size(); }
    int getMaxConnections() const { return maxConnections; }
    uint32_t getTotalConnectionsEver() const { return totalConnections; }
    bool isFull() const { return connections.size() >= maxConnections; }
    
    // Configuration
    void setConnectionTimeout(uint32_t timeoutMs) { connectionTimeout = timeoutMs; }
    uint32_t getConnectionTimeout() const { return connectionTimeout; }
};