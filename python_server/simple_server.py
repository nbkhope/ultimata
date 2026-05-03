#!/usr/bin/env python3
"""
Simple Python server for Ultimata 2 game client testing
Compatible with the SDL client - listens on port 8099
"""

import socket
import struct
import threading
import time
import signal
import sys

class GameServer:
    def __init__(self, host='localhost', port=8099):
        self.host = host
        self.port = port
        self.running = True
        self.clients = []
        self.socket = None

    def start(self):
        """Start the server"""
        try:
            self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
            self.socket.bind((self.host, self.port))
            self.socket.listen(16)  # Match C++ MAX_SOCKETS
            # On Windows, a blocking accept() can delay Ctrl+C handling.
            # Poll periodically so signals are processed promptly.
            self.socket.settimeout(1.0)

            print(f"🚀 Python server listening on {self.host}:{self.port}")
            print("📱 Your SDL client can now connect!")
            print("⏹️  Press Ctrl+C to stop the server")

            while self.running:
                try:
                    client_socket, address = self.socket.accept()
                    print(f"✅ Client connected from {address}")

                    # Handle each client in a separate thread
                    client_thread = threading.Thread(
                        target=self.handle_client,
                        args=(client_socket, address)
                    )
                    client_thread.daemon = True
                    client_thread.start()

                except socket.timeout:
                    continue
                except KeyboardInterrupt:
                    print("\n🛑 Keyboard interrupt received")
                    self.running = False
                    break

                except socket.error as e:
                    if self.running:
                        print(f"❌ Socket error: {e}")
                    break

        except Exception as e:
            print(f"💥 Failed to start server: {e}")
        finally:
            self.shutdown()

    def handle_client(self, client_socket, address):
        """Handle individual client connection"""
        self.clients.append(client_socket)

        try:
            print(f"🎮 Handling client {address}")

            # Send a welcome message (32-bit integer as example)
            welcome_msg = struct.pack('!I', 12345)  # Network byte order, unsigned int
            client_socket.send(welcome_msg)
            print(f"📤 Sent welcome message to {address}")

            # Keep connection alive and handle messages
            while self.running:
                try:
                    # Set a timeout so we can check if server is shutting down
                    client_socket.settimeout(1.0)
                    data = client_socket.recv(1024)  # Match MAX_PACKET size

                    if not data:
                        print(f"📪 Client {address} disconnected")
                        break

                    print(f"📨 Received {len(data)} bytes from {address}: {data.hex()}")

                    # Echo the data back (simple test)
                    client_socket.send(data)
                    print(f"📤 Echoed data back to {address}")

                except socket.timeout:
                    continue  # Check if server is still running
                except socket.error as e:
                    print(f"🔌 Client {address} connection error: {e}")
                    break

        except Exception as e:
            print(f"💥 Error handling client {address}: {e}")
        finally:
            if client_socket in self.clients:
                self.clients.remove(client_socket)
            client_socket.close()
            print(f"👋 Client {address} connection closed")

    def shutdown(self):
        """Clean shutdown of server"""
        print("\n🛑 Shutting down server...")
        self.running = False

        # Close all client connections
        for client in self.clients[:]:
            try:
                client.close()
            except:
                pass
        self.clients.clear()

        # Close server socket
        if self.socket:
            try:
                self.socket.close()
            except:
                pass

        print("✅ Server shutdown complete")

def signal_handler(signum, frame):
    """Handle Ctrl+C gracefully"""
    print(f"\n🛑 Received signal {signum}")
    server.shutdown()
    sys.exit(0)

if __name__ == "__main__":
    # Set up signal handlers for graceful shutdown
    signal.signal(signal.SIGINT, signal_handler)
    signal.signal(signal.SIGTERM, signal_handler)

    server = GameServer()

    try:
        server.start()
    except KeyboardInterrupt:
        server.shutdown()
    except SystemExit:
        pass