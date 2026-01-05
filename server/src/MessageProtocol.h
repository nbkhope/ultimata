#pragma once
#include <cstdint>
#include <cstring>
#include <vector>
#include <string>
#include <stdexcept>

#ifdef _WIN32
#include <winsock2.h>
#else
#include <arpa/inet.h>
#endif

// Maximum allowed message size (1MB)
const uint32_t MAX_MESSAGE_SIZE = 1024 * 1024;

// Message types
enum class MessageType : uint32_t {
    CHAT = 1,
    PLAYER_MOVE = 2,
    PLAYER_CONNECT = 3,
    PLAYER_DISCONNECT = 4,
    // Add more message types as needed
};

// Helper class for building messages with length prefix
class MessageBuilder {
public:
    MessageBuilder() {}
    
    // Start a new message with a specific type
    void start(MessageType type) {
        buffer.clear();
        // Reserve space for length (will be filled later)
        buffer.resize(4, 0);
        // Add message type
        writeUint32(static_cast<uint32_t>(type));
    }
    
    // Write different data types
    void writeUint32(uint32_t value) {
        uint32_t netValue = htonl(value);
        size_t pos = buffer.size();
        buffer.resize(pos + 4);
        std::memcpy(buffer.data() + pos, &netValue, 4);
    }
    
    void writeString(const std::string& str) {
        if (str.size() > MAX_MESSAGE_SIZE) {
            throw std::runtime_error("String too large");
        }
        // Write string length first
        writeUint32(static_cast<uint32_t>(str.size()));
        // Write string data
        buffer.insert(buffer.end(), str.begin(), str.end());
    }
    
    void writeBytes(const void* data, size_t size) {
        const char* bytes = static_cast<const char*>(data);
        buffer.insert(buffer.end(), bytes, bytes + size);
    }
    
    // Finalize the message by writing the length prefix
    std::vector<char> build() {
        // Calculate message length (everything after the 4-byte length prefix)
        uint32_t messageLength = static_cast<uint32_t>(buffer.size() - 4);
        if (messageLength > MAX_MESSAGE_SIZE) {
            throw std::runtime_error("Message too large");
        }
        // Write length at the beginning in network byte order
        uint32_t netLength = htonl(messageLength);
        std::memcpy(buffer.data(), &netLength, 4);
        return buffer;
    }
    
private:
    std::vector<char> buffer;
};

// Helper class for reading messages
class MessageReader {
public:
    MessageReader(const char* data, size_t size) : data(data), size(size), pos(0) {}
    
    // Read message type
    MessageType readMessageType() {
        uint32_t type = readUint32();
        return static_cast<MessageType>(type);
    }
    
    // Read different data types
    uint32_t readUint32() {
        if (pos + 4 > size) throw std::runtime_error("Buffer overflow");
        uint32_t netValue;
        std::memcpy(&netValue, data + pos, 4);
        pos += 4;
        return ntohl(netValue);
    }
    
    std::string readString() {
        uint32_t length = readUint32();
        if (length > MAX_MESSAGE_SIZE) throw std::runtime_error("String too large");
        if (pos + length > size) throw std::runtime_error("Buffer overflow");
        std::string str(data + pos, length);
        pos += length;
        return str;
    }
    
    void readBytes(void* buffer, size_t count) {
        if (pos + count > size) throw std::runtime_error("Buffer overflow");
        std::memcpy(buffer, data + pos, count);
        pos += count;
    }
    
    bool hasMoreData() const {
        return pos < size;
    }
    
private:
    const char* data;
    size_t size;
    size_t pos;
};
