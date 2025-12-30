#!/usr/bin/env python3
"""
Test script to verify the Python server works
This simulates what your SDL client does
"""

import socket
import struct

def test_connection():
    print("🧪 Testing connection to Python server...")
    
    try:
        # Connect to the server (same as SDL client)
        client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        client.connect(('localhost', 8099))
        print("✅ Connected to server!")
        
        # Receive welcome message
        welcome_data = client.recv(4)  # 4 bytes for unsigned int
        if len(welcome_data) == 4:
            welcome_num = struct.unpack('!I', welcome_data)[0]
            print(f"📨 Received welcome message: {welcome_num}")
        
        # Send a test message
        test_message = b"Hello from test client!"
        client.send(test_message)
        print(f"📤 Sent: {test_message}")
        
        # Receive echo
        echo_data = client.recv(len(test_message))
        print(f"📨 Received echo: {echo_data}")
        
        client.close()
        print("✅ Test completed successfully!")
        
    except Exception as e:
        print(f"❌ Test failed: {e}")

if __name__ == "__main__":
    test_connection()