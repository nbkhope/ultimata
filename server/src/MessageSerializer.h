#pragma once

#include "MessageTypes.h"
#include <vector>
#include <memory>
#include <cstring>

class MessageSerializer {
public:
    // Serialize messages to bytes
    static std::vector<unsigned char> serialize(const Message& msg);
    static std::vector<unsigned char> serialize(const PlayerMoveMessage& msg);
    static std::vector<unsigned char> serialize(const ChatMessageData& msg);
    static std::vector<unsigned char> serialize(const PlayerJoinedMessage& msg);
    static std::vector<unsigned char> serialize(const PlayerLeftMessage& msg);
    static std::vector<unsigned char> serialize(const ChatBroadcastMessage& msg);

    // Deserialize bytes to messages
    static MessageType peekType(const std::vector<unsigned char>& data);
    static std::unique_ptr<PlayerMoveMessage> deserializePlayerMove(const std::vector<unsigned char>& data);
    static std::unique_ptr<ChatMessageData> deserializeChat(const std::vector<unsigned char>& data);
    static std::unique_ptr<PlayerJoinedMessage> deserializePlayerJoined(const std::vector<unsigned char>& data);
    static std::unique_ptr<PlayerLeftMessage> deserializePlayerLeft(const std::vector<unsigned char>& data);
    static std::unique_ptr<ChatBroadcastMessage> deserializeChatBroadcast(const std::vector<unsigned char>& data);

private:
    // Helper functions for writing/reading data
    static void writeUint8(std::vector<unsigned char>& buffer, uint8_t value);
    static void writeInt32(std::vector<unsigned char>& buffer, int32_t value);
    static void writeFloat(std::vector<unsigned char>& buffer, float value);
    static void writeString(std::vector<unsigned char>& buffer, const std::string& value);

    static uint8_t readUint8(const unsigned char* data, size_t& offset);
    static int32_t readInt32(const unsigned char* data, size_t& offset);
    static float readFloat(const unsigned char* data, size_t& offset);
    static std::string readString(const unsigned char* data, size_t& offset, size_t maxSize);
};
