import unittest
from ai.src.roles.role_selector import select_role
from ai.src.inventory_parser import WorldState
from ai.src.vision_parser import Vision

class DummyVision(Vision):
    def __init__(self):
        super().__init__()

class TestRoleSelector(unittest.TestCase):
    def setUp(self):
        self.vision = DummyVision()
        self.world = WorldState()

    def test_select_survivor_if_low_food(self):
        self.world.inventory = {"food": 2}
        role = select_role(self.world, self.vision)
        self.assertEqual(role.name, "Survivor")

    def test_select_miner_if_missing_resources(self):
        self.world.inventory = {"food": 6, "linemate": 0}
        role = select_role(self.world, self.vision)
        self.assertEqual(role.name, "Miner")

    def test_select_scout_if_all_resources_ok(self):
        self.world.inventory = {
            "food": 6, "linemate": 2, "deraumere": 2,
            "sibur": 2, "mendiane": 1, "phiras": 1, "thystame": 1
        }
        role = select_role(self.world, self.vision)
        self.assertEqual(role.name, "Scout")

if __name__ == '__main__':
    unittest.main()
