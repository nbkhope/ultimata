#include "ConnectionManager.h"
#include <iostream>
#include <algorithm>

ConnectionManager::ConnectionManager(int maxConnections, uint32_t timeoutMs)
    : nextId(0), maxConnections(maxConnections), connectionTimeout(timeoutMs),
      totalConnections(0), activeConnections(0) {
    
    // Pre-fill free IDs
    freeIds.reserve(maxConnections);
    for (int i = maxConnections - 1; i >= 0; --i) {
        freeIds.push_back(i);
    }
    
    std::cout << "ConnectionManager initialized (max: " << maxConnections 
              << ", timeout: " << timeoutMs << "ms)" << std::endl;
}

ConnectionManager::~ConnectionManager() {
    closeAllConnections();
}

int ConnectionManager::addConnection(TCPsocket socket) {
    if (isFull()) {
        std::cout << "Cannot accept connection - server full (" << maxConnections << ")" << std::endl;
        return -1;
    }
    
    // Get next available ID
    int connectionId;
    if (!freeIds.empty()) {
        connectionId = freeIds.back();
        freeIds.pop_back();
    } else {
        connectionId = nextId++;
    }
    
    // Create connection
    auto connection = std::make_unique<Connection>(connectionId, socket);
    connections[connectionId] = std::move(connection);
    
    totalConnections++;
    activeConnections++;
    
    std::cout << "Added connection [" << connectionId << "] (total: " 
              << connections.size() << "/" << maxConnections << ")" << std::endl;
    
    return connectionId;
}

void ConnectionManager::removeConnection(int connectionId) {
    auto it = connections.find(connectionId);
    if (it == connections.end()) {
        return;
    }
    
    std::cout << "Removing connection [" << connectionId << "]" << std::endl;
    
    // Clean up the connection
    it->second.reset();
    connections.erase(it);
    
    // Return ID to free pool
    freeIds.push_back(connectionId);
    activeConnections--;
}

Connection* ConnectionManager::getConnection(int connectionId) {
    auto it = connections.find(connectionId);
    return (it != connections.end()) ? it->second.get() : nullptr;
}

std::vector<int> ConnectionManager::getActiveConnectionIds() const {
    std::vector<int> activeIds;
    activeIds.reserve(connections.size());
    
    for (const auto& pair : connections) {
        if (pair.second->isConnected()) {
            activeIds.push_back(pair.first);
        }
    }
    
    return activeIds;
}

std::vector<Connection*> ConnectionManager::getActiveConnections() const {
    std::vector<Connection*> activeConns;
    activeConns.reserve(connections.size());
    
    for (const auto& pair : connections) {
        if (pair.second->isConnected()) {
            activeConns.push_back(pair.second.get());
        }
    }
    
    return activeConns;
}

void ConnectionManager::closeAllConnections() {
    std::cout << "Closing all connections (" << connections.size() << ")" << std::endl;
    
    for (auto& pair : connections) {
        pair.second->close();
    }
    
    connections.clear();
    freeIds.clear();
    
    // Refill free IDs
    for (int i = maxConnections - 1; i >= 0; --i) {
        freeIds.push_back(i);
    }
    
    activeConnections = 0;
}

void ConnectionManager::processTimeouts() {
    std::vector<int> timedOut;
    
    for (const auto& pair : connections) {
        if (pair.second->isTimedOut(connectionTimeout)) {
            timedOut.push_back(pair.first);
        }
    }
    
    for (int id : timedOut) {
        std::cout << "Connection [" << id << "] timed out" << std::endl;
        removeConnection(id);
    }
}

void ConnectionManager::processDisconnecting() {
    std::vector<int> toRemove;
    
    for (const auto& pair : connections) {
        if (pair.second->getState() == ConnectionState::Disconnecting) {
            toRemove.push_back(pair.first);
        }
    }
    
    for (int id : toRemove) {
        removeConnection(id);
    }
}

void ConnectionManager::update() {
    processTimeouts();
    processDisconnecting();
}

void ConnectionManager::broadcastToAll(const void* data, size_t size) {
    int sent = 0;
    for (const auto& pair : connections) {
        if (pair.second->isActive()) {
            if (pair.second->send(data, size)) {
                sent++;
            }
        }
    }
    
    if (sent > 0) {
        std::cout << "Broadcasted " << size << " bytes to " << sent << " clients" << std::endl;
    }
}

void ConnectionManager::broadcastToAllExcept(const void* data, size_t size, int excludeId) {
    int sent = 0;
    for (const auto& pair : connections) {
        if (pair.first != excludeId && pair.second->isActive()) {
            if (pair.second->send(data, size)) {
                sent++;
            }
        }
    }
    
    if (sent > 0) {
        std::cout << "Broadcasted " << size << " bytes to " << sent << " clients (excluding " 
                  << excludeId << ")" << std::endl;
    }
}