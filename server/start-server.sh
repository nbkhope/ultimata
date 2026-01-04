#!/bin/bash

echo "Starting Ultimata Server..."

# Get the directory where this script is located
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

# Try different possible executable locations
if [ -f "build/ultimata-server" ]; then
    echo "Found executable at build/ultimata-server"
    ./build/ultimata-server
elif [ -f "build/Release/ultimata-server" ]; then
    echo "Found executable at build/Release/ultimata-server"
    ./build/Release/ultimata-server
elif [ -f "build/Debug/ultimata-server" ]; then
    echo "Found executable at build/Debug/ultimata-server"
    ./build/Debug/ultimata-server
else
    echo "Server executable not found!"
    echo "Please build the project first:"
    echo "  mkdir -p build"
    echo "  cd build"
    echo "  cmake .."
    echo "  make"
    exit 1
fi