#include "MessageSerializer.h"
#include "MessageTypes.h"
#include <stdexcept>

// Polymorphic serialize - dispatches based on message type
std::vector<unsigned char> MessageSerializer::serialize(const Message& msg) {
    switch (msg.type) {
        case MessageType::PlayerMove:
            return serialize(static_cast<const PlayerMoveMessage&>(msg));
        case MessageType::ChatMessage:
            return serialize(static_cast<const ChatMessageData&>(msg));
        case MessageType::PlayerJoined:
            return serialize(static_cast<const PlayerJoinedMessage&>(msg));
        case MessageType::PlayerLeft:
            return serialize(static_cast<const PlayerLeftMessage&>(msg));
        case MessageType::ChatBroadcast:
            return serialize(static_cast<const ChatBroadcastMessage&>(msg));
        default:
            throw std::runtime_error("Unknown message type for serialization");
    }
}

// Serialize PlayerMoveMessage
std::vector<unsigned char> MessageSerializer::serialize(const PlayerMoveMessage& msg) {
    std::vector<unsigned char> buffer;
    writeUint8(buffer, static_cast<uint8_t>(msg.type));
    writeInt32(buffer, msg.playerId);
    writeFloat(buffer, msg.x);
    writeFloat(buffer, msg.y);
    writeFloat(buffer, msg.direction);
    return buffer;
}

// Serialize ChatMessageData
std::vector<unsigned char> MessageSerializer::serialize(const ChatMessageData& msg) {
    std::vector<unsigned char> buffer;
    writeUint8(buffer, static_cast<uint8_t>(msg.type));
    writeInt32(buffer, msg.playerId);
    writeString(buffer, msg.text);
    return buffer;
}

// Serialize PlayerJoinedMessage
std::vector<unsigned char> MessageSerializer::serialize(const PlayerJoinedMessage& msg) {
    std::vector<unsigned char> buffer;
    writeUint8(buffer, static_cast<uint8_t>(msg.type));
    writeInt32(buffer, msg.playerId);
    writeString(buffer, msg.playerName);
    writeFloat(buffer, msg.x);
    writeFloat(buffer, msg.y);
    return buffer;
}

// Serialize PlayerLeftMessage
std::vector<unsigned char> MessageSerializer::serialize(const PlayerLeftMessage& msg) {
    std::vector<unsigned char> buffer;
    writeUint8(buffer, static_cast<uint8_t>(msg.type));
    writeInt32(buffer, msg.playerId);
    return buffer;
}

// Serialize ChatBroadcastMessage
std::vector<unsigned char> MessageSerializer::serialize(const ChatBroadcastMessage& msg) {
    std::vector<unsigned char> buffer;
    writeUint8(buffer, static_cast<uint8_t>(msg.type));
    writeInt32(buffer, msg.fromPlayerId);
    writeString(buffer, msg.playerName);
    writeString(buffer, msg.text);
    return buffer;
}

// Peek at message type without deserializing
MessageType MessageSerializer::peekType(const std::vector<unsigned char>& data) {
    if (data.empty()) {
        throw std::runtime_error("Cannot peek type from empty data");
    }
    return static_cast<MessageType>(data[0]);
}

// Deserialize PlayerMoveMessage
std::unique_ptr<PlayerMoveMessage> MessageSerializer::deserializePlayerMove(const std::vector<unsigned char>& data) {
    size_t offset = 0;
    auto msg = std::make_unique<PlayerMoveMessage>();

    msg->type = static_cast<MessageType>(readUint8(data.data(), offset));
    msg->playerId = readInt32(data.data(), offset);
    msg->x = readFloat(data.data(), offset);
    msg->y = readFloat(data.data(), offset);
    msg->direction = readFloat(data.data(), offset);

    return msg;
}

// Deserialize ChatMessageData
std::unique_ptr<ChatMessageData> MessageSerializer::deserializeChat(const std::vector<unsigned char>& data) {
    size_t offset = 0;
    auto msg = std::make_unique<ChatMessageData>();

    msg->type = static_cast<MessageType>(readUint8(data.data(), offset));
    msg->playerId = readInt32(data.data(), offset);
    msg->text = readString(data.data(), offset, data.size() - offset);

    return msg;
}

// Deserialize PlayerJoinedMessage
std::unique_ptr<PlayerJoinedMessage> MessageSerializer::deserializePlayerJoined(const std::vector<unsigned char>& data) {
    size_t offset = 0;
    auto msg = std::make_unique<PlayerJoinedMessage>();

    msg->type = static_cast<MessageType>(readUint8(data.data(), offset));
    msg->playerId = readInt32(data.data(), offset);
    msg->playerName = readString(data.data(), offset, data.size() - offset);
    msg->x = readFloat(data.data(), offset);
    msg->y = readFloat(data.data(), offset);

    return msg;
}

// Deserialize PlayerLeftMessage
std::unique_ptr<PlayerLeftMessage> MessageSerializer::deserializePlayerLeft(const std::vector<unsigned char>& data) {
    size_t offset = 0;
    auto msg = std::make_unique<PlayerLeftMessage>();

    msg->type = static_cast<MessageType>(readUint8(data.data(), offset));
    msg->playerId = readInt32(data.data(), offset);

    return msg;
}

// Deserialize ChatBroadcastMessage
std::unique_ptr<ChatBroadcastMessage> MessageSerializer::deserializeChatBroadcast(const std::vector<unsigned char>& data) {
    size_t offset = 0;
    auto msg = std::make_unique<ChatBroadcastMessage>();

    msg->type = static_cast<MessageType>(readUint8(data.data(), offset));
    msg->fromPlayerId = readInt32(data.data(), offset);
    msg->playerName = readString(data.data(), offset, data.size() - offset);
    msg->text = readString(data.data(), offset, data.size() - offset);

    return msg;
}

// Helper: Write uint8
void MessageSerializer::writeUint8(std::vector<unsigned char>& buffer, uint8_t value) {
    buffer.push_back(value);
}

// Helper: Write int32
void MessageSerializer::writeInt32(std::vector<unsigned char>& buffer, int32_t value) {
    buffer.push_back((value >> 0) & 0xFF);
    buffer.push_back((value >> 8) & 0xFF);
    buffer.push_back((value >> 16) & 0xFF);
    buffer.push_back((value >> 24) & 0xFF);
}

// Helper: Write float
void MessageSerializer::writeFloat(std::vector<unsigned char>& buffer, float value) {
    uint32_t intValue;
    std::memcpy(&intValue, &value, sizeof(float));
    writeInt32(buffer, static_cast<int32_t>(intValue));
}

// Helper: Write string (length-prefixed)
void MessageSerializer::writeString(std::vector<unsigned char>& buffer, const std::string& value) {
    writeInt32(buffer, static_cast<int32_t>(value.size()));
    buffer.insert(buffer.end(), value.begin(), value.end());
}

// Helper: Read uint8
uint8_t MessageSerializer::readUint8(const unsigned char* data, size_t& offset) {
    return data[offset++];
}

// Helper: Read int32
int32_t MessageSerializer::readInt32(const unsigned char* data, size_t& offset) {
    int32_t value =
        (static_cast<int32_t>(data[offset + 0]) << 0) |
        (static_cast<int32_t>(data[offset + 1]) << 8) |
        (static_cast<int32_t>(data[offset + 2]) << 16) |
        (static_cast<int32_t>(data[offset + 3]) << 24);
    offset += 4;
    return value;
}

// Helper: Read float
float MessageSerializer::readFloat(const unsigned char* data, size_t& offset) {
    int32_t intValue = readInt32(data, offset);
    float floatValue;
    std::memcpy(&floatValue, &intValue, sizeof(float));
    return floatValue;
}

// Helper: Read string (length-prefixed)
std::string MessageSerializer::readString(const unsigned char* data, size_t& offset, size_t maxSize) {
    int32_t length = readInt32(data, offset);
    if (length < 0 || static_cast<size_t>(length) > maxSize) {
        throw std::runtime_error("Invalid string length");
    }

    std::string value(reinterpret_cast<const char*>(data + offset), length);
    offset += length;
    return value;
}
