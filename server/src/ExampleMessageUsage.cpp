// Example: How to send a chat message from a client

#include "MessageProtocol.h"
#include <iostream>

// Example function to send a chat message
void sendChatMessage(/* your socket/network object */, const std::string& message) {
    MessageBuilder builder;
    
    // Start a CHAT message
    builder.start(MessageType::CHAT);
    
    // Add the chat message text
    builder.writeString(message);
    
    // Build the final message (with length prefix)
    auto msgData = builder.build();
    
    // Send to server
    // yourSocket.send(msgData.data(), msgData.size());
    
    std::cout << "Sent chat message: " << message << std::endl;
}

// Example function to send player movement
void sendPlayerMove(/* your socket/network object */, int x, int y) {
    MessageBuilder builder;
    
    // Start a PLAYER_MOVE message
    builder.start(MessageType::PLAYER_MOVE);
    
    // Add position data
    builder.writeUint32(x);
    builder.writeUint32(y);
    
    // Build and send
    auto msgData = builder.build();
    // yourSocket.send(msgData.data(), msgData.size());
    
    std::cout << "Sent move to (" << x << ", " << y << ")" << std::endl;
}

// Example: How to receive and parse messages from the server
void handleServerMessage(const char* data, size_t size) {
    try {
        MessageReader reader(data, size);
        
        MessageType msgType = reader.readMessageType();
        
        switch (msgType) {
            case MessageType::CHAT: {
                uint32_t senderId = reader.readUint32();
                std::string playerName = reader.readString();
                std::string message = reader.readString();
                
                std::cout << "[CHAT] " << playerName << ": " << message << std::endl;
                break;
            }
            
            case MessageType::PLAYER_MOVE: {
                uint32_t playerId = reader.readUint32();
                uint32_t x = reader.readUint32();
                uint32_t y = reader.readUint32();
                
                std::cout << "[MOVE] Player " << playerId << " moved to (" << x << ", " << y << ")" << std::endl;
                break;
            }
            
            default:
                std::cout << "Unknown message type" << std::endl;
                break;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error parsing message: " << e.what() << std::endl;
    }
}
