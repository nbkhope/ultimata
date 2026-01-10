#include "AsioNetworkManager.h"
#include <iostream>
#include <format>
#include <print>

AsioNetworkManager::AsioNetworkManager() 
    : running(false), connectionManager(std::make_unique<ConnectionManager>()) {
}

AsioNetworkManager::~AsioNetworkManager() {
    shutdown();
}

bool AsioNetworkManager::initialize() {
    try {
        std::cout << "ASIO Network Manager initialized successfully." << std::endl;
        return true;
    } catch (std::exception& e) {
        lastError = std::string("Initialize failed: ") + e.what();
        return false;
    }
}

void AsioNetworkManager::shutdown() {
    if (running) {
        stopServer();
    }
    
    if (connectionManager) {
        connectionManager->closeAllConnections();
    }
    
    std::cout << "ASIO Network Manager shutdown complete." << std::endl;
}

bool AsioNetworkManager::startServer(uint16_t port) {
    try {
        acceptor = std::make_unique<tcp::acceptor>(ioContext, tcp::endpoint(tcp::v4(), port));
        running = true;
        
        // Start accepting connections
        startAccept();
        
        // Start the network thread
        networkThread = std::thread([this] { runNetworkThread(); });
        
        std::cout << "ASIO Server started on port " << port << std::endl;
        return true;
        
    } catch (std::exception& e) {
        lastError = std::string("Start server failed: ") + e.what();
        return false;
    }
}

void AsioNetworkManager::stopServer() {
    if (!running) return;
    
    running = false;
    
    // Stop accepting new connections
    if (acceptor) {
        boost::system::error_code ec;
        acceptor->close(ec);
        acceptor.reset();
    }
    
    // Stop the io_context
    ioContext.stop();
    
    // Wait for network thread to finish
    if (networkThread.joinable()) {
        networkThread.join();
    }
    
    std::cout << "ASIO Server stopped." << std::endl;
}

void AsioNetworkManager::runNetworkThread() {
    std::cout << "Network thread started." << std::endl;
    
    try {
        ioContext.run();
    } catch (std::exception& e) {
        std::cout << "Network thread exception: " << e.what() << std::endl;
    }
    
    std::cout << "Network thread ended." << std::endl;
}

void AsioNetworkManager::startAccept() {
    if (!running || !acceptor) return;
    
    auto newConnection = std::make_shared<Connection>(ioContext);
    
    acceptor->async_accept(
        newConnection->getSocket(),
        [this, newConnection](const boost::system::error_code& error) {
            handleAccept(newConnection, error);
        }
    );
}

void AsioNetworkManager::handleAccept(std::shared_ptr<Connection> newConnection, 
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
        std::cout << "Accept error: " << error.message() << std::endl;
        // Continue accepting despite errors
        startAccept();
    }
}

int AsioNetworkManager::acceptConnection() {
    // With ASIO, connections are accepted automatically in the background
    // This method is kept for interface compatibility but doesn't do the actual accepting
    // Return -1 to indicate no new connection is immediately available
    return -1;
}

void AsioNetworkManager::closeConnection(int clientId) {
    if (connectionManager) {
        connectionManager->removeConnection(clientId);
    }
}

bool AsioNetworkManager::isConnectionActive(int clientId) {
    if (!connectionManager) return false;
    
    auto connection = connectionManager->getConnection(clientId);
    return connection && connection->isActive();
}

std::vector<int> AsioNetworkManager::getActiveConnections() {
    if (!connectionManager) return {};
    
    return connectionManager->getActiveConnectionIds();
}

bool AsioNetworkManager::sendData(int clientId, const void* data, size_t size) {
    if (!connectionManager) return false;
    
    auto connection = connectionManager->getConnection(clientId);
    if (!connection || !connection->isActive()) {
        lastError = "Connection not active";
        return false;
    }
    
    connection->sendAsync(data, size);
    return true; // Async send always "succeeds" immediately
}

int AsioNetworkManager::receiveData(int clientId, void* buffer, size_t maxSize) {
    // With ASIO async model, data is received through callbacks
    // This method is kept for compatibility but should use the message queue
    
    if (!connectionManager) return -1;
    
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

bool AsioNetworkManager::broadcastData(const void* data, size_t size) {
    if (connectionManager) {
        connectionManager->broadcastToAll(data, size);
        return true;
    }
    return false;
}

void AsioNetworkManager::processAllMessages(std::function<void(int clientId, const unsigned char* data, size_t size)> callback) {
    if (!connectionManager || !callback) return;
    
    // Get all received messages in arrival order
    auto messages = connectionManager->getReceivedMessages();
    
    // Process each message in the order it was received
    for (const auto& msg : messages) {
        callback(msg.connectionId, reinterpret_cast<const unsigned char*>(msg.data.data()), msg.data.size());
    }
}

void AsioNetworkManager::processEvents() {
    if (connectionManager) {
        connectionManager->update();
    }
}

std::string AsioNetworkManager::getLastError() {
    return lastError;
}