#pragma once

#include <string>
#include <cstdint>

// Message type identifiers
enum class MessageType : uint8_t {
    // Client -> Server
    PlayerMove = 1,
    ChatMessage = 2,
    PlayerAction = 3,

    // Server -> Client
    PlayerJoined = 10,
    PlayerLeft = 11,
    GameStateUpdate = 12,
    ChatBroadcast = 13,

    // Bidirectional
    Ping = 20,
    Pong = 21
};

// Base message structure
struct Message {
    MessageType type;
    int clientId = -1;  // -1 = broadcast/not set, >= 0 = specific client
    virtual ~Message() = default;
};

// Client -> Server: Player movement
struct PlayerMoveMessage : Message {
    PlayerMoveMessage() { type = MessageType::PlayerMove; }
    int playerId;
    float x;
    float y;
    float direction;
};

// Client -> Server or Server -> Client: Chat
struct ChatMessageData : Message {
    ChatMessageData() { type = MessageType::ChatMessage; }
    int playerId;
    std::string text;
};

// Server -> Client: Player joined notification
struct PlayerJoinedMessage : Message {
    PlayerJoinedMessage() { type = MessageType::PlayerJoined; }
    int playerId;
    std::string playerName;
    float x;
    float y;
};

// Server -> Client: Player left notification
struct PlayerLeftMessage : Message {
    PlayerLeftMessage() { type = MessageType::PlayerLeft; }
    int playerId;
};

// Server -> Client: Broadcast chat message
struct ChatBroadcastMessage : Message {
    ChatBroadcastMessage() { type = MessageType::ChatBroadcast; }
    int fromPlayerId;
    std::string playerName;
    std::string text;
};
