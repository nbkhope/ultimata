#include "NetworkManager.h"
#include <iostream>
#include <format>
#include <print>
#include <spdlog/spdlog.h>

NetworkManager::NetworkManager()
    : running(false), connectionManager(std::make_unique<ConnectionManager>(16, 60000)) {
}

NetworkManager::~NetworkManager() {
    shutdown();
}

void NetworkManager::shutdown() {
    if (running) {
        stopServer();
    }

    spdlog::info("Network Manager shutdown complete.");
}

bool NetworkManager::startServer(uint16_t port) {
    try {
        acceptor = std::make_unique<tcp::acceptor>(ioContext, tcp::endpoint(tcp::v4(), port));
        running = true;

        // Start accepting connections
        startAccept();

        spdlog::info("Server started on port {}", port);
        return true;

    } catch (std::exception& e) {
        lastError = std::string("Start server failed: ") + e.what();
        return false;
    }
}

void NetworkManager::stopServer() {
    if (!running) {
        return;
    }

    running = false;

    // Stop accepting new connections
    if (acceptor) {
        boost::system::error_code errorCode;
        acceptor->close(errorCode);
        // Ignore close errors during shutdown
        (void)errorCode;
        acceptor.reset();
    }

    // Stop the io_context
    ioContext.stop();

    spdlog::info("Server stopped.");
}

void NetworkManager::runNetworkLoop() {
    spdlog::info("Network loop started.");
    try {
        ioContext.run();
    } catch (std::exception& e) {
        spdlog::error("Network loop exception: {}", e.what());
    }

    spdlog::info("Network loop ended.");
}

boost::asio::io_context& NetworkManager::getIoContext() {
    return ioContext;
}

void NetworkManager::startAccept() {
    if (!running || !acceptor) {
        return;
    }

    auto newConnection = std::make_shared<Connection>(ioContext);

    acceptor->async_accept(
        newConnection->getSocket(),
        [this, newConnection](const boost::system::error_code& error) {
            handleAccept(newConnection, error);
        }
    );
}

void NetworkManager::handleAccept(const std::shared_ptr<Connection>& newConnection,
                                  const boost::system::error_code& error) {
    if (!error && running) {
        // Add connection to manager
        int connectionId = connectionManager->addConnection(newConnection);
        if (connectionId >= 0) {
            newConnection->start(connectionId);
            newConnection->setState(ConnectionState::Active);
        }

        // Continue accepting new connections
        startAccept();

    } else if (error && running) {
        spdlog::error("Accept error: {}", error.message());
        // Continue accepting despite errors
        startAccept();
    }
}

int NetworkManager::acceptConnection() {
    // With ASIO, connections are accepted automatically in the background
    // This method is kept for interface compatibility but doesn't do the actual accepting
    // Return -1 to indicate no new connection is immediately available
    return -1;
}

void NetworkManager::closeConnection(int clientId) {
    if (connectionManager) {
        connectionManager->removeConnection(clientId);
    }
}

bool NetworkManager::isConnectionActive(int clientId) {
    if (!connectionManager) {
        return false;
    }

    auto connection = connectionManager->getConnection(clientId);
    return connection && connection->isActive();
}

std::vector<int> NetworkManager::getActiveConnections() {
    if (!connectionManager) {
        return {};
    }

    return connectionManager->getActiveConnectionIds();
}

bool NetworkManager::sendData(int clientId, const void* data, size_t size) {
    if (!connectionManager) {
        return false;
    }

    auto connection = connectionManager->getConnection(clientId);
    if (!connection || !connection->isActive()) {
        lastError = "Connection not active";
        return false;
    }

    connection->sendAsync(data, size);
    return true; // Async send always "succeeds" immediately
}

int NetworkManager::receiveData(int clientId, void* buffer, size_t maxSize) {
    // With ASIO async model, data is received through callbacks
    // This method is kept for compatibility but should use the message queue

    if (!connectionManager) {
        return -1;
    }

    // Get received messages from the connection manager
    auto messages = connectionManager->getReceivedMessages();

    // Look for messages from the specified client
    for (const auto& msg : messages) {
        std::println("<ReceivedMessage connectionId={}>", msg.connectionId);
        if (msg.connectionId == clientId) {
            size_t copySize = std::min(maxSize, msg.data.size());
            std::memcpy(buffer, msg.data.data(), copySize);
            return static_cast<int>(copySize);
        }
    }

    return 0; // No data available
}

bool NetworkManager::broadcastData(const void* data, size_t size) {
    if (connectionManager) {
        connectionManager->broadcastToAll(data, size);
        return true;
    }
    return false;
}

void NetworkManager::processAllMessages(const std::function<void(int clientId, const unsigned char* data, size_t size)>& callback) {
    if (!connectionManager || !callback) {
        return;
    }

    // Get all received messages in arrival order
    auto messages = connectionManager->getReceivedMessages();

    // Process each message in the order it was received
    for (const auto& msg : messages) {
        callback(msg.connectionId, reinterpret_cast<const unsigned char*>(msg.data.data()), msg.data.size());
    }
}

void NetworkManager::processEvents() {
    if (connectionManager) {
        connectionManager->update();
    }
}

std::string NetworkManager::getLastError() {
    return lastError;
}