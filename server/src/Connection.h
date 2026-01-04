#pragma once
#include <string>
#include <cstdint>
#include <memory>
#include "SDL2/SDL_net.h"

enum class ConnectionState {
    Connecting,
    Connected,
    Authenticating,
    Active,
    Disconnecting,
    Disconnected
};

class Connection {
private:
    int id;
    TCPsocket socket;
    ConnectionState state;
    std::string ipAddress;
    uint32_t connectTime;
    uint32_t lastActivity;
    
    // Connection stats
    uint32_t bytesReceived;
    uint32_t bytesSent;
    uint32_t packetsReceived;
    uint32_t packetsSent;

public:
    Connection(int id, TCPsocket socket);
    ~Connection();
    
    // Basic getters
    int getId() const { return id; }
    TCPsocket getSocket() const { return socket; }
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
    
    // Data operations
    bool send(const void* data, size_t size);
    int receive(void* buffer, size_t maxSize);
    void updateActivity();
    
    // Statistics
    uint32_t getBytesReceived() const { return bytesReceived; }
    uint32_t getBytesSent() const { return bytesSent; }
    uint32_t getPacketsReceived() const { return packetsReceived; }
    uint32_t getPacketsSent() const { return packetsSent; }
    
    // Utility
    bool isTimedOut(uint32_t timeoutMs) const;
    void close();
};