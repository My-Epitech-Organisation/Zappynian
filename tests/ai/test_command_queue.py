import unittest
from unittest.mock import patch
from collections import deque
from ai.src.command_queue import CommandQueue
from ai.src.inventory_parser import WorldState
from ai.src.vision_parser import Vision

class FakeConnection:
    def __init__(self):
        self.commands_sent = []
        self.lines_to_read = deque(["ok"])

    def send_command(self, command: str):
        self.commands_sent.append(command)

    def read_line(self):
        if self.lines_to_read:
            return self.lines_to_read.popleft()
        return None


class TestCommandQueue(unittest.TestCase):

    def test_push_and_flush(self):
        conn = FakeConnection()
        queue = CommandQueue(conn)
        queue.push("Forward")
        queue.push("Look")
        queue.flush()
        self.assertEqual(conn.commands_sent, ["Forward", "Look"])
        self.assertEqual(queue.pending, 2)

    def test_handle_response(self):
        conn = FakeConnection()
        queue = CommandQueue(conn)
        world = WorldState()
        queue.pending = 2
        queue.handle_response(world, "ok")
        self.assertEqual(queue.pending, 1)
        queue.handle_response(world, "message 1,1,hello")
        self.assertEqual(queue.pending, 0)

    def test_reset(self):
        conn = FakeConnection()
        queue = CommandQueue(conn)
        queue.push("Test")
        queue.pending = 5
        queue.reset()
        self.assertEqual(len(queue.queue), 0)
        self.assertEqual(queue.pending, 0)

    def test_push_valid_command(self):
        conn = FakeConnection()
        queue = CommandQueue(conn)
        queue.push("Inventory")
        self.assertEqual(len(queue.queue), 1)
        self.assertEqual(queue.queue[0], "Inventory")
        self.assertEqual(queue.expected_responses[0], "Inventory")

    def test_push_invalid_command_type(self):
        conn = FakeConnection()
        queue = CommandQueue(conn)
        with self.assertRaises(TypeError) as context:
            queue.push(123)
        self.assertIn("Command must be a string", str(context.exception))

    def test_handle_response_level_update(self):
        conn = FakeConnection()
        queue = CommandQueue(conn)
        world = WorldState()
        world.level = 1
        
        queue.pending = 1
        queue.handle_response(world, "Current level: 2")
        
        self.assertEqual(world.level, 2)
        self.assertEqual(queue.pending, 0)
        # Should add broadcast command
        self.assertEqual(len(queue.queue), 1)
        self.assertEqual(queue.queue[0], "Broadcast notleader")

    def test_handle_response_invalid_level_format(self):
        conn = FakeConnection()
        queue = CommandQueue(conn)
        world = WorldState()
        world.level = 1
        
        queue.pending = 1
        with patch('builtins.print') as mock_print:
            queue.handle_response(world, "Current level: invalid")
            # Level should remain unchanged
            self.assertEqual(world.level, 1)
            self.assertEqual(queue.pending, 1)
            # Should print error message
            mock_print.assert_called()

    def test_handle_response_standard_responses(self):
        conn = FakeConnection()
        queue = CommandQueue(conn)
        world = WorldState()
        
        # Test different standard responses
        for response in ["ok", "ko", "dead"]:
            queue.pending = 1
            queue.handle_response(world, response)
            self.assertEqual(queue.pending, 0)

    def test_handle_response_message_format(self):
        conn = FakeConnection()
        queue = CommandQueue(conn)
        world = WorldState()
        
        queue.pending = 1
        queue.handle_response(world, "message 1,2,hello")
        self.assertEqual(queue.pending, 0)

    def test_handle_response_inventory_format(self):
        conn = FakeConnection()
        queue = CommandQueue(conn)
        world = WorldState()
        
        queue.pending = 1
        queue.handle_response(world, "[ food 5, linemate 2 ]")
        self.assertEqual(queue.pending, 0)

    def test_handle_response_numeric_format(self):
        conn = FakeConnection()
        queue = CommandQueue(conn)
        world = WorldState()
        
        queue.pending = 1
        queue.handle_response(world, "5")
        self.assertEqual(queue.pending, 0)


class TestWorldState(unittest.TestCase):

    def test_parse_inventory_valid(self):
        world = WorldState()
        world.parse_inventory("[ food 5, linemate 2 ]")
        self.assertEqual(world.inventory["food"], 5)
        self.assertEqual(world.inventory["linemate"], 2)

    def test_parse_inventory_invalid_format(self):
        world = WorldState()
        world.parse_inventory("bad input")
        self.assertEqual(world.inventory, {})

    def test_get_food_count(self):
        world = WorldState()
        world.parse_inventory("[ food 3 ]")
        self.assertEqual(world.get_food_count(), 3)


class TestVision(unittest.TestCase):

    def test_parse_look_valid(self):
        vision = Vision()
        vision.parse_look("[ player, food, linemate deraumere ]")
        self.assertEqual(vision.tiles[0], ["player"])
        self.assertEqual(vision.tiles[1], ["food"])
        self.assertEqual(vision.tiles[2], ["linemate", "deraumere"])

    def test_get_tile(self):
        vision = Vision()
        vision.parse_look("[ food, linemate ]")
        self.assertEqual(vision.get_tile(1), ["linemate"])
        self.assertEqual(vision.get_tile(99), [])

    def test_find_nearest(self):
        vision = Vision()
        vision.parse_look("[ food, linemate, deraumere ]")
        self.assertEqual(vision.find_nearest("deraumere"), 2)
        self.assertEqual(vision.find_nearest("thystame"), -1)


if __name__ == "__main__":
    unittest.main()
