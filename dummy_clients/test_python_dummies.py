#!/usr/bin/env python3
"""
Multi-client test script for Ultimata multiplayer
Starts multiple dummy Python clients automatically
"""

import subprocess
import time
import sys
import signal
import os

def start_dummy_client(name, x, y, host="localhost", port=8099):
    """Start a dummy client in a separate process"""
    cmd = [sys.executable, "dummy_client.py", name, str(x), str(y), host, str(port)]
    
    if os.name == 'nt':  # Windows
        # Start in new window
        proc = subprocess.Popen(["start", "cmd", "/k"] + cmd, shell=True)
    else:  # Linux/Mac
        # Start in new terminal
        proc = subprocess.Popen(["gnome-terminal", "--", "python3"] + cmd[1:])
    
    return proc

def main():
    print("=== Ultimata Python Dummy Client Test ===")
    print()
    
    # Parse arguments
    host = "localhost"
    port = 8099
    if len(sys.argv) > 1:
        host = sys.argv[1]
    if len(sys.argv) > 2:
        port = int(sys.argv[2])
    
    print(f"Starting dummy clients for server {host}:{port}")
    print()
    
    # Start multiple dummy clients
    clients = [
        ("Alice", 150, 100),
        ("Bob", 250, 200), 
        ("Carol", 350, 150),
        ("Dave", 200, 300)
    ]
    
    processes = []
    
    for name, x, y in clients:
        print(f"Starting {name} at ({x}, {y})")
        proc = start_dummy_client(name, x, y, host, port)
        processes.append(proc)
        time.sleep(0.5)  # Small delay between starts
    
    print()
    print("=== All dummy clients started! ===")
    print(f"Started {len(clients)} dummy players")
    print()
    print("Now start your real game client:")
    print("  make && ./build/Ultimata.exe")
    print()
    print("You should see all the dummy players as blue rectangles!")
    print()
    print("Press Ctrl+C to stop this script")
    print("(The dummy client windows will stay open)")
    
    try:
        while True:
            time.sleep(1)
    except KeyboardInterrupt:
        print("\nStopping test script...")
        print("Note: Dummy client windows are still running.")
        print("Close them manually or press Ctrl+C in each window.")

if __name__ == "__main__":
    main()