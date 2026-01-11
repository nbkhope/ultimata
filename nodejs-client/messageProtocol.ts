// Message types (must match server MessageTypes.h)
export const MessageType = {
    // Client -> Server
    PLAYER_MOVE: 1,
    CHAT_MESSAGE: 2,
    PLAYER_ACTION: 3,

    // Server -> Client
    PLAYER_JOINED: 10,
    PLAYER_LEFT: 11,
    GAME_STATE_UPDATE: 12,
    CHAT_BROADCAST: 13,

    // Bidirectional
    PING: 20,
    PONG: 21
} as const;

// Helper to create messages (new protocol - no length prefix, little-endian)
export class MessageBuilder {
    private buffer: Buffer[] = [];

    start(type: number) {
        this.buffer = [];
        // Add message type (1 byte)
        this.buffer.push(Buffer.from([type]));
    }

    writeUint8(value: number) {
        this.buffer.push(Buffer.from([value]));
    }

    writeUint32(value: number) {
        const buf = Buffer.alloc(4);
        buf.writeUInt32LE(value, 0); // Little-endian
        this.buffer.push(buf);
    }

    writeFloat(value: number) {
        const buf = Buffer.alloc(4);
        buf.writeFloatLE(value, 0); // Little-endian
        this.buffer.push(buf);
    }

    writeString(str: string) {
        // Write string length (4 bytes, little-endian)
        this.writeUint32(str.length);
        // Write string data
        this.buffer.push(Buffer.from(str, 'utf8'));
    }

    build(): Buffer {
        return Buffer.concat(this.buffer);
    }
}

// Helper to parse messages (new protocol - little-endian)
export class MessageReader {
    private pos: number = 0;

    constructor(private data: Buffer) {}

    readMessageType(): number {
        const value = this.data.readUInt8(this.pos);
        this.pos += 1;
        return value;
    }

    readUint8(): number {
        const value = this.data.readUInt8(this.pos);
        this.pos += 1;
        return value;
    }

    readUint32(): number {
        const value = this.data.readUInt32LE(this.pos);
        this.pos += 4;
        return value;
    }

    readFloat(): number {
        const value = this.data.readFloatLE(this.pos);
        this.pos += 4;
        return value;
    }

    readString(): string {
        const length = this.readUint32();
        const str = this.data.toString('utf8', this.pos, this.pos + length);
        this.pos += length;
        return str;
    }
}

// Parser for handling streaming data (new protocol - no length prefix)
export class MessageParser {
    private buffer: Buffer = Buffer.alloc(0);

    addData(data: Buffer) {
        this.buffer = Buffer.concat([this.buffer, data]);
    }

    parseMessages(callback: (msgType: number, reader: MessageReader) => void) {
        while (this.buffer.length > 0) {
            // Try to read message type (1 byte)
            if (this.buffer.length < 1) {
                break;
            }

            const msgType = this.buffer.readUInt8(0);

            // Determine expected message length based on type
            let expectedLength = 1; // type byte

            switch (msgType) {
                case MessageType.PLAYER_MOVE:
                    // playerId (4) + x (4) + y (4) + direction (1)
                    expectedLength += 4 + 4 + 4 + 1;
                    break;
                case MessageType.CHAT_MESSAGE:
                    // playerId (4) + string length (4) + string data
                    if (this.buffer.length < 9) return; // Need at least type + playerId + length
                    const chatLength = this.buffer.readUInt32LE(5);
                    expectedLength += 4 + 4 + chatLength;
                    break;
                case MessageType.CHAT_BROADCAST:
                    // fromPlayerId (4) + playerName (4 + str) + text (4 + str)
                    if (this.buffer.length < 9) return;
                    const nameLen = this.buffer.readUInt32LE(5);
                    if (this.buffer.length < 9 + nameLen + 4) return;
                    const textLen = this.buffer.readUInt32LE(9 + nameLen);
                    expectedLength += 4 + 4 + nameLen + 4 + textLen;
                    break;
                case MessageType.PLAYER_JOINED:
                    // playerId (4) + playerName (4 + str) + x (4) + y (4)
                    if (this.buffer.length < 9) return;
                    const joinNameLen = this.buffer.readUInt32LE(5);
                    expectedLength += 4 + 4 + joinNameLen + 4 + 4;
                    break;
                case MessageType.PLAYER_LEFT:
                    // playerId (4)
                    expectedLength += 4;
                    break;
                default:
                    console.error(`Unknown message type: ${msgType}`);
                    // Skip this byte and try again
                    this.buffer = this.buffer.subarray(1);
                    continue;
            }

            // Check if we have the full message
            if (this.buffer.length < expectedLength) {
                break; // Wait for more data
            }

            // Extract the message
            const messageData = this.buffer.subarray(0, expectedLength);
            this.buffer = this.buffer.subarray(expectedLength);

            // Parse and handle the message
            const reader = new MessageReader(messageData);
            reader.readMessageType(); // Skip type since we already read it
            callback(msgType, reader);
        }
    }
}
