
import net from 'net';
import { MessageType, MessageBuilder, MessageParser } from './messageProtocol.ts';

const client = new net.Socket();
const port = 8099;
const parser = new MessageParser();
let connected = false;

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

let chatMessageInterval;
let playerMoveInterval;

function handleConnected() {
    connected = true;
    console.log('Connected to server');
    
    // Send a chat message every 2 seconds
    chatMessageInterval = setInterval(() => sendChatMessage('Hello from Node.js!'), 2000);
    
    // Send a move command every 5 seconds
    playerMoveInterval = setInterval(() => sendPlayerMove(150, 200), 5000);
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
    connected = false;
    clearInterval(chatMessageInterval);
    clearInterval(playerMoveInterval);

    console.log('Connection closed');
    process.exit(0);
});

client.on('error', (err) => {
    connected = false;
    clearInterval(chatMessageInterval);
    clearInterval(playerMoveInterval);
    
    console.error('Connection error:', err);
    process.exit(1);
});
