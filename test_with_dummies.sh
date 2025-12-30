#!/bin/bash

# Test script for multiplayer with dummy clients
echo "=== Ultimata Multiplayer Test with Dummy Clients ==="
echo

# Build dummy client first
echo "Building dummy client..."
g++ -IC:\Users\nkans\include -LC:\Users\nkans\lib -o dummy_client.exe dummy_client.cpp -lSDL2main -lSDL2 -lSDL2_net

if [ $? -ne 0 ]; then
    echo "Failed to build dummy client!"
    exit 1
fi

echo "✓ Dummy client built successfully"
echo

# Start server in background
echo "Starting server..."
cd server
./server.exe &
SERVER_PID=$!
echo "✓ Server started (PID: $SERVER_PID)"
cd ..

# Wait for server to start
sleep 2

# Start dummy clients
echo "Starting dummy clients..."
./dummy_client.exe Alice 150 100 &
ALICE_PID=$!
echo "✓ Started Alice at (150,100)"

./dummy_client.exe Bob 250 200 &
BOB_PID=$!  
echo "✓ Started Bob at (250,200)"

./dummy_client.exe Carol 350 150 &
CAROL_PID=$!
echo "✓ Started Carol at (350,150)"

echo
echo "=== Test Setup Complete ==="
echo "Server PID: $SERVER_PID"
echo "Alice PID: $ALICE_PID" 
echo "Bob PID: $BOB_PID"
echo "Carol PID: $CAROL_PID"
echo
echo "Now start a real client with: ./build/Ultimata.exe"
echo "You should see Alice, Bob, and Carol moving around as blue rectangles!"
echo
echo "To stop everything:"
echo "kill $SERVER_PID $ALICE_PID $BOB_PID $CAROL_PID"
echo
echo "Press Ctrl+C to stop this script..."

# Keep script running
trap "echo; echo 'Stopping all processes...'; kill $SERVER_PID $ALICE_PID $BOB_PID $CAROL_PID 2>/dev/null; exit" INT TERM

while true; do
    sleep 1
done