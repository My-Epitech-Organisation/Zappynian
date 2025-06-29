import unittest
from ai.src.utils.route_factory import route_to, goal_to, DIRECTION_MAP, GOAL_MAP


class TestRouteFactory(unittest.TestCase):

    def test_direction_map_structure(self):
        """Test DIRECTION_MAP has correct structure"""
        # Should have mappings for indices 0-8
        expected_indices = list(range(9))
        self.assertEqual(sorted(DIRECTION_MAP.keys()), expected_indices)
        
        # Each value should be a list of strings
        for index, directions in DIRECTION_MAP.items():
            self.assertIsInstance(directions, list)
            for direction in directions:
                self.assertIsInstance(direction, str)
                self.assertIn(direction, ["Forward", "Left", "Right"])

    def test_goal_map_structure(self):
        """Test GOAL_MAP has correct structure"""
        # Should have mappings for indices 0-8
        expected_indices = list(range(9))
        self.assertEqual(sorted(GOAL_MAP.keys()), expected_indices)
        
        # Each value should be a list of strings
        for index, directions in GOAL_MAP.items():
            self.assertIsInstance(directions, list)
            for direction in directions:
                self.assertIsInstance(direction, str)
                self.assertIn(direction, ["Forward", "Left", "Right"])

    def test_route_to_valid_indices(self):
        """Test route_to function with valid indices"""
        # Test index 0 (current position)
        self.assertEqual(route_to(0), [])
        
        # Test index 1 (front-left)
        self.assertEqual(route_to(1), ["Forward", "Left", "Forward"])
        
        # Test index 2 (front)
        self.assertEqual(route_to(2), ["Forward"])
        
        # Test index 3 (front-right)
        self.assertEqual(route_to(3), ["Forward", "Right", "Forward"])

    def test_route_to_invalid_index(self):
        """Test route_to function with invalid index"""
        self.assertEqual(route_to(999), [])
        self.assertEqual(route_to(-1), [])

    def test_goal_to_valid_indices(self):
        """Test goal_to function with valid indices"""
        # Test index 0 (current position)
        self.assertEqual(goal_to(0), [])
        
        # Test index 1 (front)
        self.assertEqual(goal_to(1), ["Forward"])
        
        # Test index 2 (front-left)
        self.assertEqual(goal_to(2), ["Forward", "Left", "Forward"])
        
        # Test index 3 (left)
        self.assertEqual(goal_to(3), ["Left", "Forward"])

    def test_goal_to_invalid_index(self):
        """Test goal_to function with invalid index"""
        self.assertEqual(goal_to(999), [])
        self.assertEqual(goal_to(-1), [])

    def test_route_to_all_valid_indices(self):
        """Test route_to for all valid indices"""
        for i in range(9):
            result = route_to(i)
            self.assertIsInstance(result, list)
            # Check that all commands are valid
            for command in result:
                self.assertIn(command, ["Forward", "Left", "Right"])

    def test_goal_to_all_valid_indices(self):
        """Test goal_to for all valid indices"""
        for i in range(9):
            result = goal_to(i)
            self.assertIsInstance(result, list)
            # Check that all commands are valid
            for command in result:
                self.assertIn(command, ["Forward", "Left", "Right"])

    def test_specific_route_patterns(self):
        """Test specific route patterns make sense"""
        # Index 4 should be a longer path (back-left)
        route_4 = route_to(4)
        self.assertTrue(len(route_4) > 2)
        
        # Index 8 should be a longer path (back-right)
        route_8 = route_to(8)
        self.assertTrue(len(route_8) > 2)

    def test_specific_goal_patterns(self):
        """Test specific goal patterns make sense"""
        # Index 4 should involve turning around (back-left)
        goal_4 = goal_to(4)
        self.assertIn("Left", goal_4)
        
        # Index 6 should involve turning around (back-right)
        goal_6 = goal_to(6)
        self.assertIn("Right", goal_6)

    def test_maps_consistency(self):
        """Test that both maps have the same indices"""
        self.assertEqual(set(DIRECTION_MAP.keys()), set(GOAL_MAP.keys()))

    def test_center_position(self):
        """Test that center position (0) returns empty list for both functions"""
        self.assertEqual(route_to(0), [])
        self.assertEqual(goal_to(0), [])

    def test_adjacent_positions(self):
        """Test adjacent positions have reasonable paths"""
        # Adjacent positions should not require too many moves
        for i in [1, 2, 3]:
            route = route_to(i)
            goal = goal_to(i)
            # Adjacent positions should not require more than 3 moves
            self.assertLessEqual(len(route), 3)
            self.assertLessEqual(len(goal), 3)


if __name__ == "__main__":
    unittest.main()
