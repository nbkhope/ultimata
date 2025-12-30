#!/bin/bash

# Test script for Ultimata multiplayer
# First, start the server in the background
echo "Starting server..."
cd server
./build/server.exe &
SERVER_PID=$!
echo "Server started with PID $SERVER_PID"

# Wait a moment for server to start up
sleep 2

# Go back to main directory and start first client
cd ..
echo "Starting first client..."
./build/Ultimata.exe &
CLIENT1_PID=$!

# Wait a moment, then start second client
sleep 2
echo "Starting second client..."
./build/Ultimata.exe &
CLIENT2_PID=$!

echo ""
echo "Both clients should now be running!"
echo "Move around in either client to see if the other player appears."
echo ""
echo "To stop everything, press Ctrl+C in this terminal or run:"
echo "kill $SERVER_PID $CLIENT1_PID $CLIENT2_PID"

# Wait for user to stop the test
wait