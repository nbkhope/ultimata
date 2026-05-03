/**
 * Unit tests for the Ultimata Node.js client message protocol.
 *
 * Run with:
 *   npm test              (from nodejs-client/)
 *   npx vitest run        (from nodejs-client/)
 */

import { describe, it, expect, beforeEach } from 'vitest';
import { MessageType, MessageBuilder, MessageReader, MessageParser } from '../messageProtocol.ts';

// ---------------------------------------------------------------------------
// MessageType constants
// ---------------------------------------------------------------------------

describe('MessageType', () => {
    it('has the correct value for PLAYER_MOVE', () => {
        expect(MessageType.PLAYER_MOVE).toBe(1);
    });

    it('has the correct value for CHAT_MESSAGE', () => {
        expect(MessageType.CHAT_MESSAGE).toBe(2);
    });

    it('has the correct value for PLAYER_ACTION', () => {
        expect(MessageType.PLAYER_ACTION).toBe(3);
    });

    it('has the correct value for PLAYER_JOINED', () => {
        expect(MessageType.PLAYER_JOINED).toBe(10);
    });

    it('has the correct value for PLAYER_LEFT', () => {
        expect(MessageType.PLAYER_LEFT).toBe(11);
    });

    it('has the correct value for GAME_STATE_UPDATE', () => {
        expect(MessageType.GAME_STATE_UPDATE).toBe(12);
    });

    it('has the correct value for CHAT_BROADCAST', () => {
        expect(MessageType.CHAT_BROADCAST).toBe(13);
    });

    it('has the correct value for PING', () => {
        expect(MessageType.PING).toBe(20);
    });

    it('has the correct value for PONG', () => {
        expect(MessageType.PONG).toBe(21);
    });
});

// ---------------------------------------------------------------------------
// MessageBuilder
// ---------------------------------------------------------------------------

describe('MessageBuilder', () => {
    let builder: MessageBuilder;

    beforeEach(() => {
        builder = new MessageBuilder();
    });

    it('builds a buffer that starts with the message type byte', () => {
        builder.start(MessageType.PLAYER_MOVE);
        const buf = builder.build();
        expect(buf[0]).toBe(MessageType.PLAYER_MOVE);
    });

    it('builds a 1-byte buffer when only start() is called', () => {
        builder.start(MessageType.PING);
        expect(builder.build().length).toBe(1);
    });

    it('writes a uint8 value correctly', () => {
        builder.start(MessageType.PING);
        builder.writeUint8(42);
        const buf = builder.build();
        expect(buf[1]).toBe(42);
        expect(buf.length).toBe(2);
    });

    it('writes a uint32 in little-endian byte order', () => {
        builder.start(MessageType.PING);
        builder.writeUint32(0x01020304);
        const buf = builder.build();
        // Little-endian: least significant byte first
        expect(buf[1]).toBe(0x04);
        expect(buf[2]).toBe(0x03);
        expect(buf[3]).toBe(0x02);
        expect(buf[4]).toBe(0x01);
    });

    it('writes a float in little-endian format', () => {
        builder.start(MessageType.PING);
        builder.writeFloat(1.0);
        const buf = builder.build();
        // Verify the float round-trips correctly
        const floatValue = buf.readFloatLE(1);
        expect(floatValue).toBeCloseTo(1.0);
    });

    it('writes a string with a 4-byte length prefix', () => {
        builder.start(MessageType.PING);
        builder.writeString('hi');
        const buf = builder.build();
        // Bytes: [type(1)] [len(4)] [data(2)]
        expect(buf.length).toBe(1 + 4 + 2);
        const lenField = buf.readUInt32LE(1);
        expect(lenField).toBe(2);
        expect(buf.toString('utf8', 5, 7)).toBe('hi');
    });

    it('writes an empty string with zero-length prefix', () => {
        builder.start(MessageType.PING);
        builder.writeString('');
        const buf = builder.build();
        expect(buf.length).toBe(1 + 4);
        expect(buf.readUInt32LE(1)).toBe(0);
    });
});

// ---------------------------------------------------------------------------
// MessageReader
// ---------------------------------------------------------------------------

describe('MessageReader', () => {
    it('reads the message type byte', () => {
        const buf = Buffer.from([MessageType.CHAT_MESSAGE]);
        const reader = new MessageReader(buf);
        expect(reader.readMessageType()).toBe(MessageType.CHAT_MESSAGE);
    });

    it('reads a uint8 value', () => {
        const buf = Buffer.from([99]);
        const reader = new MessageReader(buf);
        expect(reader.readUint8()).toBe(99);
    });

    it('reads a uint32 in little-endian order', () => {
        const buf = Buffer.alloc(4);
        buf.writeUInt32LE(0xdeadbeef, 0);
        const reader = new MessageReader(buf);
        expect(reader.readUint32()).toBe(0xdeadbeef);
    });

    it('reads a float in little-endian order', () => {
        const buf = Buffer.alloc(4);
        buf.writeFloatLE(3.14, 0);
        const reader = new MessageReader(buf);
        expect(reader.readFloat()).toBeCloseTo(3.14, 5);
    });

    it('reads a length-prefixed string', () => {
        const str = 'hello';
        const buf = Buffer.alloc(4 + str.length);
        buf.writeUInt32LE(str.length, 0);
        buf.write(str, 4, 'utf8');
        const reader = new MessageReader(buf);
        expect(reader.readString()).toBe('hello');
    });

    it('advances the position between reads', () => {
        const buf = Buffer.alloc(2);
        buf.writeUInt8(10, 0);
        buf.writeUInt8(20, 1);
        const reader = new MessageReader(buf);
        expect(reader.readUint8()).toBe(10);
        expect(reader.readUint8()).toBe(20);
    });
});

// ---------------------------------------------------------------------------
// MessageBuilder + MessageReader round-trip
// ---------------------------------------------------------------------------

describe('MessageBuilder + MessageReader round-trip', () => {
    it('round-trips a PLAYER_MOVE message', () => {
        const builder = new MessageBuilder();
        builder.start(MessageType.PLAYER_MOVE);
        builder.writeUint32(42);   // playerId
        builder.writeFloat(100.5); // x
        builder.writeFloat(200.0); // y
        builder.writeUint8(3);     // direction
        const buf = builder.build();

        const reader = new MessageReader(buf);
        expect(reader.readMessageType()).toBe(MessageType.PLAYER_MOVE);
        expect(reader.readUint32()).toBe(42);
        expect(reader.readFloat()).toBeCloseTo(100.5, 4);
        expect(reader.readFloat()).toBeCloseTo(200.0, 4);
        expect(reader.readUint8()).toBe(3);
    });

    it('round-trips a CHAT_MESSAGE with text', () => {
        const builder = new MessageBuilder();
        builder.start(MessageType.CHAT_MESSAGE);
        builder.writeUint32(7);            // playerId
        builder.writeString('Hello!');
        const buf = builder.build();

        const reader = new MessageReader(buf);
        expect(reader.readMessageType()).toBe(MessageType.CHAT_MESSAGE);
        expect(reader.readUint32()).toBe(7);
        expect(reader.readString()).toBe('Hello!');
    });

    it('round-trips a PLAYER_JOINED message', () => {
        const builder = new MessageBuilder();
        builder.start(MessageType.PLAYER_JOINED);
        builder.writeUint32(5);          // playerId
        builder.writeString('Alice');    // playerName
        builder.writeFloat(10.0);        // x
        builder.writeFloat(20.0);        // y
        const buf = builder.build();

        const reader = new MessageReader(buf);
        expect(reader.readMessageType()).toBe(MessageType.PLAYER_JOINED);
        expect(reader.readUint32()).toBe(5);
        expect(reader.readString()).toBe('Alice');
        expect(reader.readFloat()).toBeCloseTo(10.0, 4);
        expect(reader.readFloat()).toBeCloseTo(20.0, 4);
    });

    it('round-trips a PLAYER_LEFT message', () => {
        const builder = new MessageBuilder();
        builder.start(MessageType.PLAYER_LEFT);
        builder.writeUint32(99); // playerId
        const buf = builder.build();

        const reader = new MessageReader(buf);
        expect(reader.readMessageType()).toBe(MessageType.PLAYER_LEFT);
        expect(reader.readUint32()).toBe(99);
    });

    it('round-trips a CHAT_BROADCAST message', () => {
        const builder = new MessageBuilder();
        builder.start(MessageType.CHAT_BROADCAST);
        builder.writeUint32(3);           // fromPlayerId
        builder.writeString('Bob');       // playerName
        builder.writeString('Hi there!'); // text
        const buf = builder.build();

        const reader = new MessageReader(buf);
        expect(reader.readMessageType()).toBe(MessageType.CHAT_BROADCAST);
        expect(reader.readUint32()).toBe(3);
        expect(reader.readString()).toBe('Bob');
        expect(reader.readString()).toBe('Hi there!');
    });
});

// ---------------------------------------------------------------------------
// MessageParser
// ---------------------------------------------------------------------------

describe('MessageParser', () => {
    it('parses a complete PLAYER_MOVE message', () => {
        const builder = new MessageBuilder();
        builder.start(MessageType.PLAYER_MOVE);
        builder.writeUint32(1);   // playerId
        builder.writeFloat(5.0);  // x
        builder.writeFloat(10.0); // y
        builder.writeUint8(0);    // direction
        const buf = builder.build();

        const parser = new MessageParser();
        parser.addData(buf);

        const results: Array<{ type: number; data: number[] }> = [];
        parser.parseMessages((msgType, reader) => {
            results.push({
                type: msgType,
                data: [reader.readUint32(), reader.readFloat(), reader.readFloat(), reader.readUint8()],
            });
        });

        expect(results.length).toBe(1);
        expect(results[0].type).toBe(MessageType.PLAYER_MOVE);
        expect(results[0].data[0]).toBe(1);
        expect(results[0].data[1]).toBeCloseTo(5.0, 4);
        expect(results[0].data[2]).toBeCloseTo(10.0, 4);
        expect(results[0].data[3]).toBe(0);
    });

    it('waits for more data when message is incomplete', () => {
        const parser = new MessageParser();
        // Only send partial data (just the type byte, no payload)
        parser.addData(Buffer.from([MessageType.PLAYER_LEFT]));

        const results: number[] = [];
        parser.parseMessages((msgType) => results.push(msgType));

        expect(results.length).toBe(0);
    });

    it('parses multiple messages from a single buffer', () => {
        const builder = new MessageBuilder();

        // First: PLAYER_LEFT
        builder.start(MessageType.PLAYER_LEFT);
        builder.writeUint32(1);
        const msg1 = builder.build();

        // Second: PLAYER_LEFT
        builder.start(MessageType.PLAYER_LEFT);
        builder.writeUint32(2);
        const msg2 = builder.build();

        const parser = new MessageParser();
        parser.addData(Buffer.concat([msg1, msg2]));

        const playerIds: number[] = [];
        parser.parseMessages((_type, reader) => {
            playerIds.push(reader.readUint32());
        });

        expect(playerIds).toEqual([1, 2]);
    });

    it('parses a CHAT_MESSAGE correctly', () => {
        const builder = new MessageBuilder();
        builder.start(MessageType.CHAT_MESSAGE);
        builder.writeUint32(9);            // playerId
        builder.writeString('Test chat');
        const buf = builder.build();

        const parser = new MessageParser();
        parser.addData(buf);

        const results: Array<{ playerId: number; text: string }> = [];
        parser.parseMessages((_type, reader) => {
            results.push({ playerId: reader.readUint32(), text: reader.readString() });
        });

        expect(results.length).toBe(1);
        expect(results[0].playerId).toBe(9);
        expect(results[0].text).toBe('Test chat');
    });

    it('parses a CHAT_BROADCAST correctly', () => {
        const builder = new MessageBuilder();
        builder.start(MessageType.CHAT_BROADCAST);
        builder.writeUint32(2);          // fromPlayerId
        builder.writeString('Alice');    // playerName
        builder.writeString('Greetings!');
        const buf = builder.build();

        const parser = new MessageParser();
        parser.addData(buf);

        const results: Array<{ fromId: number; name: string; text: string }> = [];
        parser.parseMessages((_type, reader) => {
            results.push({
                fromId: reader.readUint32(),
                name: reader.readString(),
                text: reader.readString(),
            });
        });

        expect(results.length).toBe(1);
        expect(results[0].fromId).toBe(2);
        expect(results[0].name).toBe('Alice');
        expect(results[0].text).toBe('Greetings!');
    });

    it('parses a PLAYER_JOINED message correctly', () => {
        const builder = new MessageBuilder();
        builder.start(MessageType.PLAYER_JOINED);
        builder.writeUint32(4);        // playerId
        builder.writeString('Carol'); // playerName
        builder.writeFloat(3.0);      // x
        builder.writeFloat(7.0);      // y
        const buf = builder.build();

        const parser = new MessageParser();
        parser.addData(buf);

        const results: Array<{ id: number; name: string; x: number; y: number }> = [];
        parser.parseMessages((_type, reader) => {
            results.push({
                id: reader.readUint32(),
                name: reader.readString(),
                x: reader.readFloat(),
                y: reader.readFloat(),
            });
        });

        expect(results.length).toBe(1);
        expect(results[0].id).toBe(4);
        expect(results[0].name).toBe('Carol');
        expect(results[0].x).toBeCloseTo(3.0, 4);
        expect(results[0].y).toBeCloseTo(7.0, 4);
    });

    it('handles an unknown message type gracefully', () => {
        // Type 0xFF is not in the protocol; parser should skip it
        const buf = Buffer.from([0xff]);
        const parser = new MessageParser();
        parser.addData(buf);

        const results: number[] = [];
        parser.parseMessages((type) => results.push(type));

        // Unknown bytes are skipped; no parsed messages
        expect(results.length).toBe(0);
    });

    it('reassembles messages split across multiple data chunks', () => {
        const builder = new MessageBuilder();
        builder.start(MessageType.PLAYER_LEFT);
        builder.writeUint32(7);
        const buf = builder.build(); // 5 bytes total

        const parser = new MessageParser();
        // Send first 3 bytes, then the rest
        parser.addData(buf.subarray(0, 3));
        const partial: number[] = [];
        parser.parseMessages((type) => partial.push(type));
        expect(partial.length).toBe(0); // not enough data yet

        parser.addData(buf.subarray(3));
        const full: number[] = [];
        parser.parseMessages((_type, reader) => full.push(reader.readUint32()));
        expect(full).toEqual([7]);
    });
});
