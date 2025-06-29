import unittest
from ai.src.utils.incantation_data import INCANTATION_REQUIREMENTS, MINIMUM_FOOD_REQUIREMENTS


class TestIncantationData(unittest.TestCase):

    def test_incantation_requirements_structure(self):
        """Test that INCANTATION_REQUIREMENTS has correct structure"""
        # Should have requirements for levels 1-7
        expected_levels = [1, 2, 3, 4, 5, 6, 7]
        self.assertEqual(sorted(INCANTATION_REQUIREMENTS.keys()), expected_levels)
        
        # Each level should have at least players requirement
        for level in expected_levels:
            self.assertIn("players", INCANTATION_REQUIREMENTS[level])
            self.assertIsInstance(INCANTATION_REQUIREMENTS[level]["players"], int)
            self.assertGreater(INCANTATION_REQUIREMENTS[level]["players"], 0)

    def test_incantation_requirements_level_1(self):
        """Test level 1 requirements"""
        req = INCANTATION_REQUIREMENTS[1]
        self.assertEqual(req["players"], 1)
        self.assertEqual(req["linemate"], 1)
        self.assertEqual(len(req), 2)  # Only players and linemate

    def test_incantation_requirements_level_2(self):
        """Test level 2 requirements"""
        req = INCANTATION_REQUIREMENTS[2]
        self.assertEqual(req["players"], 2)
        self.assertEqual(req["linemate"], 1)
        self.assertEqual(req["deraumere"], 1)
        self.assertEqual(req["sibur"], 1)

    def test_incantation_requirements_level_7(self):
        """Test level 7 requirements (most complex)"""
        req = INCANTATION_REQUIREMENTS[7]
        self.assertEqual(req["players"], 6)
        self.assertEqual(req["linemate"], 2)
        self.assertEqual(req["deraumere"], 2)
        self.assertEqual(req["sibur"], 2)
        self.assertEqual(req["mendiane"], 2)
        self.assertEqual(req["phiras"], 2)
        self.assertEqual(req["thystame"], 1)

    def test_minimum_food_requirements_structure(self):
        """Test that MINIMUM_FOOD_REQUIREMENTS has correct structure"""
        # Should have requirements for levels 1-7
        expected_levels = [1, 2, 3, 4, 5, 6, 7]
        self.assertEqual(sorted(MINIMUM_FOOD_REQUIREMENTS.keys()), expected_levels)
        
        # Each level should have a positive integer food requirement
        for level in expected_levels:
            food_req = MINIMUM_FOOD_REQUIREMENTS[level]
            self.assertIsInstance(food_req, int)
            self.assertGreater(food_req, 0)

    def test_minimum_food_requirements_values(self):
        """Test specific minimum food requirement values"""
        self.assertEqual(MINIMUM_FOOD_REQUIREMENTS[1], 40)
        self.assertEqual(MINIMUM_FOOD_REQUIREMENTS[2], 40)
        self.assertEqual(MINIMUM_FOOD_REQUIREMENTS[3], 45)
        self.assertEqual(MINIMUM_FOOD_REQUIREMENTS[4], 55)
        self.assertEqual(MINIMUM_FOOD_REQUIREMENTS[5], 55)
        self.assertEqual(MINIMUM_FOOD_REQUIREMENTS[6], 60)
        self.assertEqual(MINIMUM_FOOD_REQUIREMENTS[7], 60)

    def test_food_requirements_progression(self):
        """Test that food requirements generally increase with level"""
        previous_level_food = 0
        for level in sorted(MINIMUM_FOOD_REQUIREMENTS.keys()):
            current_food = MINIMUM_FOOD_REQUIREMENTS[level]
            # Food requirements should not decrease significantly
            self.assertGreaterEqual(current_food, previous_level_food - 5)
            previous_level_food = current_food

    def test_player_requirements_progression(self):
        """Test that player requirements increase appropriately"""
        player_counts = [INCANTATION_REQUIREMENTS[level]["players"] for level in sorted(INCANTATION_REQUIREMENTS.keys())]
        # Level 1 should require 1 player, higher levels should require more
        self.assertEqual(player_counts[0], 1)  # Level 1
        self.assertTrue(all(count >= 1 for count in player_counts))
        # Maximum should not be unreasonably high
        self.assertTrue(all(count <= 10 for count in player_counts))


if __name__ == "__main__":
    unittest.main()
