
const net = require('net');
import { MessageType, MessageBuilder, MessageParser } from './messageProtocol';

const client = new net.Socket();
const port = 8099;
const parser = new MessageParser();

function sendChatMessage(message: string) {
    const builder = new MessageBuilder();
    builder.start(MessageType.CHAT);
    builder.writeString(message);
    const msg = builder.build();
    client.write(msg);
    console.log('Sent chat message:', message);
}

function sendPlayerMove(x: number, y: number) {
    const builder = new MessageBuilder();
    builder.start(MessageType.PLAYER_MOVE);
    builder.writeUint32(x);
    builder.writeUint32(y);
    const msg = builder.build();
    client.write(msg);
    console.log(`Sent move command to (${x}, ${y})`);
}

function handleConnected() {
    console.log('Connected to server');
    
    // Send a chat message after 1 second
    setTimeout(() => sendChatMessage('Hello from Node.js!'), 1000);
    
    // Send a move command after 2 seconds
    setTimeout(() => sendPlayerMove(150, 200), 2000);
}

function handleMessage(msgType: MessageType, reader: any) {
    switch (msgType) {
        case MessageType.CHAT:
            const senderId = reader.readUint32();
            const playerName = reader.readString();
            const chatMessage = reader.readString();
            console.log(`[CHAT] ${playerName} (${senderId}): ${chatMessage}`);
            break;
        
        case MessageType.PLAYER_MOVE:
            const playerId = reader.readUint32();
            const x = reader.readUint32();
            const y = reader.readUint32();
            console.log(`[MOVE] Player ${playerId} moved to (${x}, ${y})`);
            break;
        
        default:
            console.log(`Unknown message type: ${msgType}`);
    }
}

client.connect(port, '127.0.0.1', handleConnected);

client.on('data', (data: Buffer) => {
    parser.addData(data);
    parser.parseMessages(handleMessage);
});

client.on('close', () => {
    console.log('Connection closed');
});

client.on('error', (err) => {
    console.error('Connection error:', err);
});
