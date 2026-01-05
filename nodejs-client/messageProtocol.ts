// Message types (must match server MessageProtocol.h)
export enum MessageType {
    CHAT = 1,
    PLAYER_MOVE = 2,
    PLAYER_CONNECT = 3,
    PLAYER_DISCONNECT = 4
}

// Helper to create length-prefixed messages
export class MessageBuilder {
    private buffer: Buffer[] = [];

    start(type: MessageType) {
        this.buffer = [];
        // Reserve 4 bytes for length (will be filled in build())
        this.buffer.push(Buffer.alloc(4));
        // Add message type
        this.writeUint32(type);
    }

    writeUint32(value: number) {
        const buf = Buffer.alloc(4);
        buf.writeUInt32BE(value, 0); // Big-endian (network byte order)
        this.buffer.push(buf);
    }

    writeString(str: string) {
        // Write string length
        this.writeUint32(str.length);
        // Write string data
        this.buffer.push(Buffer.from(str, 'utf8'));
    }

    build(): Buffer {
        // Concatenate all buffers
        const data = Buffer.concat(this.buffer);
        // Calculate message length (everything after the 4-byte length prefix)
        const messageLength = data.length - 4;
        // Write length at the beginning
        data.writeUInt32BE(messageLength, 0);
        return data;
    }
}

// Helper to parse messages
export class MessageReader {
    private pos: number = 0;

    constructor(private data: Buffer) {}

    readMessageType(): MessageType {
        return this.readUint32() as MessageType;
    }

    readUint32(): number {
        const value = this.data.readUInt32BE(this.pos);
        this.pos += 4;
        return value;
    }

    readString(): string {
        const length = this.readUint32();
        const str = this.data.toString('utf8', this.pos, this.pos + length);
        this.pos += length;
        return str;
    }

    hasMoreData(): boolean {
        return this.pos < this.data.length;
    }
}

// Message parser for handling incoming data stream
export class MessageParser {
    private receiveBuffer = Buffer.alloc(0);

    addData(data: Buffer): void {
        this.receiveBuffer = Buffer.concat([this.receiveBuffer, data]);
    }

    parseMessages(handler: (msgType: MessageType, reader: MessageReader) => void): void {
        while (this.receiveBuffer.length >= 4) {
            // Read message length
            const messageLength = this.receiveBuffer.readUInt32BE(0);
            
            // Check if we have the complete message
            if (this.receiveBuffer.length >= 4 + messageLength) {
                // Extract message (skip 4-byte length prefix)
                const messageData = this.receiveBuffer.slice(4, 4 + messageLength);
                
                try {
                    const reader = new MessageReader(messageData);
                    const msgType = reader.readMessageType();
                    handler(msgType, reader);
                } catch (err) {
                    console.error('Error parsing message:', err);
                }
                
                // Remove processed message from buffer
                this.receiveBuffer = this.receiveBuffer.slice(4 + messageLength);
            } else {
                // Incomplete message, wait for more data
                break;
            }
        }
    }

    clear(): void {
        this.receiveBuffer = Buffer.alloc(0);
    }
}
