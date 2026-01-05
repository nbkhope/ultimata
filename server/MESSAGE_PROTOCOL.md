# Message Protocol Documentation

## Overview

This server now implements a **length-prefixed binary message protocol** to handle network messages reliably. This ensures that messages are properly framed and can be parsed correctly even when split across multiple reads or combined in a single buffer.

## Message Format

All messages follow this structure:

```
[4 bytes: message length (uint32_t)][4 bytes: message type (uint32_t)][message payload]
```

- **Length prefix (4 bytes)**: The total size of the message (excluding the 4-byte length prefix itself)
- **Message type (4 bytes)**: Identifies what kind of message this is (chat, movement, etc.)
- **Payload**: The actual message data

## How It Works

### Server Side

1. **Receiving Messages**:
   - `AsioConnection` reads data into a buffer
   - Data is accumulated in `messageBuffer`
   - `parseMessages()` extracts complete messages based on the length prefix
   - Complete messages are passed to the callback

2. **Processing Messages**:
   - `handleClientMessage()` uses `MessageReader` to parse the message
   - Based on message type, appropriate action is taken
   - Responses can be broadcast to all clients

3. **Sending Messages**:
   - Use `MessageBuilder` to construct messages
   - Call `build()` to finalize with length prefix
   - Send via `g_network->broadcastData()` or `sendData()`

### Client Side

Clients must implement the same protocol:

1. **Sending**:
   ```cpp
   MessageBuilder builder;
   builder.start(MessageType::CHAT);
   builder.writeString("Hello, world!");
   auto msg = builder.build();
   socket.send(msg.data(), msg.size());
   ```

2. **Receiving**:
   - Accumulate received bytes in a buffer
   - Check for complete messages (length prefix + data)
   - Parse with `MessageReader`

## Message Types

Defined in `MessageProtocol.h`:

- `CHAT (1)`: Chat messages
- `PLAYER_MOVE (2)`: Player movement
- `PLAYER_CONNECT (3)`: Player connection notification
- `PLAYER_DISCONNECT (4)`: Player disconnection notification

Add more types as needed.

## Example: Chat Message

### Client Sends:
```cpp
MessageBuilder builder;
builder.start(MessageType::CHAT);
builder.writeString("Hello everyone!");
auto msg = builder.build();
// Send msg.data() and msg.size() to server
```

Message structure:
```
[0-3]:   Message length (e.g., 23 bytes)
[4-7]:   Message type (1 = CHAT)
[8-11]:  String length (15)
[12-26]: "Hello everyone!"
```

### Server Receives and Broadcasts:
```cpp
// Server parses the message
MessageReader reader(data, size);
MessageType type = reader.readMessageType(); // CHAT
std::string text = reader.readString();     // "Hello everyone!"

// Server broadcasts to all clients
MessageBuilder builder;
builder.start(MessageType::CHAT);
builder.writeUint32(senderId);
builder.writeString(playerName);
builder.writeString(text);
auto response = builder.build();
g_network->broadcastData(response.data(), response.size());
```

### Other Clients Receive:
```cpp
MessageReader reader(data, size);
MessageType type = reader.readMessageType(); // CHAT
uint32_t senderId = reader.readUint32();
std::string playerName = reader.readString();
std::string message = reader.readString();
// Display: "[PlayerName]: Hello everyone!"
```

## Benefits

1. **Reliable Message Boundaries**: Messages are never split or merged incorrectly
2. **Efficient**: Binary protocol is compact and fast to parse
3. **Extensible**: Easy to add new message types
4. **Type-Safe**: MessageReader throws exceptions on buffer overruns
5. **Cross-Platform**: Works on any platform with correct endianness handling

## Implementation Files

- `MessageProtocol.h`: Message types, MessageBuilder, MessageReader
- `AsioConnection.h/cpp`: Message framing and parsing
- `Main.cpp`: Message handling logic
- `ExampleMessageUsage.cpp`: Client-side examples

## Migration Notes

If you have existing client code using raw bytes:

1. Wrap your messages with `MessageBuilder`
2. Add the 4-byte length prefix
3. Use `MessageReader` to parse incoming messages
4. Update message handlers to use the new protocol

## Testing

To test the chat functionality:

1. Build and start the server
2. Connect two clients
3. Client 1 sends: `MessageType::CHAT` with text "Hello"
4. Server broadcasts to all clients
5. Client 2 should receive the chat message with sender info
