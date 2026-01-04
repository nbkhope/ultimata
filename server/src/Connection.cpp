#include "Connection.h"
#include "SDL2/SDL.h"
#include <iostream>
#include <sstream>

Connection::Connection(int id, TCPsocket socket) 
    : id(id), socket(socket), state(ConnectionState::Connecting),
      connectTime(SDL_GetTicks()), lastActivity(SDL_GetTicks()),
      bytesReceived(0), bytesSent(0), packetsReceived(0), packetsSent(0) {
    
    // Get IP address from socket
    IPaddress* remoteIP = SDLNet_TCP_GetPeerAddress(socket);
    if (remoteIP) {
        uint32_t ip = SDL_SwapBE32(remoteIP->host);
        std::stringstream ss;
        ss << ((ip >> 24) & 0xFF) << "."
           << ((ip >> 16) & 0xFF) << "."
           << ((ip >> 8) & 0xFF) << "."
           << (ip & 0xFF);
        ipAddress = ss.str();
    } else {
        ipAddress = "unknown";
    }
    
    std::cout << "New connection [" << id << "] from " << ipAddress << std::endl;
}

Connection::~Connection() {
    close();
}

uint32_t Connection::getConnectionTime() const {
    return SDL_GetTicks() - connectTime;
}

bool Connection::send(const void* data, size_t size) {
    if (!socket || state == ConnectionState::Disconnected) {
        return false;
    }
    
    int result = SDLNet_TCP_Send(socket, data, static_cast<int>(size));
    if (result >= static_cast<int>(size)) {
        bytesSent += size;
        packetsSent++;
        updateActivity();
        return true;
    }
    
    // Send failed - connection likely broken
    std::cout << "Send failed for connection [" << id << "]: " << SDLNet_GetError() << std::endl;
    setState(ConnectionState::Disconnecting);
    return false;
}

int Connection::receive(void* buffer, size_t maxSize) {
    if (!socket || state == ConnectionState::Disconnected) {
        return -1;
    }
    
    int result = SDLNet_TCP_Recv(socket, buffer, static_cast<int>(maxSize));
    if (result > 0) {
        bytesReceived += result;
        packetsReceived++;
        updateActivity();
    } else if (result <= 0) {
        // Connection closed or error
        setState(ConnectionState::Disconnecting);
    }
    
    return result;
}

void Connection::updateActivity() {
    lastActivity = SDL_GetTicks();
}

bool Connection::isTimedOut(uint32_t timeoutMs) const {
    return (SDL_GetTicks() - lastActivity) > timeoutMs;
}

void Connection::close() {
    if (socket) {
        std::cout << "Closing connection [" << id << "] from " << ipAddress 
                  << " (active for " << getConnectionTime() << "ms)" << std::endl;
        SDLNet_TCP_Close(socket);
        socket = nullptr;
    }
    state = ConnectionState::Disconnected;
}