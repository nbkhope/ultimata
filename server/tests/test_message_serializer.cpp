/**
 * Unit tests for MessageSerializer.
 *
 * Tests serialize/deserialize round-trips for every message type
 * defined in MessageTypes.h.
 *
 * Build & run (from server/ directory):
 *   cmake --preset default -DBUILD_TESTING=ON -B build-tests
 *   cmake --build build-tests --target ultimata-server-tests
 *   ctest --test-dir build-tests --output-on-failure
 */

#include <gtest/gtest.h>
#include "../src/MessageSerializer.h"
#include "../src/MessageTypes.h"

// ---------------------------------------------------------------------------
// PlayerMoveMessage
// ---------------------------------------------------------------------------

TEST(MessageSerializer, SerializePlayerMove_RoundTrip) {
    PlayerMoveMessage original;
    original.playerId  = 42;
    original.x         = 100.5f;
    original.y         = 200.0f;
    original.direction = 1.75f;

    auto data = MessageSerializer::serialize(original);
    ASSERT_FALSE(data.empty());

    auto result = MessageSerializer::deserializePlayerMove(data);
    ASSERT_NE(result, nullptr);

    EXPECT_EQ(result->type,      MessageType::PlayerMove);
    EXPECT_EQ(result->playerId,  42);
    EXPECT_FLOAT_EQ(result->x,         100.5f);
    EXPECT_FLOAT_EQ(result->y,         200.0f);
    EXPECT_FLOAT_EQ(result->direction, 1.75f);
}

TEST(MessageSerializer, SerializePlayerMove_PeekType) {
    PlayerMoveMessage msg;
    msg.playerId  = 1;
    msg.x         = 0.0f;
    msg.y         = 0.0f;
    msg.direction = 0.0f;

    auto data = MessageSerializer::serialize(msg);
    EXPECT_EQ(MessageSerializer::peekType(data), MessageType::PlayerMove);
}

TEST(MessageSerializer, SerializePlayerMove_ZeroValues) {
    PlayerMoveMessage msg;
    msg.playerId  = 0;
    msg.x         = 0.0f;
    msg.y         = 0.0f;
    msg.direction = 0.0f;

    auto data    = MessageSerializer::serialize(msg);
    auto result  = MessageSerializer::deserializePlayerMove(data);
    ASSERT_NE(result, nullptr);

    EXPECT_EQ(result->playerId, 0);
    EXPECT_FLOAT_EQ(result->x,         0.0f);
    EXPECT_FLOAT_EQ(result->y,         0.0f);
    EXPECT_FLOAT_EQ(result->direction, 0.0f);
}

TEST(MessageSerializer, SerializePlayerMove_NegativeCoordinates) {
    PlayerMoveMessage msg;
    msg.playerId  = 7;
    msg.x         = -50.25f;
    msg.y         = -99.99f;
    msg.direction = -3.14f;

    auto data   = MessageSerializer::serialize(msg);
    auto result = MessageSerializer::deserializePlayerMove(data);
    ASSERT_NE(result, nullptr);

    EXPECT_FLOAT_EQ(result->x,         -50.25f);
    EXPECT_FLOAT_EQ(result->y,         -99.99f);
    EXPECT_FLOAT_EQ(result->direction, -3.14f);
}

// ---------------------------------------------------------------------------
// ChatMessageData
// ---------------------------------------------------------------------------

TEST(MessageSerializer, SerializeChatMessage_RoundTrip) {
    ChatMessageData original;
    original.playerId = 3;
    original.text     = "Hello, world!";

    auto data   = MessageSerializer::serialize(original);
    auto result = MessageSerializer::deserializeChat(data);
    ASSERT_NE(result, nullptr);

    EXPECT_EQ(result->type,     MessageType::ChatMessage);
    EXPECT_EQ(result->playerId, 3);
    EXPECT_EQ(result->text,     "Hello, world!");
}

TEST(MessageSerializer, SerializeChatMessage_EmptyText) {
    ChatMessageData original;
    original.playerId = 0;
    original.text     = "";

    auto data   = MessageSerializer::serialize(original);
    auto result = MessageSerializer::deserializeChat(data);
    ASSERT_NE(result, nullptr);

    EXPECT_EQ(result->text, "");
}

TEST(MessageSerializer, SerializeChatMessage_SpecialCharacters) {
    ChatMessageData original;
    original.playerId = 5;
    original.text     = "!@#$%^&*()_+ <>?/\\\"'";

    auto data   = MessageSerializer::serialize(original);
    auto result = MessageSerializer::deserializeChat(data);
    ASSERT_NE(result, nullptr);

    EXPECT_EQ(result->text, "!@#$%^&*()_+ <>?/\\\"'");
}

TEST(MessageSerializer, SerializeChatMessage_PeekType) {
    ChatMessageData msg;
    msg.playerId = 1;
    msg.text     = "test";

    auto data = MessageSerializer::serialize(msg);
    EXPECT_EQ(MessageSerializer::peekType(data), MessageType::ChatMessage);
}

// ---------------------------------------------------------------------------
// PlayerJoinedMessage
// ---------------------------------------------------------------------------

TEST(MessageSerializer, SerializePlayerJoined_RoundTrip) {
    PlayerJoinedMessage original;
    original.playerId    = 10;
    original.playerName  = "Alice";
    original.x           = 50.0f;
    original.y           = 75.0f;

    auto data   = MessageSerializer::serialize(original);
    auto result = MessageSerializer::deserializePlayerJoined(data);
    ASSERT_NE(result, nullptr);

    EXPECT_EQ(result->type,       MessageType::PlayerJoined);
    EXPECT_EQ(result->playerId,   10);
    EXPECT_EQ(result->playerName, "Alice");
    EXPECT_FLOAT_EQ(result->x,   50.0f);
    EXPECT_FLOAT_EQ(result->y,   75.0f);
}

TEST(MessageSerializer, SerializePlayerJoined_EmptyName) {
    PlayerJoinedMessage original;
    original.playerId   = 1;
    original.playerName = "";
    original.x          = 0.0f;
    original.y          = 0.0f;

    auto data   = MessageSerializer::serialize(original);
    auto result = MessageSerializer::deserializePlayerJoined(data);
    ASSERT_NE(result, nullptr);

    EXPECT_EQ(result->playerName, "");
}

TEST(MessageSerializer, SerializePlayerJoined_LongName) {
    PlayerJoinedMessage original;
    original.playerId   = 99;
    original.playerName = std::string(255, 'A'); // 255-char name
    original.x          = 1.0f;
    original.y          = 2.0f;

    auto data   = MessageSerializer::serialize(original);
    auto result = MessageSerializer::deserializePlayerJoined(data);
    ASSERT_NE(result, nullptr);

    EXPECT_EQ(result->playerName.size(), 255u);
    EXPECT_EQ(result->playerName, std::string(255, 'A'));
}

// ---------------------------------------------------------------------------
// PlayerLeftMessage
// ---------------------------------------------------------------------------

TEST(MessageSerializer, SerializePlayerLeft_RoundTrip) {
    PlayerLeftMessage original;
    original.playerId = 7;

    auto data   = MessageSerializer::serialize(original);
    auto result = MessageSerializer::deserializePlayerLeft(data);
    ASSERT_NE(result, nullptr);

    EXPECT_EQ(result->type,     MessageType::PlayerLeft);
    EXPECT_EQ(result->playerId, 7);
}

TEST(MessageSerializer, SerializePlayerLeft_ZeroId) {
    PlayerLeftMessage original;
    original.playerId = 0;

    auto data   = MessageSerializer::serialize(original);
    auto result = MessageSerializer::deserializePlayerLeft(data);
    ASSERT_NE(result, nullptr);

    EXPECT_EQ(result->playerId, 0);
}

TEST(MessageSerializer, SerializePlayerLeft_PeekType) {
    PlayerLeftMessage msg;
    msg.playerId = 1;

    auto data = MessageSerializer::serialize(msg);
    EXPECT_EQ(MessageSerializer::peekType(data), MessageType::PlayerLeft);
}

// ---------------------------------------------------------------------------
// ChatBroadcastMessage
// ---------------------------------------------------------------------------

TEST(MessageSerializer, SerializeChatBroadcast_RoundTrip) {
    ChatBroadcastMessage original;
    original.fromPlayerId = 5;
    original.playerName   = "Bob";
    original.text         = "Hi there!";

    auto data   = MessageSerializer::serialize(original);
    auto result = MessageSerializer::deserializeChatBroadcast(data);
    ASSERT_NE(result, nullptr);

    EXPECT_EQ(result->type,         MessageType::ChatBroadcast);
    EXPECT_EQ(result->fromPlayerId, 5);
    EXPECT_EQ(result->playerName,   "Bob");
    EXPECT_EQ(result->text,         "Hi there!");
}

TEST(MessageSerializer, SerializeChatBroadcast_EmptyFields) {
    ChatBroadcastMessage original;
    original.fromPlayerId = 0;
    original.playerName   = "";
    original.text         = "";

    auto data   = MessageSerializer::serialize(original);
    auto result = MessageSerializer::deserializeChatBroadcast(data);
    ASSERT_NE(result, nullptr);

    EXPECT_EQ(result->playerName, "");
    EXPECT_EQ(result->text,       "");
}

// ---------------------------------------------------------------------------
// Polymorphic serialize (via base Message&)
// ---------------------------------------------------------------------------

TEST(MessageSerializer, PolymorphicSerialize_PlayerMove) {
    PlayerMoveMessage msg;
    msg.playerId  = 1;
    msg.x         = 2.0f;
    msg.y         = 3.0f;
    msg.direction = 0.0f;

    const Message& base = msg;
    auto data = MessageSerializer::serialize(base);
    EXPECT_EQ(MessageSerializer::peekType(data), MessageType::PlayerMove);
}

TEST(MessageSerializer, PolymorphicSerialize_ChatMessage) {
    ChatMessageData msg;
    msg.playerId = 2;
    msg.text     = "poly test";

    const Message& base = msg;
    auto data = MessageSerializer::serialize(base);
    EXPECT_EQ(MessageSerializer::peekType(data), MessageType::ChatMessage);
}

TEST(MessageSerializer, PolymorphicSerialize_PlayerJoined) {
    PlayerJoinedMessage msg;
    msg.playerId   = 3;
    msg.playerName = "Carol";
    msg.x          = 0.0f;
    msg.y          = 0.0f;

    const Message& base = msg;
    auto data = MessageSerializer::serialize(base);
    EXPECT_EQ(MessageSerializer::peekType(data), MessageType::PlayerJoined);
}

TEST(MessageSerializer, PolymorphicSerialize_PlayerLeft) {
    PlayerLeftMessage msg;
    msg.playerId = 4;

    const Message& base = msg;
    auto data = MessageSerializer::serialize(base);
    EXPECT_EQ(MessageSerializer::peekType(data), MessageType::PlayerLeft);
}

TEST(MessageSerializer, PolymorphicSerialize_ChatBroadcast) {
    ChatBroadcastMessage msg;
    msg.fromPlayerId = 5;
    msg.playerName   = "Dave";
    msg.text         = "broadcast";

    const Message& base = msg;
    auto data = MessageSerializer::serialize(base);
    EXPECT_EQ(MessageSerializer::peekType(data), MessageType::ChatBroadcast);
}

// ---------------------------------------------------------------------------
// peekType edge cases
// ---------------------------------------------------------------------------

TEST(MessageSerializer, PeekType_ThrowsOnEmptyData) {
    std::vector<unsigned char> empty;
    EXPECT_THROW(MessageSerializer::peekType(empty), std::runtime_error);
}

// ---------------------------------------------------------------------------
// Data size sanity checks
// ---------------------------------------------------------------------------

TEST(MessageSerializer, PlayerMoveSize) {
    PlayerMoveMessage msg;
    msg.playerId  = 1;
    msg.x         = 0.0f;
    msg.y         = 0.0f;
    msg.direction = 0.0f;

    // 1 (type) + 4 (playerId) + 4 (x) + 4 (y) + 4 (direction) = 17
    auto data = MessageSerializer::serialize(msg);
    EXPECT_EQ(data.size(), 17u);
}

TEST(MessageSerializer, PlayerLeftSize) {
    PlayerLeftMessage msg;
    msg.playerId = 1;

    // 1 (type) + 4 (playerId) = 5
    auto data = MessageSerializer::serialize(msg);
    EXPECT_EQ(data.size(), 5u);
}
