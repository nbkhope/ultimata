/**
 * Unit tests for MessageProtocol (MessageBuilder and MessageReader helpers).
 *
 * Tests the lower-level length-prefixed binary protocol defined in
 * MessageProtocol.h, which is used by the server networking layer.
 *
 * Build & run (from server/ directory):
 *   cmake --preset default -DBUILD_TESTING=ON -B build-tests
 *   cmake --build build-tests --target ultimata-server-tests
 *   ctest --test-dir build-tests --output-on-failure
 */

#include <gtest/gtest.h>
#include "../src/MessageProtocol.h"

// ---------------------------------------------------------------------------
// MessageBuilder
// ---------------------------------------------------------------------------

TEST(MessageBuilder, Start_ReservesLengthAndType) {
    MessageBuilder builder;
    builder.start(MessageType::CHAT);
    auto buf = builder.build();

    // Buffer has at minimum: 4 (length prefix) + 4 (type) = 8 bytes
    EXPECT_GE(buf.size(), 8u);
}

TEST(MessageBuilder, Build_LengthPrefixMatchesPayload) {
    MessageBuilder builder;
    builder.start(MessageType::CHAT);
    // No extra payload

    auto buf = builder.build();

    // First 4 bytes are the length (network byte order)
    uint32_t netLen;
    std::memcpy(&netLen, buf.data(), 4);
    uint32_t len = ntohl(netLen);

    // Length should equal the remaining bytes after the 4-byte prefix
    EXPECT_EQ(len, buf.size() - 4);
}

TEST(MessageBuilder, WriteUint32_AppearsInBuffer) {
    MessageBuilder builder;
    builder.start(MessageType::PLAYER_MOVE);
    builder.writeUint32(0xDEADBEEF);
    auto buf = builder.build();

    // Verify we can read the value back (it's in network byte order)
    // The value is written after: length(4) + type(4)
    const size_t valueOffset = 8;
    ASSERT_GE(buf.size(), valueOffset + 4);

    uint32_t netValue;
    std::memcpy(&netValue, buf.data() + valueOffset, 4);
    EXPECT_EQ(ntohl(netValue), 0xDEADBEEF);
}

TEST(MessageBuilder, WriteString_IncludesLengthAndData) {
    MessageBuilder builder;
    builder.start(MessageType::CHAT);
    builder.writeString("abc");
    auto buf = builder.build();

    // After length(4) + type(4): stringLen(4) + "abc"(3)
    const size_t strLenOffset = 8;
    ASSERT_GE(buf.size(), strLenOffset + 4 + 3);

    uint32_t netStrLen;
    std::memcpy(&netStrLen, buf.data() + strLenOffset, 4);
    EXPECT_EQ(ntohl(netStrLen), 3u);

    std::string str(buf.data() + strLenOffset + 4, 3);
    EXPECT_EQ(str, "abc");
}

TEST(MessageBuilder, BuildTwice_ProducesConsistentOutput) {
    MessageBuilder builder;
    builder.start(MessageType::CHAT);
    builder.writeUint32(42);
    auto buf1 = builder.build();
    auto buf2 = builder.build();
    EXPECT_EQ(buf1, buf2);
}

// ---------------------------------------------------------------------------
// MessageReader
// ---------------------------------------------------------------------------

TEST(MessageReader, ReadMessageType_CorrectType) {
    MessageBuilder builder;
    builder.start(MessageType::PLAYER_CONNECT);
    auto buf = builder.build();

    // MessageReader reads from the payload (after the 4-byte length prefix)
    MessageReader reader(buf.data() + 4, buf.size() - 4);
    EXPECT_EQ(reader.readMessageType(), MessageType::PLAYER_CONNECT);
}

TEST(MessageReader, ReadUint32_RoundTrip) {
    MessageBuilder builder;
    builder.start(MessageType::PLAYER_MOVE);
    builder.writeUint32(0x12345678);
    auto buf = builder.build();

    MessageReader reader(buf.data() + 4, buf.size() - 4);
    reader.readMessageType(); // skip type
    EXPECT_EQ(reader.readUint32(), 0x12345678u);
}

TEST(MessageReader, ReadString_RoundTrip) {
    const std::string testStr = "Hello, protocol!";
    MessageBuilder builder;
    builder.start(MessageType::CHAT);
    builder.writeString(testStr);
    auto buf = builder.build();

    MessageReader reader(buf.data() + 4, buf.size() - 4);
    reader.readMessageType(); // skip type
    EXPECT_EQ(reader.readString(), testStr);
}

TEST(MessageReader, ReadString_EmptyString) {
    MessageBuilder builder;
    builder.start(MessageType::CHAT);
    builder.writeString("");
    auto buf = builder.build();

    MessageReader reader(buf.data() + 4, buf.size() - 4);
    reader.readMessageType();
    EXPECT_EQ(reader.readString(), "");
}

TEST(MessageReader, HasMoreData_TrueWhenDataRemains) {
    MessageBuilder builder;
    builder.start(MessageType::CHAT);
    builder.writeUint32(99);
    auto buf = builder.build();

    MessageReader reader(buf.data() + 4, buf.size() - 4);
    reader.readMessageType(); // type only (4 bytes)
    EXPECT_TRUE(reader.hasMoreData());
}

TEST(MessageReader, HasMoreData_FalseAtEnd) {
    MessageBuilder builder;
    builder.start(MessageType::PLAYER_DISCONNECT);
    auto buf = builder.build();

    MessageReader reader(buf.data() + 4, buf.size() - 4);
    reader.readMessageType();
    EXPECT_FALSE(reader.hasMoreData());
}

TEST(MessageReader, ReadUint32_ThrowsOnOverflow) {
    // Only provide 2 bytes, reader expects 4
    char data[2] = {0x00, 0x01};
    MessageReader reader(data, 2);
    EXPECT_THROW(reader.readUint32(), std::runtime_error);
}

// ---------------------------------------------------------------------------
// MessageBuilder + MessageReader round-trip
// ---------------------------------------------------------------------------

TEST(MessageProtocol, ChatMessage_RoundTrip) {
    const std::string chatText = "Integration test message";
    const uint32_t playerId    = 7;

    MessageBuilder builder;
    builder.start(MessageType::CHAT);
    builder.writeUint32(playerId);
    builder.writeString(chatText);
    auto buf = builder.build();

    MessageReader reader(buf.data() + 4, buf.size() - 4);
    EXPECT_EQ(reader.readMessageType(), MessageType::CHAT);
    EXPECT_EQ(reader.readUint32(),      playerId);
    EXPECT_EQ(reader.readString(),      chatText);
    EXPECT_FALSE(reader.hasMoreData());
}

TEST(MessageProtocol, PlayerMove_RoundTrip) {
    MessageBuilder builder;
    builder.start(MessageType::PLAYER_MOVE);
    builder.writeUint32(42);   // playerId
    builder.writeUint32(100);  // x (stored as uint for this lower-level API)
    builder.writeUint32(200);  // y
    auto buf = builder.build();

    MessageReader reader(buf.data() + 4, buf.size() - 4);
    EXPECT_EQ(reader.readMessageType(), MessageType::PLAYER_MOVE);
    EXPECT_EQ(reader.readUint32(),      42u);
    EXPECT_EQ(reader.readUint32(),      100u);
    EXPECT_EQ(reader.readUint32(),      200u);
    EXPECT_FALSE(reader.hasMoreData());
}
