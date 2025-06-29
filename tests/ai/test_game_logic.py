import unittest
from unittest.mock import Mock, patch
from ai.src.roles.game_logic import (
    move_to_target, parse_broadcast, security_move, select_role
)
from ai.src.inventory_parser import WorldState


class TestGameLogic(unittest.TestCase):

    def setUp(self):
        self.queue = Mock()
        self.world = Mock()
        self.vision = Mock()
        self.ai = Mock()

    def test_move_to_target_with_path(self):
        """Test move_to_target with valid target path"""
        target_path = ["Forward", "Left", "Forward"]
        
        result = move_to_target(self.queue, target_path)
        
        self.assertTrue(result)
        # Should push each command in the path
        expected_calls = [unittest.mock.call("Forward"), unittest.mock.call("Left"), unittest.mock.call("Forward")]
        self.queue.push.assert_has_calls(expected_calls)

    def test_move_to_target_empty_path(self):
        """Test move_to_target with empty path"""
        result = move_to_target(self.queue, [])
        
        self.assertFalse(result)
        self.queue.push.assert_not_called()

    def test_move_to_target_none_path(self):
        """Test move_to_target with None path"""
        result = move_to_target(self.queue, None)
        
        self.assertFalse(result)
        self.queue.push.assert_not_called()

    def test_security_move(self):
        """Test security_move function"""
        security_move(self.queue)
        
        # Should push Right then Forward
        expected_calls = [unittest.mock.call("Right"), unittest.mock.call("Forward")]
        self.queue.push.assert_has_calls(expected_calls)

    def test_parse_broadcast_leader_message(self):
        """Test parse_broadcast with leader message"""
        self.world.level = 2
        message = "message 1, leader_2"
        
        parse_broadcast(self.queue, message, self.world)
        
        self.assertTrue(self.world.leader)

    def test_parse_broadcast_notleader_message(self):
        """Test parse_broadcast with notleader message"""
        self.world.leader = True
        message = "message 1, notleader"
        
        parse_broadcast(self.queue, message, self.world)
        
        self.assertFalse(self.world.leader)
        self.assertEqual(self.world.leader_direction, -69)

    def test_parse_broadcast_incantation_message(self):
        """Test parse_broadcast with incantation message"""
        world = WorldState()
        world.team_name = "test_team"
        world.level = 3
        message = "message 2, incantation_test_team_3"
        
        with patch('ai.src.roles.game_logic.goal_to') as mock_goal_to:
            mock_goal_to.return_value = ["Forward", "Left"]
            
            parse_broadcast(self.queue, message, world)
            
            self.assertEqual(world.leader_direction, 2)
            self.assertTrue(world.following_leader)
            mock_goal_to.assert_called_once_with(2)

    def test_parse_broadcast_wrong_team(self):
        """Test parse_broadcast with wrong team name"""
        self.world.team_name = "my_team"
        self.world.level = 3
        message = "message 2, incantation_other_team_3"
        
        parse_broadcast(self.queue, message, self.world)
        
        # Should not modify world state
        self.assertFalse(hasattr(self.world, 'leader_direction') and self.world.leader_direction == 2)

    def test_parse_broadcast_wrong_level(self):
        """Test parse_broadcast with wrong level"""
        self.world.team_name = "test_team"
        self.world.level = 2
        message = "message 2, incantation_test_team_3"
        
        parse_broadcast(self.queue, message, self.world)
        
        # Should not modify world state
        self.assertFalse(hasattr(self.world, 'leader_direction') and self.world.leader_direction == 2)

    def test_parse_broadcast_malformed_message(self):
        """Test parse_broadcast with malformed message"""
        message = "malformed message without comma"
        
        # Should not raise exception
        parse_broadcast(self.queue, message, self.world)
        
        # Queue should not be called
        self.queue.push.assert_not_called()

    @patch('ai.src.roles.game_logic.survivor')
    def test_select_role_survivor_low_food(self, mock_survivor):
        """Test select_role chooses survivor when food is very low"""
        self.world.get_food_count.return_value = 10
        self.world.level = 1
        mock_survivor.return_value = 0
        
        select_role(self.ai, self.queue, self.world, self.vision)
        
        mock_survivor.assert_called_once_with(self.queue, self.vision)
        self.assertEqual(self.ai.last_role, "Survivor")

    @patch('ai.src.roles.game_logic.survivor')
    def test_select_role_survivor_with_security_move(self, mock_survivor):
        """Test select_role calls security_move when survivor returns 1"""
        self.world.get_food_count.return_value = 10
        self.world.level = 1
        mock_survivor.return_value = 1
        
        select_role(self.ai, self.queue, self.world, self.vision)
        
        # Should call security_move (Right, Forward)
        expected_calls = [unittest.mock.call("Right"), unittest.mock.call("Forward")]
        self.queue.push.assert_has_calls(expected_calls, any_order=False)

    def test_select_role_following_leader_same_position(self):
        """Test select_role when following leader at same position"""
        self.world.get_food_count.return_value = 50
        self.world.following_leader = True
        self.world.leader_direction = 0
        
        with patch('builtins.print') as mock_print:
            select_role(self.ai, self.queue, self.world, self.vision)
            mock_print.assert_called_with("[INFO] Leader is at the same POSITION, not moving.")

    def test_select_role_following_leader_stop_following(self):
        """Test select_role stops following leader when direction is -69"""
        self.world.get_food_count.return_value = 50
        self.world.following_leader = True
        self.world.leader_direction = -69
        
        select_role(self.ai, self.queue, self.world, self.vision)
        
        self.assertFalse(self.world.following_leader)
        self.assertEqual(self.world.leader_direction, -1)

    @patch('ai.src.roles.game_logic.breeder')
    def test_select_role_breeder(self, mock_breeder):
        """Test select_role chooses breeder when food is sufficient"""
        self.world.get_food_count.return_value = 60
        self.world.level = 1
        self.world.following_leader = False
        
        with patch('ai.src.roles.game_logic.MINIMUM_FOOD_REQUIREMENTS', {1: 40}):
            select_role(self.ai, self.queue, self.world, self.vision)
            
            mock_breeder.assert_called_once_with(self.queue)
            self.assertEqual(self.ai.last_role, "Breeder")

    @patch('ai.src.roles.game_logic.eject')
    def test_select_role_attacker(self, mock_eject):
        """Test select_role chooses attacker when player detected in front"""
        self.world.get_food_count.return_value = 30
        self.world.level = 1
        self.world.following_leader = False
        self.vision.get_tile.return_value = ["player", "food"]  # Player in front tile
        
        select_role(self.ai, self.queue, self.world, self.vision)
        
        mock_eject.assert_called_once_with(self.queue)
        self.assertEqual(self.ai.last_role, "Attacker")

    @patch('ai.src.roles.game_logic.leader')
    def test_select_role_leader_with_resources(self, mock_leader):
        """Test select_role chooses leader when having enough resources"""
        self.world.get_food_count.return_value = 30
        self.world.level = 1
        self.world.following_leader = False
        self.world.leader = False
        self.world.inventory = {"linemate": 2}
        self.vision.get_tile.return_value = []
        mock_leader.return_value = 0
        
        with patch('ai.src.roles.game_logic.INCANTATION_REQUIREMENTS', {1: {"players": 1, "linemate": 1}}):
            select_role(self.ai, self.queue, self.world, self.vision)
            
            mock_leader.assert_called_once_with(self.ai, self.queue, self.world, self.vision)
            self.assertEqual(self.ai.last_role, "Leader")

    @patch('ai.src.roles.game_logic.miner')
    def test_select_role_miner_default(self, mock_miner):
        """Test select_role defaults to miner"""
        self.world.get_food_count.return_value = 30
        self.world.level = 1
        self.world.following_leader = False
        self.world.leader = False
        self.world.inventory = {}  # No resources
        self.vision.get_tile.return_value = []
        mock_miner.return_value = 0
        
        select_role(self.ai, self.queue, self.world, self.vision)
        
        mock_miner.assert_called_once_with(self.queue, self.world, self.vision)
        self.assertEqual(self.ai.last_role, "Miner")


if __name__ == "__main__":
    unittest.main()
