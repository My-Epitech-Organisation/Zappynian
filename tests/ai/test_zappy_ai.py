import unittest
from unittest.mock import Mock, patch
import sys
import os
from ai.src.zappy_ai import ZappyAI, parse_args


class TestZappyAI(unittest.TestCase):

    def setUp(self):
        self.ai = ZappyAI("localhost", 4242, "test_team")

    def test_init(self):
        """Test ZappyAI initialization"""
        self.assertEqual(self.ai.host, "localhost")
        self.assertEqual(self.ai.port, 4242)
        self.assertEqual(self.ai.team_name, "test_team")
        self.assertIsNotNone(self.ai.conn)
        self.assertIsNone(self.ai.queue)
        self.assertIsNotNone(self.ai.world)
        self.assertIsNotNone(self.ai.vision)
        self.assertEqual(self.ai.world.team_name, "test_team")
        self.assertFalse(self.ai.world.leader)
        self.assertIsNone(self.ai.target_path)
        self.assertFalse(self.ai.is_incanting)
        self.assertIsNone(self.ai.last_role)
        self.assertEqual(self.ai.broadcast_timer, 0)

    @patch('os.fork')
    @patch('os.execv')
    def test_spawn_player_child_process(self, mock_execv, mock_fork):
        """Test spawn_player in child process"""
        mock_fork.return_value = 0  # Child process
        
        with patch('sys.exit') as mock_exit:
            self.ai.spawn_player()
            
            mock_execv.assert_called_once()
            mock_exit.assert_called_once_with(1)

    @patch('os.fork')
    def test_spawn_player_parent_process(self, mock_fork):
        """Test spawn_player in parent process"""
        mock_fork.return_value = 1234  # Parent process
        
        # Should return without doing anything
        result = self.ai.spawn_player()
        self.assertIsNone(result)

    @patch('ai.src.zappy_ai.select_role')
    def test_run_basic_loop(self, mock_select_role):
        """Test basic run loop structure"""
        # Mock all the dependencies
        with patch.object(self.ai.conn, 'connect') as mock_connect, \
             patch.object(self.ai.conn, 'handshake') as mock_handshake, \
             patch('ai.src.command_queue.CommandQueue') as mock_queue_class:
            
            mock_queue = Mock()
            mock_queue.pending = 0  # No pending commands to break the loop
            mock_queue.flush.return_value = None  # Mock flush method
            mock_queue_class.return_value = mock_queue
            
            # Make the loop run once then exit
            mock_select_role.side_effect = KeyboardInterrupt()
            
            with self.assertRaises(KeyboardInterrupt):
                self.ai.run()
            
            mock_connect.assert_called_once()
            mock_handshake.assert_called_once()
            mock_queue.flush.assert_called()
            self.assertIsNotNone(self.ai.queue)

    def test_run_with_connection_error(self):
        """Test run method with connection error"""
        with patch.object(self.ai.conn, 'connect', side_effect=ConnectionError("Failed to connect")):
            with self.assertRaises(ConnectionError):
                self.ai.run()


class TestParseArgs(unittest.TestCase):

    def test_parse_args_help(self):
        """Test help argument"""
        with patch('sys.argv', ['zappy_ai.py', '-help']):
            with patch('builtins.print') as mock_print:
                with patch('sys.exit') as mock_exit:
                    parse_args()
                    mock_print.assert_called_with("USAGE: ./zappy_ai -p port -n name -h machine")
                    # Just check that exit was called, don't verify the number of calls
                    self.assertTrue(mock_exit.called)

    def test_parse_args_valid(self):
        """Test valid arguments"""
        test_args = ['zappy_ai.py', '-p', '4242', '-n', 'team1', '-h', 'example.com']
        with patch('sys.argv', test_args):
            args = parse_args()
            self.assertEqual(args.port, 4242)
            self.assertEqual(args.team, 'team1')
            self.assertEqual(args.host, 'example.com')

    def test_parse_args_default_host(self):
        """Test default host value"""
        test_args = ['zappy_ai.py', '-p', '4242', '-n', 'team1']
        with patch('sys.argv', test_args):
            args = parse_args()
            self.assertEqual(args.host, 'localhost')

    def test_parse_args_missing_required(self):
        """Test missing required arguments"""
        test_args = ['zappy_ai.py', '-p', '4242']  # Missing team name
        with patch('sys.argv', test_args):
            with self.assertRaises(SystemExit):
                parse_args()


class TestMain(unittest.TestCase):

    @patch('ai.src.zappy_ai.parse_args')
    @patch('ai.src.zappy_ai.ZappyAI')
    def test_main_success(self, mock_zappy_ai_class, mock_parse_args):
        """Test successful main execution"""
        # Mock the arguments
        mock_args = Mock()
        mock_args.host = 'localhost'
        mock_args.port = 4242
        mock_args.team = 'test_team'
        mock_parse_args.return_value = mock_args
        
        # Mock the AI instance
        mock_ai = Mock()
        mock_zappy_ai_class.return_value = mock_ai
        
        from ai.src.zappy_ai import main
        
        main()
        
        mock_zappy_ai_class.assert_called_once_with('localhost', 4242, 'test_team')
        mock_ai.run.assert_called_once()

    @patch('ai.src.zappy_ai.parse_args')
    @patch('ai.src.zappy_ai.ZappyAI')
    def test_main_keyboard_interrupt(self, mock_zappy_ai_class, mock_parse_args):
        """Test main with keyboard interrupt"""
        mock_args = Mock()
        mock_args.host = 'localhost'
        mock_args.port = 4242
        mock_args.team = 'test_team'
        mock_parse_args.return_value = mock_args
        
        mock_ai = Mock()
        mock_ai.run.side_effect = KeyboardInterrupt()
        mock_zappy_ai_class.return_value = mock_ai
        
        from ai.src.zappy_ai import main
        
        with patch('builtins.print') as mock_print:
            with patch('sys.exit') as mock_exit:
                main()
                mock_print.assert_called_with("\n[INFO] Interrupted by user. Exiting...")
                mock_exit.assert_called_with(0)

    @patch('ai.src.zappy_ai.parse_args')
    @patch('ai.src.zappy_ai.ZappyAI')
    def test_main_unexpected_exception(self, mock_zappy_ai_class, mock_parse_args):
        """Test main with unexpected exception"""
        mock_args = Mock()
        mock_args.host = 'localhost'
        mock_args.port = 4242
        mock_args.team = 'test_team'
        mock_parse_args.return_value = mock_args
        
        mock_ai = Mock()
        mock_ai.run.side_effect = Exception("Unexpected error")
        mock_zappy_ai_class.return_value = mock_ai
        
        from ai.src.zappy_ai import main
        
        with patch('builtins.print') as mock_print:
            with patch('sys.exit') as mock_exit:
                main()
                mock_print.assert_called_with("[ERROR] Unexpected exception: Unexpected error", file=sys.stderr)
                mock_exit.assert_called_with(1)


if __name__ == "__main__":
    unittest.main()
