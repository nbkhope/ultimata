#include "AsioConnection.h"
#include <iostream>
#include <sstream>
#include <chrono>

// Cross-platform timing utility
uint32_t getCurrentTimeMs() {
    auto now = std::chrono::steady_clock::now();
    auto duration = now.time_since_epoch();
    return static_cast<uint32_t>(std::chrono::duration_cast<std::chrono::milliseconds>(duration).count());
}

AsioConnection::AsioConnection(boost::asio::io_context& ioContext) 
    : id(-1), socket(ioContext), state(ConnectionState::Connecting),
      connectTime(getCurrentTimeMs()), lastActivity(getCurrentTimeMs()),
      bytesReceived(0), bytesSent(0), packetsReceived(0), packetsSent(0) {
}

AsioConnection::~AsioConnection() {
    close();
}

uint32_t AsioConnection::getConnectionTime() const {
    return getCurrentTimeMs() - connectTime;
}

void AsioConnection::start(int connectionId) {
    id = connectionId;
    
    // Get remote IP address
    try {
        auto endpoint = socket.remote_endpoint();
        ipAddress = endpoint.address().to_string();
        std::cout << "New connection [" << id << "] from " << ipAddress << std::endl;
    } catch (std::exception& e) {
        ipAddress = "unknown";
        std::cout << "Could not get IP for connection [" << id << "]: " << e.what() << std::endl;
    }
    
    setState(ConnectionState::Connected);
    startRead();
}

void AsioConnection::startRead() {
    if (!isConnected()) return;
    
    auto self = shared_from_this();
    socket.async_read_some(
        boost::asio::buffer(readBuffer),
        [this, self](const boost::system::error_code& error, size_t bytes_transferred) {
            handleRead(error, bytes_transferred);
        }
    );
}

void AsioConnection::handleRead(const boost::system::error_code& error, size_t bytes_transferred) {
    if (!error) {
        bytesReceived += bytes_transferred;
        packetsReceived++;
        updateActivity();
        
        // Call data received callback
        if (onDataReceived && bytes_transferred > 0) {
            onDataReceived(id, readBuffer.data(), bytes_transferred);
        }
        
        // Continue reading
        startRead();
    } else {
        std::cout << "Read error on connection [" << id << "]: " << error.message() << std::endl;
        setState(ConnectionState::Disconnecting);
        
        // Notify about disconnection
        if (onDisconnected) {
            onDisconnected(id);
        }
    }
}

void AsioConnection::sendAsync(const void* data, size_t size) {
    if (!isConnected()) {
        std::cout << "Attempted to send to disconnected connection [" << id << "]" << std::endl;
        return;
    }
    
    // Create a copy of the data for async operation
    auto buffer = std::make_shared<std::string>(static_cast<const char*>(data), size);
    auto self = shared_from_this();
    
    boost::asio::async_write(
        socket,
        boost::asio::buffer(*buffer),
        [this, self, buffer](const boost::system::error_code& error, size_t bytes_transferred) {
            handleWrite(error, bytes_transferred);
        }
    );
}

void AsioConnection::handleWrite(const boost::system::error_code& error, size_t bytes_transferred) {
    if (!error) {
        bytesSent += bytes_transferred;
        packetsSent++;
        updateActivity();
    } else {
        std::cout << "Write error on connection [" << id << "]: " << error.message() << std::endl;
        setState(ConnectionState::Disconnecting);
        
        if (onDisconnected) {
            onDisconnected(id);
        }
    }
}

void AsioConnection::updateActivity() {
    lastActivity = getCurrentTimeMs();
}

bool AsioConnection::isTimedOut(uint32_t timeoutMs) const {
    return (getCurrentTimeMs() - lastActivity) > timeoutMs;
}

void AsioConnection::close() {
    if (socket.is_open()) {
        std::cout << "Closing connection [" << id << "] from " << ipAddress 
                  << " (active for " << getConnectionTime() << "ms)" << std::endl;
        
        boost::system::error_code ec;
        socket.close(ec);
        if (ec) {
            std::cout << "Error closing socket: " << ec.message() << std::endl;
        }
    }
    state = ConnectionState::Disconnected;
}