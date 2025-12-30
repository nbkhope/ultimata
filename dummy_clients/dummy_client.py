#!/usr/bin/env python3
"""
Simple Python dummy client for testing Ultimata multiplayer
Usage: python dummy_client.py [name] [x] [y] [host] [port]
"""

import socket
import struct
import time
import random
import sys
import threading

class DummyClient:
    def __init__(self, name="DummyPlayer", start_x=100, start_y=100, host="localhost", port=8099):
        self.name = name[:15]  # Max 15 chars for name
        self.x = float(start_x)
        self.y = float(start_y) 
        self.direction = 0
        self.player_id = random.randint(1000, 9999)  # Random ID
        self.host = host
        self.port = port
        self.sock = None
        self.connected = False
        self.running = True
        
    def connect(self):
        """Connect to the game server"""
        try:
            self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.sock.connect((self.host, self.port))
            self.connected = True
            print(f"[{self.name}] Connected to {self.host}:{self.port}")
            return True
        except Exception as e:
            print(f"[{self.name}] Connection failed: {e}")
            return False
    
    def disconnect(self):
        """Disconnect from server"""
        if self.sock:
            try:
                # Send disconnect message
                disconnect_msg = "disconnect\n"
                self.sock.send(disconnect_msg.encode())
            except:
                pass
            self.sock.close()
        self.connected = False
        self.running = False
        print(f"[{self.name}] Disconnected")
    
    def send_player_state(self):
        """Send binary PLAYER_STATE packet to server"""
        if not self.connected:
            return False
            
        try:
            # Create binary packet: [command][id][x][y][direction][nameLen][name]
            # PLAYER_STATE command = 6
            command = 6
            name_bytes = self.name.encode('utf-8')
            name_len = len(name_bytes)
            
            # Pack as binary data (little-endian)
            packet = struct.pack('<I', command)  # command (4 bytes)
            packet += struct.pack('<I', self.player_id)  # id (4 bytes)  
            packet += struct.pack('<I', int(self.x))  # x (4 bytes)
            packet += struct.pack('<I', int(self.y))  # y (4 bytes)
            packet += struct.pack('<I', self.direction)  # direction (4 bytes)
            packet += struct.pack('<B', name_len)  # nameLen (1 byte)
            packet += name_bytes  # name (nameLen bytes)
            
            self.sock.send(packet)
            return True
        except Exception as e:
            print(f"[{self.name}] Failed to send state: {e}")
            return False
    
    def simulate_movement(self):
        """Simulate random movement"""
        # Random walk with bounds
        dx = random.uniform(-2.0, 2.0)
        dy = random.uniform(-2.0, 2.0)
        
        self.x += dx
        self.y += dy
        
        # Keep within reasonable bounds
        self.x = max(50, min(800, self.x))
        self.y = max(50, min(600, self.y))
        
        # Random direction change occasionally
        if random.random() < 0.1:
            self.direction = random.randint(0, 3)
    
    def run(self):
        """Main client loop"""
        if not self.connect():
            return
            
        print(f"[{self.name}] Starting at ({int(self.x)}, {int(self.y)})")
        print(f"[{self.name}] Player ID: {self.player_id}")
        print(f"[{self.name}] Press Ctrl+C to stop")
        
        try:
            while self.running:
                # Send current state
                if not self.send_player_state():
                    break
                    
                # Simulate movement
                self.simulate_movement()
                
                # Print position occasionally
                if random.random() < 0.05:  # 5% chance
                    print(f"[{self.name}] Position: ({int(self.x)}, {int(self.y)})")
                
                # Update every 100ms
                time.sleep(0.1)
                
        except KeyboardInterrupt:
            print(f"\n[{self.name}] Stopping...")
        except Exception as e:
            print(f"[{self.name}] Error: {e}")
        finally:
            self.disconnect()

def main():
    # Parse command line arguments
    name = "Player" + str(random.randint(1, 999))
    x = 100 + random.randint(0, 300)
    y = 100 + random.randint(0, 200) 
    host = "localhost"
    port = 8099
    
    if len(sys.argv) > 1:
        name = sys.argv[1]
    if len(sys.argv) > 2:
        x = int(sys.argv[2])
    if len(sys.argv) > 3:
        y = int(sys.argv[3])  
    if len(sys.argv) > 4:
        host = sys.argv[4]
    if len(sys.argv) > 5:
        port = int(sys.argv[5])
    
    print("=== Ultimata Dummy Client ===")
    print(f"Name: {name}")
    print(f"Starting position: ({x}, {y})")
    print(f"Server: {host}:{port}")
    print()
    
    client = DummyClient(name, x, y, host, port)
    client.run()

if __name__ == "__main__":
    main()