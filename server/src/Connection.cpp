#include "Connection.h"
#include <iostream>
#include <sstream>
#include <chrono>

#ifdef _WIN32
#include <winsock2.h>
#else
#include <arpa/inet.h>
#endif

// Maximum allowed message size (1MB)
const uint32_t MAX_MESSAGE_SIZE = 1024 * 1024;

// Cross-platform timing utility
uint32_t getCurrentTimeMs() {
    auto now = std::chrono::steady_clock::now();
    auto duration = now.time_since_epoch();
    return static_cast<uint32_t>(std::chrono::duration_cast<std::chrono::milliseconds>(duration).count());
}


Connection::Connection(boost::asio::io_context& ioContext) 
    : id(-1), socket(ioContext), state(ConnectionState::Connecting),
      connectTime(getCurrentTimeMs()), lastActivity(getCurrentTimeMs()),
      bytesReceived(0), bytesSent(0), packetsReceived(0), packetsSent(0) {
}

Connection::~Connection() {
    close();
}

uint32_t Connection::getConnectionTime() const {
    return getCurrentTimeMs() - connectTime;
}

void Connection::start(int connectionId) {
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

void Connection::startRead() {
    if (!isConnected()) return;
    
    auto self = shared_from_this();
    socket.async_read_some(
        boost::asio::buffer(readBuffer),
        [this, self](const boost::system::error_code& error, size_t bytes_transferred) {
            std::cout << "async_read_some callback" << std::endl;
            handleRead(error, bytes_transferred);
        }
    );
}

void Connection::handleRead(const boost::system::error_code& error, size_t bytes_transferred) {
    if (!error) {
        bytesReceived += bytes_transferred;
        packetsReceived++;
        updateActivity();
        
        // Append new data to message buffer and parse complete messages
        if (bytes_transferred > 0) {
            parseMessages(bytes_transferred);
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

void Connection::sendAsync(const void* data, size_t size) {
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

void Connection::handleWrite(const boost::system::error_code& error, size_t bytes_transferred) {
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

void Connection::updateActivity() {
    lastActivity = getCurrentTimeMs();
}

bool Connection::isTimedOut(uint32_t timeoutMs) const {
    return (getCurrentTimeMs() - lastActivity) > timeoutMs;
}

void Connection::close() {
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

void Connection::parseMessages(size_t newBytes) {
    // Append new data to the message buffer
    messageBuffer.insert(messageBuffer.end(), readBuffer.data(), readBuffer.data() + newBytes);
    
    // Parse complete messages (length-prefixed: [4 bytes length][data])
    while (messageBuffer.size() >= 4) {
        // Read message length (first 4 bytes) in network byte order
        uint32_t netLength;
        std::memcpy(&netLength, messageBuffer.data(), 4);
        uint32_t messageLength = ntohl(netLength);
        
        // Validate message size to prevent DoS attacks
        if (messageLength > MAX_MESSAGE_SIZE) {
            std::cout << "Connection [" << id << "] sent oversized message (" << messageLength 
                      << " bytes), closing connection" << std::endl;
            setState(ConnectionState::Disconnecting);
            if (onDisconnected) {
                onDisconnected(id);
            }
            messageBuffer.clear();
            return;
        }
        
        // Check if we have the complete message
        if (messageBuffer.size() >= 4 + messageLength) {
            // Extract the complete message (skip the 4-byte length prefix)
            if (onDataReceived && messageLength > 0) {
                onDataReceived(id, messageBuffer.data() + 4, messageLength);
            }
            
            // Remove the processed message from the buffer
            messageBuffer.erase(messageBuffer.begin(), messageBuffer.begin() + 4 + messageLength);
        } else {
            // Incomplete message, wait for more data
            break;
        }
    }
}