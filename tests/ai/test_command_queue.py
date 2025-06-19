import unittest
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
        queue.pending = 2
        queue.handle_response("ok")
        self.assertEqual(queue.pending, 1)
        queue.handle_response("message 1,1,hello")
        self.assertEqual(queue.pending, 0)

    def test_reset(self):
        conn = FakeConnection()
        queue = CommandQueue(conn)
        queue.push("Test")
        queue.pending = 5
        queue.reset()
        self.assertEqual(len(queue.queue), 0)
        self.assertEqual(queue.pending, 0)

    def test_send_and_wait(self):
        conn = FakeConnection()
        queue = CommandQueue(conn)
        response = queue.send_and_wait("Inventory")
        self.assertEqual(response, "ok")
        self.assertIn("Inventory", conn.commands_sent)


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
