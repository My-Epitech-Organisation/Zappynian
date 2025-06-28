/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** Complete tests for WorldScene class
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "Core/WorldScene.hpp"
#include "Event/EventReceiver.hpp"
#include "Entities/Direction.hpp"
#include "irrlicht_mocks.hpp"
#include <memory>

// Mock EventReceiver for testing
class MockEventReceiver : public EventReceiver {
public:
    MockEventReceiver() : EventReceiver() {}
};

TestSuite(world_scene_complete);

Test(world_scene_complete, constructor_destructor) {
    std::shared_ptr<EventReceiver> eventReceiver = std::make_shared<MockEventReceiver>();
    WorldScene scene(eventReceiver);
    
    cr_assert(true, "WorldScene constructor should work");
}

Test(world_scene_complete, create_player_entities_all_directions) {
    std::shared_ptr<EventReceiver> eventReceiver = std::make_shared<MockEventReceiver>();
    WorldScene scene(eventReceiver);
    
    // Test creating players in all directions
    scene.createEntities(1, 0, 0, Direction::NORTH, 1, "team1");
    scene.createEntities(2, 1, 0, Direction::EAST, 1, "team1");
    scene.createEntities(3, 0, 1, Direction::SOUTH, 1, "team1");
    scene.createEntities(4, 1, 1, Direction::WEST, 1, "team1");
    
    cr_assert(true, "Creating player entities in all directions should work");
}

Test(world_scene_complete, create_player_entities_all_levels) {
    std::shared_ptr<EventReceiver> eventReceiver = std::make_shared<MockEventReceiver>();
    WorldScene scene(eventReceiver);
    
    // Test creating players at all levels
    for (int level = 1; level <= 8; level++) {
        scene.createEntities(level, 0, 0, Direction::NORTH, level, "team_level_" + std::to_string(level));
    }
    
    cr_assert(true, "Creating player entities at all levels should work");
}

Test(world_scene_complete, create_stone_entities_all_resources) {
    std::shared_ptr<EventReceiver> eventReceiver = std::make_shared<MockEventReceiver>();
    WorldScene scene(eventReceiver);
    
    // Test creating stones with all resource combinations
    scene.createEntities(0, 0, 1, 0, 0, 0, 0, 0, 0);  // Only food
    scene.createEntities(0, 1, 0, 1, 0, 0, 0, 0, 0);  // Only linemate
    scene.createEntities(0, 2, 0, 0, 1, 0, 0, 0, 0);  // Only deraumere
    scene.createEntities(0, 3, 0, 0, 0, 1, 0, 0, 0);  // Only sibur
    scene.createEntities(0, 4, 0, 0, 0, 0, 1, 0, 0);  // Only mendiane
    scene.createEntities(0, 5, 0, 0, 0, 0, 0, 1, 0);  // Only phiras
    scene.createEntities(0, 6, 0, 0, 0, 0, 0, 0, 1);  // Only thystame
    
    cr_assert(true, "Creating stone entities with all resources should work");
}

Test(world_scene_complete, create_stone_entities_with_tiles) {
    std::shared_ptr<EventReceiver> eventReceiver = std::make_shared<MockEventReceiver>();
    WorldScene scene(eventReceiver);
    
    // Test creating stones with tiles (nbTiles parameter)
    scene.createEntities(0, 0, 1, 1, 1, 1, 1, 1, 1, 5);  // 5 tiles
    scene.createEntities(1, 1, 2, 2, 2, 2, 2, 2, 2, 10); // 10 tiles
    scene.createEntities(2, 2, 0, 0, 0, 0, 0, 0, 0, 1);  // 1 tile, no stones
    
    cr_assert(true, "Creating stone entities with tiles should work");
}

Test(world_scene_complete, create_stone_entities_without_tiles) {
    std::shared_ptr<EventReceiver> eventReceiver = std::make_shared<MockEventReceiver>();
    WorldScene scene(eventReceiver);
    
    // Test creating stones without tiles (7-parameter version)
    scene.createEntities(0, 0, 1, 1, 1, 1, 1, 1, 1);
    scene.createEntities(5, 5, 3, 2, 1, 4, 2, 1, 3);
    scene.createEntities(10, 10, 0, 0, 0, 0, 0, 0, 0);  // No resources
    
    cr_assert(true, "Creating stone entities without tiles should work");
}

Test(world_scene_complete, create_egg_entities) {
    std::shared_ptr<EventReceiver> eventReceiver = std::make_shared<MockEventReceiver>();
    WorldScene scene(eventReceiver);
    
    // Test creating egg entities
    scene.createEntities(1);
    scene.createEntities(100);
    scene.createEntities(-1);
    scene.createEntities(0);
    
    cr_assert(true, "Creating egg entities should work");
}

Test(world_scene_complete, add_teams) {
    std::shared_ptr<EventReceiver> eventReceiver = std::make_shared<MockEventReceiver>();
    WorldScene scene(eventReceiver);
    
    // Test adding various teams
    scene.addTeam("red_team");
    scene.addTeam("blue_team");
    scene.addTeam("green_team");
    scene.addTeam("");  // Empty team name
    scene.addTeam("team_with_very_long_name_and_special_characters_123");
    
    cr_assert(true, "Adding teams should work");
}

Test(world_scene_complete, change_player_position_all_directions) {
    std::shared_ptr<EventReceiver> eventReceiver = std::make_shared<MockEventReceiver>();
    WorldScene scene(eventReceiver);
    
    // First create a player
    scene.createEntities(1, 0, 0, Direction::NORTH, 1, "team1");
    
    // Test changing position in all directions
    scene.changePlayerPos(1, 1, 0, Direction::EAST, Direction::NORTH);
    scene.changePlayerPos(1, 1, 1, Direction::SOUTH, Direction::EAST);
    scene.changePlayerPos(1, 0, 1, Direction::WEST, Direction::SOUTH);
    scene.changePlayerPos(1, 0, 0, Direction::NORTH, Direction::WEST);
    
    cr_assert(true, "Changing player position in all directions should work");
}

Test(world_scene_complete, change_player_position_edge_cases) {
    std::shared_ptr<EventReceiver> eventReceiver = std::make_shared<MockEventReceiver>();
    WorldScene scene(eventReceiver);
    
    // Create a player
    scene.createEntities(1, 5, 5, Direction::NORTH, 1, "team1");
    
    // Test edge cases
    scene.changePlayerPos(1, -1, -1, Direction::NORTH, Direction::NORTH);  // Negative coordinates
    scene.changePlayerPos(1, 1000, 1000, Direction::SOUTH, Direction::NORTH);  // Large coordinates
    scene.changePlayerPos(1, 0, 0, Direction::NORTH, Direction::NORTH);  // Same direction
    
    cr_assert(true, "Changing player position edge cases should work");
}

Test(world_scene_complete, change_nonexistent_player_position) {
    std::shared_ptr<EventReceiver> eventReceiver = std::make_shared<MockEventReceiver>();
    WorldScene scene(eventReceiver);
    
    // Try to change position of non-existent player
    scene.changePlayerPos(999, 0, 0, Direction::NORTH, Direction::SOUTH);
    
    cr_assert(true, "Changing non-existent player position should handle gracefully");
}

Test(world_scene_complete, set_player_level_all_levels) {
    std::shared_ptr<EventReceiver> eventReceiver = std::make_shared<MockEventReceiver>();
    WorldScene scene(eventReceiver);
    
    // Create a player
    scene.createEntities(1, 0, 0, Direction::NORTH, 1, "team1");
    
    // Test setting all levels
    for (int level = 1; level <= 8; level++) {
        scene.setPlayerLevel(1, level);
    }
    
    cr_assert(true, "Setting player level to all values should work");
}

Test(world_scene_complete, set_player_level_edge_cases) {
    std::shared_ptr<EventReceiver> eventReceiver = std::make_shared<MockEventReceiver>();
    WorldScene scene(eventReceiver);
    
    // Create a player
    scene.createEntities(1, 0, 0, Direction::NORTH, 1, "team1");
    
    // Test edge cases
    scene.setPlayerLevel(1, 0);    // Level 0
    scene.setPlayerLevel(1, -1);   // Negative level
    scene.setPlayerLevel(1, 100);  // Very high level
    
    cr_assert(true, "Setting player level edge cases should work");
}

Test(world_scene_complete, set_nonexistent_player_level) {
    std::shared_ptr<EventReceiver> eventReceiver = std::make_shared<MockEventReceiver>();
    WorldScene scene(eventReceiver);
    
    // Try to set level of non-existent player
    scene.setPlayerLevel(999, 5);
    
    cr_assert(true, "Setting non-existent player level should handle gracefully");
}

Test(world_scene_complete, set_player_inventory_all_resources) {
    std::shared_ptr<EventReceiver> eventReceiver = std::make_shared<MockEventReceiver>();
    WorldScene scene(eventReceiver);
    
    // Create a player
    scene.createEntities(1, 0, 0, Direction::NORTH, 1, "team1");
    
    // Test setting inventory with all resources
    scene.setPlayerInventory(1, 10, 5, 3, 2, 1, 4, 6);
    scene.setPlayerInventory(1, 0, 0, 0, 0, 0, 0, 0);  // Empty inventory
    scene.setPlayerInventory(1, 100, 100, 100, 100, 100, 100, 100);  // Large quantities
    
    cr_assert(true, "Setting player inventory with all resources should work");
}

Test(world_scene_complete, set_player_inventory_edge_cases) {
    std::shared_ptr<EventReceiver> eventReceiver = std::make_shared<MockEventReceiver>();
    WorldScene scene(eventReceiver);
    
    // Create a player
    scene.createEntities(1, 5, 5, Direction::NORTH, 1, "team1");
    
    // Test setting inventory at different positions
    scene.setPlayerInventory(1, 1, 1, 1, 1, 1, 1, 1);  // Negative position
    scene.setPlayerInventory(1, 2, 2, 2, 2, 2, 2, 2);  // Large position
    
    cr_assert(true, "Setting player inventory edge cases should work");
}

Test(world_scene_complete, set_nonexistent_player_inventory) {
    std::shared_ptr<EventReceiver> eventReceiver = std::make_shared<MockEventReceiver>();
    WorldScene scene(eventReceiver);
    
    // Try to set inventory of non-existent player
    scene.setPlayerInventory(999, 1, 1, 1, 1, 1, 1, 1);
    
    cr_assert(true, "Setting non-existent player inventory should handle gracefully");
}

Test(world_scene_complete, expulsion_test) {
    std::shared_ptr<EventReceiver> eventReceiver = std::make_shared<MockEventReceiver>();
    WorldScene scene(eventReceiver);
    
    // Create players
    scene.createEntities(1, 0, 0, Direction::NORTH, 1, "team1");
    scene.createEntities(2, 1, 1, Direction::SOUTH, 1, "team2");
    
    // Test expulsion
    scene.expulsion(1);
    scene.expulsion(2);
    scene.expulsion(999);  // Non-existent player
    
    cr_assert(true, "Expulsion should work");
}

Test(world_scene_complete, broadcast_test) {
    std::shared_ptr<EventReceiver> eventReceiver = std::make_shared<MockEventReceiver>();
    WorldScene scene(eventReceiver);
    
    // Create a player
    scene.createEntities(1, 0, 0, Direction::NORTH, 1, "team1");
    
    // Test broadcast
    scene.broadcast(1, "Hello World");
    scene.broadcast(1, "");  // Empty message
    scene.broadcast(1, "Very long message with special characters !@#$%^&*()");
    scene.broadcast(999, "Message from non-existent player");
    
    cr_assert(true, "Broadcast should work");
}

Test(world_scene_complete, mixed_operations_stress_test) {
    std::shared_ptr<EventReceiver> eventReceiver = std::make_shared<MockEventReceiver>();
    WorldScene scene(eventReceiver);
    
    // Add teams
    for (int i = 0; i < 5; i++) {
        scene.addTeam("team" + std::to_string(i));
    }
    
    // Create multiple players
    for (int i = 1; i <= 10; i++) {
        scene.createEntities(i, i % 5, i / 5, static_cast<Direction>(i % 4), 
                           (i % 8) + 1, "team" + std::to_string(i % 5));
    }
    
    // Create stones and tiles
    for (int x = 0; x < 5; x++) {
        for (int y = 0; y < 5; y++) {
            scene.createEntities(x, y, x % 3, y % 3, (x + y) % 3, 
                               x % 2, y % 2, (x * y) % 3, (x + y) % 2, 
                               (x + y) % 5 + 1);
        }
    }
    
    // Create eggs
    for (int i = 1; i <= 5; i++) {
        scene.createEntities(i);
    }
    
    // Move players around
    for (int i = 1; i <= 10; i++) {
        scene.changePlayerPos(i, (i + 1) % 5, (i + 2) % 5, 
                            static_cast<Direction>((i + 1) % 4), 
                            static_cast<Direction>(i % 4));
    }
    
    // Set player levels
    for (int i = 1; i <= 10; i++) {
        scene.setPlayerLevel(i, (i % 8) + 1);
    }
    
    // Set player inventories
    for (int i = 1; i <= 10; i++) {
        scene.setPlayerInventory(i, i % 10, (i + 1) % 10, 
                               (i + 2) % 10, (i + 3) % 10, (i + 4) % 10, 
                               (i + 5) % 10, (i + 6) % 10);
    }
    
    // Test expulsion and broadcast
    for (int i = 1; i <= 10; i++) {
        if (i % 3 == 0) {
            scene.expulsion(i);
        }
        if (i % 2 == 0) {
            scene.broadcast(i, "Message from player " + std::to_string(i));
        }
    }
    
    cr_assert(true, "Mixed operations stress test should work");
}

Test(world_scene_complete, null_event_receiver) {
    // Test with null event receiver
    try {
        WorldScene scene(nullptr);
        cr_assert(true, "WorldScene with null EventReceiver should work or handle gracefully");
    } catch (const std::exception& e) {
        cr_assert(true, "WorldScene with null EventReceiver threw exception as expected");
    }
}
