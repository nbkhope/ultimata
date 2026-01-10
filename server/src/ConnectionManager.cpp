#include "ConnectionManager.h"
#include <iostream>
#include <algorithm>
#include <mutex>

ConnectionManager::ConnectionManager(int maxConnections, uint32_t timeoutMs)
    : nextId(0), maxConnections(maxConnections), connectionTimeout(timeoutMs),
      totalConnections(0), activeConnections(0) {

    // Pre-fill free IDs
    freeIds.reserve(maxConnections);
    for (int i = maxConnections - 1; i >= 0; --i) {
        freeIds.push_back(i);
    }

    std::cout << "ConnectionManager initialized (max: " << maxConnections
              << ", timeout: " << timeoutMs << "ms)" << '\n';
}

ConnectionManager::~ConnectionManager() {
    closeAllConnections();
}

int ConnectionManager::addConnection(std::shared_ptr<Connection> connection) {
    if (isFull()) {
        std::cout << "Cannot accept connection - server full (" << maxConnections << ")" << '\n';
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

    // Set up callbacks
    connection->setDataCallback([this](int id, const char* data, size_t size) {
        onDataReceived(id, data, size);
    });

    connection->setDisconnectCallback([this](int id) {
        onConnectionDisconnected(id);
    });

    // Add to connections map
    connections[connectionId] = connection;

    totalConnections++;
    activeConnections++;

    std::cout << "Added connection [" << connectionId << "] (total: "
              << connections.size() << "/" << maxConnections << ")" << '\n';

    return connectionId;
}

void ConnectionManager::removeConnection(int connectionId) {
    auto it = connections.find(connectionId);
    if (it == connections.end()) {
        return;
    }

    std::cout << "Removing connection [" << connectionId << "]" << '\n';

    // Close and clean up the connection
    it->second->close();
    connections.erase(it);

    // Return ID to free pool
    freeIds.push_back(connectionId);
    activeConnections--;
}

std::shared_ptr<Connection> ConnectionManager::getConnection(int connectionId) {
    auto it = connections.find(connectionId);
    return (it != connections.end()) ? it->second : nullptr;
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

std::vector<std::shared_ptr<Connection>> ConnectionManager::getActiveConnections() const {
    std::vector<std::shared_ptr<Connection>> activeConns;
    activeConns.reserve(connections.size());

    for (const auto& pair : connections) {
        if (pair.second->isConnected()) {
            activeConns.push_back(pair.second);
        }
    }

    return activeConns;
}

void ConnectionManager::closeAllConnections() {
    std::cout << "Closing all connections (" << connections.size() << ")" << '\n';

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
        std::cout << "Connection [" << id << "] timed out" << '\n';
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
            pair.second->sendAsync(data, size);
            sent++;
        }
    }

    if (sent > 0) {
        std::cout << "Broadcasted " << size << " bytes to " << sent << " clients" << '\n';
    }
}

void ConnectionManager::broadcastToAllExcept(const void* data, size_t size, int excludeId) {
    int sent = 0;
    for (const auto& pair : connections) {
        if (pair.first != excludeId && pair.second->isActive()) {
            pair.second->sendAsync(data, size);
            sent++;
        }
    }

    if (sent > 0) {
        std::cout << "Broadcasted " << size << " bytes to " << sent << " clients (excluding "
                  << excludeId << ")" << '\n';
    }
}

std::vector<ConnectionManager::ReceivedMessage> ConnectionManager::getReceivedMessages() {
    std::lock_guard<std::mutex> lock(messageQueueMutex);
    std::vector<ReceivedMessage> messages;
    messages.swap(receivedMessages);
    return messages;
}

void ConnectionManager::onDataReceived(int connectionId, const char* data, size_t size) {
    std::lock_guard<std::mutex> lock(messageQueueMutex);

    ReceivedMessage msg;
    msg.connectionId = connectionId;
    msg.data.assign(data, data + size);
    receivedMessages.push_back(std::move(msg));
}

void ConnectionManager::onConnectionDisconnected(int connectionId) {
    // Schedule removal for next update cycle
    auto connection = getConnection(connectionId);
    if (connection) {
        connection->setState(ConnectionState::Disconnecting);
    }
}