#pragma once
#include "AsioConnection.h"
#include <vector>
#include <memory>
#include <unordered_map>
#include <mutex>
#include <boost/asio.hpp>

class ConnectionManager {
private:
    std::unordered_map<int, std::shared_ptr<Connection>> connections;
    std::vector<int> freeIds;
    int nextId;
    int maxConnections;
    uint32_t connectionTimeout; // milliseconds
    
    // Statistics
    uint32_t totalConnections;
    uint32_t activeConnections;
    
    // Message queue for received data
    struct ReceivedMessage {
        int connectionId;
        std::vector<char> data;
    };
    std::vector<ReceivedMessage> receivedMessages;
    std::mutex messageQueueMutex;

public:
    AsioConnectionManager(int maxConnections = 16, uint32_t timeoutMs = 30000);
    ~AsioConnectionManager();
    
    // Connection management
    int addConnection(std::shared_ptr<Connection> connection);
    void removeConnection(int connectionId);
    std::shared_ptr<Connection> getConnection(int connectionId);
    
    // Bulk operations
    std::vector<int> getActiveConnectionIds() const;
    std::vector<std::shared_ptr<Connection>> getActiveConnections() const;
    void closeAllConnections();
    
    // Maintenance
    void processTimeouts();
    void processDisconnecting();
    void update();
    
    // Broadcasting
    void broadcastToAll(const void* data, size_t size);
    void broadcastToAllExcept(const void* data, size_t size, int excludeId);
    
    // Message handling
    std::vector<ReceivedMessage> getReceivedMessages();
    void onDataReceived(int connectionId, const char* data, size_t size);
    void onConnectionDisconnected(int connectionId);
    
    // Statistics
    int getConnectionCount() const { return connections.size(); }
    int getMaxConnections() const { return maxConnections; }
    uint32_t getTotalConnectionsEver() const { return totalConnections; }
    bool isFull() const { return connections.size() >= maxConnections; }
    
    // Configuration
    void setConnectionTimeout(uint32_t timeoutMs) { connectionTimeout = timeoutMs; }
    uint32_t getConnectionTimeout() const { return connectionTimeout; }
};