import unittest
from unittest.mock import Mock, patch, MagicMock
import socket
import select
from ai.src.connection import Connection


class TestConnection(unittest.TestCase):

    def setUp(self):
        self.connection = Connection("localhost", 4242, "test_team")

    def test_init(self):
        """Test Connection initialization"""
        self.assertEqual(self.connection.host, "localhost")
        self.assertEqual(self.connection.port, 4242)
        self.assertEqual(self.connection.team_name, "test_team")
        self.assertIsNone(self.connection.socket)
        self.assertEqual(self.connection.buffer, b"")
        self.assertEqual(self.connection.map_size, (0, 0))
        self.assertEqual(self.connection.client_slots, 0)

    @patch('socket.create_connection')
    def test_connect_success(self, mock_create_connection):
        """Test successful connection"""
        mock_socket = Mock()
        mock_create_connection.return_value = mock_socket
        
        self.connection.connect()
        
        mock_create_connection.assert_called_once_with(("localhost", 4242))
        mock_socket.setblocking.assert_called_once_with(False)
        self.assertEqual(self.connection.socket, mock_socket)

    @patch('socket.create_connection')
    def test_connect_failure(self, mock_create_connection):
        """Test connection failure"""
        mock_create_connection.side_effect = Exception("Connection failed")
        
        with self.assertRaises(ConnectionError) as context:
            self.connection.connect()
        
        self.assertIn("Failed to connect to localhost:4242", str(context.exception))

    def test_send_command_no_socket(self):
        """Test send_command when socket is not connected"""
        with self.assertRaises(ConnectionError) as context:
            self.connection.send_command("test")
        
        self.assertEqual(str(context.exception), "Socket is not connected.")

    def test_send_command_with_socket(self):
        """Test send_command with connected socket"""
        mock_socket = Mock()
        self.connection.socket = mock_socket
        
        self.connection.send_command("Forward")
        
        mock_socket.sendall.assert_called_once_with(b"Forward\n")

    def test_send_command_already_has_newline(self):
        """Test send_command when command already has newline"""
        mock_socket = Mock()
        self.connection.socket = mock_socket
        
        self.connection.send_command("Forward\n")
        
        mock_socket.sendall.assert_called_once_with(b"Forward\n")

    def test_read_line_no_socket(self):
        """Test read_line when socket is None"""
        result = self.connection.read_line()
        self.assertIsNone(result)

    @patch('select.select')
    def test_read_line_no_data_available(self, mock_select):
        """Test read_line when no data is available"""
        mock_socket = Mock()
        self.connection.socket = mock_socket
        mock_select.return_value = ([], [], [])
        
        result = self.connection.read_line()
        
        self.assertIsNone(result)
        mock_select.assert_called_once_with([mock_socket], [], [], 0.01)

    @patch('select.select')
    def test_read_line_with_complete_line(self, mock_select):
        """Test read_line with complete line in buffer"""
        mock_socket = Mock()
        self.connection.socket = mock_socket
        mock_select.return_value = ([mock_socket], [], [])
        mock_socket.recv.return_value = b"Hello World\nNext Line"
        
        result = self.connection.read_line()
        
        self.assertEqual(result, "Hello World")
        self.assertEqual(self.connection.buffer, b"Next Line")

    @patch('select.select')
    def test_read_line_connection_closed(self, mock_select):
        """Test read_line when server closes connection"""
        mock_socket = Mock()
        self.connection.socket = mock_socket
        mock_select.return_value = ([mock_socket], [], [])
        mock_socket.recv.return_value = b""
        
        with self.assertRaises(ConnectionError):
            self.connection.read_line()

    @patch('select.select')
    def test_read_line_socket_error(self, mock_select):
        """Test read_line with socket error"""
        mock_socket = Mock()
        self.connection.socket = mock_socket
        mock_select.return_value = ([mock_socket], [], [])
        
        socket_error = socket.error()
        socket_error.errno = 104  # ECONNRESET value on Linux
        mock_socket.recv.side_effect = socket_error
        
        with self.assertRaises(ConnectionError):
            self.connection.read_line()

    @patch('select.select')
    def test_read_line_socket_error_would_block(self, mock_select):
        """Test read_line with EWOULDBLOCK error (should be ignored)"""
        mock_socket = Mock()
        self.connection.socket = mock_socket
        mock_select.return_value = ([mock_socket], [], [])
        
        socket_error = socket.error()
        socket_error.errno = socket.EWOULDBLOCK
        mock_socket.recv.side_effect = socket_error
        
        result = self.connection.read_line()
        self.assertIsNone(result)

    def test_handshake_complete_flow(self):
        """Test complete handshake flow"""
        mock_socket = Mock()
        self.connection.socket = mock_socket
        
        # Mock the read_line method to return expected handshake sequence
        read_line_responses = [
            "WELCOME",
            "5",  # client_slots
            "10 20"  # map dimensions
        ]
        
        with patch.object(self.connection, 'read_line', side_effect=read_line_responses):
            with patch.object(self.connection, 'send_command') as mock_send:
                self.connection.handshake()
                
                mock_send.assert_called_once_with("test_team")
                self.assertEqual(self.connection.client_slots, 5)
                self.assertEqual(self.connection.map_size, (10, 20))

    def test_handshake_with_debug_output(self):
        """Test handshake with debug output"""
        mock_socket = Mock()
        self.connection.socket = mock_socket
        
        read_line_responses = [
            "WELCOME",
            "3",
            "15 25"
        ]
        
        with patch.object(self.connection, 'read_line', side_effect=read_line_responses):
            with patch.object(self.connection, 'send_command'):
                with patch('builtins.print') as mock_print:
                    self.connection.handshake()
                    
                    # Check that debug messages were printed
                    debug_calls = [call for call in mock_print.call_args_list 
                                 if '[DEBUG]' in str(call)]
                    self.assertTrue(len(debug_calls) > 0)

    def test_close_with_socket(self):
        """Test close method with active socket"""
        mock_socket = Mock()
        self.connection.socket = mock_socket
        
        self.connection.close()
        
        mock_socket.close.assert_called_once()
        self.assertIsNone(self.connection.socket)

    def test_close_without_socket(self):
        """Test close method without active socket"""
        # Should not raise an exception
        self.connection.close()
        self.assertIsNone(self.connection.socket)


if __name__ == "__main__":
    unittest.main()
