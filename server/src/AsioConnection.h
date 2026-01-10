#pragma once

// Windows-specific networking definitions
#ifdef _WIN32
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0601  // Windows 7 or later
#endif
#endif

#include <string>
#include <cstdint>
#include <memory>
#include <array>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

enum class ConnectionState {
    Connecting,
    Connected,
    Authenticating,
    Active,
    Disconnecting,
    Disconnected
};

class Connection : public std::enable_shared_from_this<Connection> {
private:
    int id;
    tcp::socket socket;
    ConnectionState state;
    std::string ipAddress;
    uint32_t connectTime;
    uint32_t lastActivity;
    
    // Connection stats
    uint32_t bytesReceived;
    uint32_t bytesSent;
    uint32_t packetsReceived;
    uint32_t packetsSent;
    
    // Read buffer
    std::array<char, 1024> readBuffer;
    
    // Message parsing buffer (accumulates partial messages)
    std::vector<char> messageBuffer;
    
    // Callbacks
    std::function<void(int, const char*, size_t)> onDataReceived;
    std::function<void(int)> onDisconnected;
    
    // Message parsing
    void parseMessages(size_t newBytes);

public:
    Connection(boost::asio::io_context& ioContext);
    ~Connection();
    
    // Basic getters
    int getId() const { return id; }
    tcp::socket& getSocket() { return socket; }
    ConnectionState getState() const { return state; }
    const std::string& getIpAddress() const { return ipAddress; }
    uint32_t getConnectionTime() const;
    uint32_t getLastActivityTime() const { return lastActivity; }
    
    // State management
    void setState(ConnectionState newState) { state = newState; }
    bool isActive() const { return state == ConnectionState::Active; }
    bool isConnected() const { 
        return state != ConnectionState::Disconnected && 
               state != ConnectionState::Disconnecting; 
    }
    
    // Connection lifecycle
    void start(int connectionId);
    void close();
    
    // Data operations
    void sendAsync(const void* data, size_t size);
    void updateActivity();
    
    // Statistics
    uint32_t getBytesReceived() const { return bytesReceived; }
    uint32_t getBytesSent() const { return bytesSent; }
    uint32_t getPacketsReceived() const { return packetsReceived; }
    uint32_t getPacketsSent() const { return packetsSent; }
    
    // Callbacks
    void setDataCallback(std::function<void(int, const char*, size_t)> callback) {
        onDataReceived = callback;
    }
    void setDisconnectCallback(std::function<void(int)> callback) {
        onDisconnected = callback;
    }
    
    // Utility
    bool isTimedOut(uint32_t timeoutMs) const;

private:
    void startRead();
    void handleRead(const boost::system::error_code& error, size_t bytes_transferred);
    void handleWrite(const boost::system::error_code& error, size_t bytes_transferred);
};