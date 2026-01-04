#!/bin/bash

# Simple test script for the Ultimata Server
# This script tests basic connectivity to the server

SERVER_HOST=${1:-localhost}
SERVER_PORT=${2:-8099}
TIMEOUT=${3:-5}

echo "Testing Ultimata Server at $SERVER_HOST:$SERVER_PORT"
echo "========================================"

# Test 1: Check if port is open
echo "1. Testing port connectivity..."
if timeout $TIMEOUT bash -c "</dev/tcp/$SERVER_HOST/$SERVER_PORT"; then
    echo "   ✓ Port $SERVER_PORT is open and accepting connections"
else
    echo "   ✗ Port $SERVER_PORT is not accessible"
    exit 1
fi

# Test 2: Try to establish a connection and send simple data
echo "2. Testing connection establishment..."
if echo "test" | timeout $TIMEOUT telnet $SERVER_HOST $SERVER_PORT 2>/dev/null | grep -q "Connected"; then
    echo "   ✓ Connection established successfully"
else
    echo "   ⚠ Connection test inconclusive (this may be normal)"
fi

# Test 3: Check if server process is running (if testing locally)
if [ "$SERVER_HOST" = "localhost" ] || [ "$SERVER_HOST" = "127.0.0.1" ]; then
    echo "3. Testing local server process..."
    if pgrep -f "ultimata-server" > /dev/null; then
        echo "   ✓ Server process is running"
    else
        echo "   ✗ Server process not found"
    fi
fi

# Test 4: Basic network statistics
echo "4. Network information..."
if command -v netstat >/dev/null 2>&1; then
    LISTENING=$(netstat -an | grep ":$SERVER_PORT.*LISTEN" | wc -l)
    if [ $LISTENING -gt 0 ]; then
        echo "   ✓ Server is listening on port $SERVER_PORT"
    else
        echo "   ✗ Server is not listening on port $SERVER_PORT"
    fi
else
    echo "   ⚠ netstat not available, skipping network check"
fi

echo ""
echo "Basic connectivity test completed!"
echo ""
echo "To test with a client, you can use:"
echo "  telnet $SERVER_HOST $SERVER_PORT"
echo "  nc $SERVER_HOST $SERVER_PORT"
echo ""
echo "For load testing, consider using:"
echo "  ab -n 100 -c 10 http://$SERVER_HOST:$SERVER_PORT/ (if HTTP)"
echo "  or custom client tools"