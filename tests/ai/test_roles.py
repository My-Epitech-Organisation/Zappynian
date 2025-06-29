import unittest
from unittest.mock import Mock, patch
from ai.src.roles.survivor import survivor
from ai.src.roles.miner import miner
from ai.src.roles.breeder import breeder
from ai.src.roles.attack import eject


class TestRoles(unittest.TestCase):

    def setUp(self):
        self.queue = Mock()
        self.world = Mock()
        self.vision = Mock()

    def test_survivor_food_found(self):
        """Test survivor role when food is found"""
        self.vision.find_nearest.return_value = 2  # Food at tile 2
        
        with patch('ai.src.roles.survivor.route_to') as mock_route_to:
            mock_route_to.return_value = ["Forward"]
            
            result = survivor(self.queue, self.vision)
            
            self.assertEqual(result, 0)
            self.vision.find_nearest.assert_called_once_with("food")
            mock_route_to.assert_called_once_with(2)
            
            # Should push movement commands and take food
            expected_calls = [
                unittest.mock.call("Forward"),
                unittest.mock.call("Take food")
            ]
            self.queue.push.assert_has_calls(expected_calls)

    def test_survivor_no_food_found(self):
        """Test survivor role when no food is found"""
        self.vision.find_nearest.return_value = -1  # No food found
        
        result = survivor(self.queue, self.vision)
        
        self.assertEqual(result, 1)
        self.vision.find_nearest.assert_called_once_with("food")
        # Should not push any commands
        self.queue.push.assert_not_called()

    def test_miner_needs_resource(self):
        """Test miner role when needing a specific resource"""
        self.world.level = 1
        self.world.inventory = {"linemate": 0}  # Needs linemate
        self.vision.find_nearest.return_value = 3  # Linemate at tile 3
        
        with patch('ai.src.roles.miner.INCANTATION_REQUIREMENTS', {1: {"players": 1, "linemate": 1}}):
            with patch('ai.src.roles.miner.route_to') as mock_route_to:
                mock_route_to.return_value = ["Forward", "Right", "Forward"]
                
                result = miner(self.queue, self.world, self.vision)
                
                self.assertEqual(result, 0)
                self.vision.find_nearest.assert_called_once_with("linemate")
                mock_route_to.assert_called_once_with(3)
                
                # Should push movement commands and take resource
                expected_calls = [
                    unittest.mock.call("Forward"),
                    unittest.mock.call("Right"),
                    unittest.mock.call("Forward"),
                    unittest.mock.call("Take linemate")
                ]
                self.queue.push.assert_has_calls(expected_calls)

    def test_miner_resource_not_found(self):
        """Test miner role when needed resource is not found"""
        self.world.level = 1
        self.world.inventory = {"linemate": 0}
        self.vision.find_nearest.return_value = -1  # Resource not found
        
        with patch('ai.src.roles.miner.INCANTATION_REQUIREMENTS', {1: {"players": 1, "linemate": 1}}):
            result = miner(self.queue, self.world, self.vision)
            
            self.assertEqual(result, 1)
            self.vision.find_nearest.assert_called_once_with("linemate")
            # Should not push any commands
            self.queue.push.assert_not_called()

    def test_miner_has_all_resources(self):
        """Test miner role when already has all needed resources"""
        self.world.level = 1
        self.world.inventory = {"linemate": 1}  # Has enough linemate
        
        with patch('ai.src.roles.miner.INCANTATION_REQUIREMENTS', {1: {"players": 1, "linemate": 1}}):
            result = miner(self.queue, self.world, self.vision)
            
            self.assertEqual(result, 1)
            # Should not look for resources or push commands
            self.vision.find_nearest.assert_not_called()
            self.queue.push.assert_not_called()

    def test_miner_multiple_resources(self):
        """Test miner role with multiple resource requirements"""
        self.world.level = 2
        self.world.inventory = {
            "linemate": 1,  # Has enough
            "deraumere": 0,  # Needs this
            "sibur": 1       # Has enough
        }
        self.vision.find_nearest.return_value = 4  # Deraumere at tile 4
        
        requirements = {
            2: {
                "players": 2,
                "linemate": 1,
                "deraumere": 1,
                "sibur": 1
            }
        }
        
        with patch('ai.src.roles.miner.INCANTATION_REQUIREMENTS', requirements):
            with patch('ai.src.roles.miner.route_to') as mock_route_to:
                mock_route_to.return_value = ["Forward", "Forward"]
                
                result = miner(self.queue, self.world, self.vision)
                
                self.assertEqual(result, 0)
                # Should look for the missing resource (deraumere)
                self.vision.find_nearest.assert_called_once_with("deraumere")
                
                expected_calls = [
                    unittest.mock.call("Forward"),
                    unittest.mock.call("Forward"),
                    unittest.mock.call("Take deraumere")
                ]
                self.queue.push.assert_has_calls(expected_calls)

    def test_miner_unknown_level(self):
        """Test miner role with unknown level"""
        self.world.level = 999  # Unknown level
        
        result = miner(self.queue, self.world, self.vision)
        
        self.assertEqual(result, 1)
        # Should not look for resources or push commands
        self.vision.find_nearest.assert_not_called()
        self.queue.push.assert_not_called()

    def test_breeder(self):
        """Test breeder role"""
        result = breeder(self.queue)
        
        self.assertEqual(result, 0)
        self.queue.push.assert_called_once_with("Fork")

    def test_eject(self):
        """Test eject (attack) role"""
        result = eject(self.queue)
        
        self.assertEqual(result, 0)
        self.queue.push.assert_called_once_with("Eject")

    def test_miner_get_method_safety(self):
        """Test miner role handles missing inventory keys safely"""
        self.world.level = 1
        self.world.inventory = {}  # Empty inventory
        self.vision.find_nearest.return_value = 2
        
        with patch('ai.src.roles.miner.INCANTATION_REQUIREMENTS', {1: {"players": 1, "linemate": 1}}):
            with patch('ai.src.roles.miner.route_to') as mock_route_to:
                mock_route_to.return_value = ["Forward"]
                
                result = miner(self.queue, self.world, self.vision)
                
                self.assertEqual(result, 0)
                # Should handle missing keys with .get() method
                self.vision.find_nearest.assert_called_once_with("linemate")


if __name__ == "__main__":
    unittest.main()
