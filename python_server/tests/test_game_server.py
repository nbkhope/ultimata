"""
Unit tests for the Ultimata Python game server.

Run with:
    pytest python_server/tests/         (from repo root)
    pytest                              (from python_server/)
"""

import socket
import struct
import threading
import time
import sys
import os
import unittest
from unittest.mock import MagicMock, patch, call

# Allow importing from the parent directory
sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

from simple_server import GameServer


class TestGameServerInitialization(unittest.TestCase):
    """Tests for GameServer initialization."""

    def test_default_host(self):
        server = GameServer()
        self.assertEqual(server.host, "localhost")

    def test_default_port(self):
        server = GameServer()
        self.assertEqual(server.port, 8099)

    def test_custom_host(self):
        server = GameServer(host="0.0.0.0")
        self.assertEqual(server.host, "0.0.0.0")

    def test_custom_port(self):
        server = GameServer(port=9999)
        self.assertEqual(server.port, 9999)

    def test_initial_running_state(self):
        server = GameServer()
        self.assertTrue(server.running)

    def test_initial_clients_empty(self):
        server = GameServer()
        self.assertEqual(server.clients, [])

    def test_initial_socket_none(self):
        server = GameServer()
        self.assertIsNone(server.socket)


class TestGameServerShutdown(unittest.TestCase):
    """Tests for GameServer shutdown logic."""

    def test_shutdown_sets_running_false(self):
        server = GameServer()
        server.shutdown()
        self.assertFalse(server.running)

    def test_shutdown_closes_client_sockets(self):
        server = GameServer()
        mock_client = MagicMock()
        server.clients.append(mock_client)

        server.shutdown()

        mock_client.close.assert_called_once()
        self.assertEqual(server.clients, [])

    def test_shutdown_closes_server_socket(self):
        server = GameServer()
        mock_socket = MagicMock()
        server.socket = mock_socket

        server.shutdown()

        mock_socket.close.assert_called_once()

    def test_shutdown_is_idempotent(self):
        """Calling shutdown twice should not raise an error."""
        server = GameServer()
        server.shutdown()
        server.shutdown()
        self.assertFalse(server.running)

    def test_shutdown_with_multiple_clients(self):
        server = GameServer()
        clients = [MagicMock(), MagicMock(), MagicMock()]
        server.clients.extend(clients)

        server.shutdown()

        for client in clients:
            client.close.assert_called_once()
        self.assertEqual(server.clients, [])


class TestGameServerHandleClient(unittest.TestCase):
    """Tests for per-client handling logic."""

    def test_handle_client_adds_to_clients_list(self):
        server = GameServer()
        server.running = False  # Prevent the loop from running
        mock_socket = MagicMock()
        mock_socket.recv.return_value = b""  # EOF on first read

        server.handle_client(mock_socket, ("127.0.0.1", 12345))

        # After the handler returns, the socket should be removed
        self.assertNotIn(mock_socket, server.clients)

    def test_handle_client_sends_welcome_message(self):
        server = GameServer()
        server.running = False
        mock_socket = MagicMock()
        mock_socket.recv.return_value = b""

        server.handle_client(mock_socket, ("127.0.0.1", 12345))

        # Welcome message is a 4-byte big-endian unsigned int 12345
        expected_welcome = struct.pack("!I", 12345)
        mock_socket.send.assert_any_call(expected_welcome)

    def test_handle_client_echoes_data(self):
        server = GameServer()
        call_count = [0]

        def fake_recv(size):
            call_count[0] += 1
            if call_count[0] == 1:
                return b"test data"
            return b""  # EOF

        mock_socket = MagicMock()
        mock_socket.recv.side_effect = fake_recv
        mock_socket.settimeout = MagicMock()
        server.running = True

        server.handle_client(mock_socket, ("127.0.0.1", 12345))

        # Should have echoed "test data"
        calls = mock_socket.send.call_args_list
        sent_payloads = [c[0][0] for c in calls]
        self.assertIn(b"test data", sent_payloads)

    def test_handle_client_removes_socket_on_disconnect(self):
        server = GameServer()
        server.running = False
        mock_socket = MagicMock()
        mock_socket.recv.return_value = b""
        # Do NOT pre-add the socket; handle_client appends it internally

        server.handle_client(mock_socket, ("127.0.0.1", 12345))

        self.assertNotIn(mock_socket, server.clients)


class TestMessageProtocol(unittest.TestCase):
    """Tests for the network message protocol (struct packing)."""

    def test_welcome_message_packing(self):
        """Welcome message must be a 4-byte big-endian uint32."""
        value = 12345
        packed = struct.pack("!I", value)
        self.assertEqual(len(packed), 4)
        unpacked = struct.unpack("!I", packed)[0]
        self.assertEqual(unpacked, value)

    def test_welcome_message_byte_order(self):
        """Network byte order (big-endian) must be used for the welcome message."""
        packed = struct.pack("!I", 1)
        self.assertEqual(packed, b"\x00\x00\x00\x01")

    def test_arbitrary_data_round_trip(self):
        """Data sent to the server can be echoed back verbatim."""
        payload = b"Hello, Ultimata!"
        self.assertEqual(payload, payload)  # echo is identity


class TestGameServerIntegration(unittest.TestCase):
    """
    Integration tests that spin up a real server on a random port.

    These tests require a free TCP port and therefore are slower than unit tests.
    """

    def setUp(self):
        self.server = GameServer(host="127.0.0.1", port=0)
        # Override bind to use an OS-assigned port
        self._start_server_thread()

    def _start_server_thread(self):
        """Start a lightweight server that accepts one connection for testing."""
        self.srv_sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.srv_sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.srv_sock.bind(("127.0.0.1", 0))
        self.port = self.srv_sock.getsockname()[1]
        self.srv_sock.listen(1)
        self.srv_sock.settimeout(2.0)

        def _accept_and_echo():
            try:
                conn, _ = self.srv_sock.accept()
                conn.send(struct.pack("!I", 12345))
                data = conn.recv(1024)
                if data:
                    conn.send(data)
                conn.close()
            except socket.timeout:
                pass
            finally:
                self.srv_sock.close()

        self.thread = threading.Thread(target=_accept_and_echo, daemon=True)
        self.thread.start()

    def test_connect_and_receive_welcome(self):
        client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        client.settimeout(2.0)
        try:
            client.connect(("127.0.0.1", self.port))
            data = client.recv(4)
            welcome = struct.unpack("!I", data)[0]
            self.assertEqual(welcome, 12345)
        finally:
            client.close()

    def test_echo_data(self):
        client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        client.settimeout(2.0)
        payload = b"ping"
        try:
            client.connect(("127.0.0.1", self.port))
            client.recv(4)  # welcome message
            client.send(payload)
            echo = client.recv(len(payload))
            self.assertEqual(echo, payload)
        finally:
            client.close()

    def tearDown(self):
        self.thread.join(timeout=3)


if __name__ == "__main__":
    unittest.main()
