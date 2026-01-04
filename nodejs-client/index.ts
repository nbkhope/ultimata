
const net = require('net');

const client = new net.Socket();
const port = 8099;
client.connect(port, '127.0.0.1', () => {
    console.log('Connected to Boost.Asio server');
    // client.write('Hello from Node.js');
});

client.on('data', (data) => {
    console.log('Received:', data.toString());
});

client.on('close', () => {
    console.log('Connection closed');
});

setInterval(() => {
  client.write('Ping from Node.js');
}, 10000);
