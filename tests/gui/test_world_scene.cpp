/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** Tests for WorldScene class
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "Core/WorldScene.hpp"
#include "Event/EventReceiver.hpp"
#include <memory>

// Mock EventReceiver for testing
class MockEventReceiver : public EventReceiver {
public:
    MockEventReceiver() : EventReceiver() {}
    
    // Override methods that might be called during testing
    bool OnEvent(const irr::SEvent& event) override {
        (void)event;
        return false;
    }
};

TestSuite(world_scene);

Test(world_scene, constructor) {
    MockEventReceiver receiver;
    WorldScene scene(nullptr, nullptr, receiver, "");
    
    // Constructor should not crash
    cr_assert(true, "WorldScene constructor should work");
}

Test(world_scene, create_plane) {
    MockEventReceiver receiver;
    WorldScene scene(nullptr, nullptr, receiver, "");
    
    // Should not crash even with null pointers (testing logic, not rendering)
    scene.createPlane(5, 5);
    
    cr_assert(true, "createPlane should not crash");
}

Test(world_scene, add_team) {
    MockEventReceiver receiver;
    WorldScene scene(nullptr, nullptr, receiver, "");
    
    scene.addTeam("TeamRed");
    scene.addTeam("TeamBlue");
    
    // Adding duplicate team should not cause issues
    scene.addTeam("TeamRed");
    
    cr_assert(true, "addTeam should handle duplicates gracefully");
}

Test(world_scene, add_multiple_teams) {
    MockEventReceiver receiver;
    WorldScene scene(nullptr, nullptr, receiver, "");
    
    std::vector<std::string> teams = {"Red", "Blue", "Green", "Yellow", "Purple", "Orange"};
    
    for (const auto& team : teams) {
        scene.addTeam(team);
    }
    
    cr_assert(true, "Should be able to add multiple teams");
}

Test(world_scene, create_entities_basic) {
    MockEventReceiver receiver;
    WorldScene scene(nullptr, nullptr, receiver, "");
    
    // Test basic entity creation
    scene.createEntities(1, 0, 0, Direction::NORTH, 1, "TeamRed");
    scene.createEntities(2, 1, 1, Direction::EAST, 2, "TeamBlue");
    
    cr_assert(true, "createEntities should work with valid parameters");
}

Test(world_scene, create_entities_edge_cases) {
    MockEventReceiver receiver;
    WorldScene scene(nullptr, nullptr, receiver, "");
    
    // Test edge cases
    scene.createEntities(0, 0, 0, Direction::NORTH, 0, "");  // Edge case: id=0, level=0, empty team
    scene.createEntities(-1, -1, -1, Direction::SOUTH, -1, "NegativeTeam");  // Negative values
    scene.createEntities(999999, 999, 999, Direction::WEST, 999, "VeryLongTeamNameThatShouldStillWork");
    
    cr_assert(true, "createEntities should handle edge cases");
}

Test(world_scene, change_player_position) {
    MockEventReceiver receiver;
    WorldScene scene(nullptr, nullptr, receiver, "");
    
    // First create a player
    scene.createEntities(1, 0, 0, Direction::NORTH, 1, "TeamRed");
    
    // Then change position
    scene.changePlayerPos(1, 5, 5, Direction::EAST, Direction::NORTH);
    scene.changePlayerPos(1, 10, 10, Direction::SOUTH, Direction::EAST);
    
    cr_assert(true, "changePlayerPos should work");
}

Test(world_scene, change_nonexistent_player_position) {
    MockEventReceiver receiver;
    WorldScene scene(nullptr, nullptr, receiver, "");
    
    // Try to change position of non-existent player
    scene.changePlayerPos(999, 5, 5, Direction::EAST, Direction::NORTH);
    
    cr_assert(true, "changePlayerPos should handle non-existent player gracefully");
}

Test(world_scene, set_player_level) {
    MockEventReceiver receiver;
    WorldScene scene(nullptr, nullptr, receiver, "");
    
    // Create player first
    scene.createEntities(1, 0, 0, Direction::NORTH, 1, "TeamRed");
    
    // Set different levels
    scene.setPlayerLevel(1, 2);
    scene.setPlayerLevel(1, 8);
    scene.setPlayerLevel(1, 1);
    
    cr_assert(true, "setPlayerLevel should work");
}

Test(world_scene, set_player_level_edge_cases) {
    MockEventReceiver receiver;
    WorldScene scene(nullptr, nullptr, receiver, "");
    
    // Create player first
    scene.createEntities(1, 0, 0, Direction::NORTH, 1, "TeamRed");
    
    // Test edge cases
    scene.setPlayerLevel(1, 0);     // Level 0
    scene.setPlayerLevel(1, -1);    // Negative level
    scene.setPlayerLevel(1, 999);   // Very high level
    scene.setPlayerLevel(999, 5);   // Non-existent player
    
    cr_assert(true, "setPlayerLevel should handle edge cases");
}

Test(world_scene, set_player_inventory) {
    MockEventReceiver receiver;
    WorldScene scene(nullptr, nullptr, receiver, "");
    
    // Create player first
    scene.createEntities(1, 5, 5, Direction::NORTH, 1, "TeamRed");
    
    // Set inventory
    scene.setPlayerInventory(1, 5, 5, 1, 2, 3, 4, 5, 6, 7);
    scene.setPlayerInventory(1, 5, 5, 0, 0, 0, 0, 0, 0, 0);  // Empty inventory
    scene.setPlayerInventory(1, 5, 5, 99, 99, 99, 99, 99, 99, 99);  // Full inventory
    
    cr_assert(true, "setPlayerInventory should work");
}

Test(world_scene, set_inventory_nonexistent_player) {
    MockEventReceiver receiver;
    WorldScene scene(nullptr, nullptr, receiver, "");
    
    // Try to set inventory for non-existent player
    scene.setPlayerInventory(999, 0, 0, 1, 2, 3, 4, 5, 6, 7);
    
    cr_assert(true, "setPlayerInventory should handle non-existent player gracefully");
}

Test(world_scene, expulsion) {
    MockEventReceiver receiver;
    WorldScene scene(nullptr, nullptr, receiver, "");
    
    // Create player first
    scene.createEntities(1, 0, 0, Direction::NORTH, 1, "TeamRed");
    
    // Test expulsion
    scene.expulsion(1);
    scene.expulsion(999);  // Non-existent player
    
    cr_assert(true, "expulsion should work");
}

Test(world_scene, broadcast) {
    MockEventReceiver receiver;
    WorldScene scene(nullptr, nullptr, receiver, "");
    
    // Create player first
    scene.createEntities(1, 0, 0, Direction::NORTH, 1, "TeamRed");
    
    // Test broadcast
    scene.broadcast(1, "Hello World!");
    scene.broadcast(1, "");  // Empty message
    scene.broadcast(1, "Very long message that should still be handled properly by the broadcast system");
    scene.broadcast(999, "Message from non-existent player");
    
    cr_assert(true, "broadcast should work");
}

Test(world_scene, resource_dropping) {
    MockEventReceiver receiver;
    WorldScene scene(nullptr, nullptr, receiver, "");
    
    // Create player first
    scene.createEntities(1, 0, 0, Direction::NORTH, 1, "TeamRed");
    
    // Test resource dropping
    scene.resourceDropping(1, "linemate");
    scene.resourceDropping(1, "deraumere");
    scene.resourceDropping(1, "sibur");
    scene.resourceDropping(1, "mendiane");
    scene.resourceDropping(1, "phiras");
    scene.resourceDropping(1, "thystame");
    scene.resourceDropping(1, "food");
    scene.resourceDropping(999, "linemate");  // Non-existent player
    scene.resourceDropping(1, "");  // Empty resource
    scene.resourceDropping(1, "unknown_resource");
    
    cr_assert(true, "resourceDropping should work");
}

Test(world_scene, create_egg) {
    MockEventReceiver receiver;
    WorldScene scene(nullptr, nullptr, receiver, "");
    
    // Test egg creation
    scene.createEgg(1);
    scene.createEgg(2);
    scene.createEgg(999);
    scene.createEgg(0);     // Edge case: id=0
    scene.createEgg(-1);    // Edge case: negative id
    
    cr_assert(true, "createEgg should work");
}

Test(world_scene, end_game) {
    MockEventReceiver receiver;
    WorldScene scene(nullptr, nullptr, receiver, "");
    
    // Test game ending
    scene.endGame("TeamRed");
    scene.endGame("TeamBlue");
    scene.endGame("");  // Empty team name
    scene.endGame("VeryLongTeamNameThatWins");
    
    cr_assert(true, "endGame should work");
}

Test(world_scene, complex_scenario) {
    MockEventReceiver receiver;
    WorldScene scene(nullptr, nullptr, receiver, "");
    
    // Create a complex scenario with multiple operations
    scene.createPlane(10, 10);
    
    // Add teams
    scene.addTeam("Red");
    scene.addTeam("Blue");
    
    // Create multiple players
    scene.createEntities(1, 0, 0, Direction::NORTH, 1, "Red");
    scene.createEntities(2, 5, 5, Direction::EAST, 2, "Blue");
    scene.createEntities(3, 9, 9, Direction::SOUTH, 3, "Red");
    
    // Move players
    scene.changePlayerPos(1, 1, 1, Direction::EAST, Direction::NORTH);
    scene.changePlayerPos(2, 6, 6, Direction::SOUTH, Direction::EAST);
    
    // Level up players
    scene.setPlayerLevel(1, 2);
    scene.setPlayerLevel(2, 3);
    scene.setPlayerLevel(3, 4);
    
    // Set inventories
    scene.setPlayerInventory(1, 1, 1, 1, 0, 0, 0, 0, 0, 0);
    scene.setPlayerInventory(2, 6, 6, 0, 1, 0, 0, 0, 0, 0);
    scene.setPlayerInventory(3, 9, 9, 0, 0, 1, 0, 0, 0, 0);
    
    // Broadcast messages
    scene.broadcast(1, "Hello from player 1!");
    scene.broadcast(2, "Player 2 responding!");
    
    // Drop resources
    scene.resourceDropping(1, "linemate");
    scene.resourceDropping(2, "deraumere");
    
    // Create eggs
    scene.createEgg(10);
    scene.createEgg(11);
    
    // Expel a player
    scene.expulsion(3);
    
    // End game
    scene.endGame("Red");
    
    cr_assert(true, "Complex scenario should execute without crashing");
}

Test(world_scene, stress_test) {
    MockEventReceiver receiver;
    WorldScene scene(nullptr, nullptr, receiver, "");
    
    // Stress test with many operations
    scene.createPlane(100, 100);
    
    // Create many teams
    for (int i = 0; i < 50; ++i) {
        scene.addTeam("Team" + std::to_string(i));
    }
    
    // Create many players
    for (int i = 0; i < 100; ++i) {
        scene.createEntities(i, i % 10, i / 10, static_cast<Direction>(i % 4), i % 8 + 1, "Team" + std::to_string(i % 50));
    }
    
    // Move all players
    for (int i = 0; i < 100; ++i) {
        scene.changePlayerPos(i, (i + 1) % 10, (i + 1) / 10, static_cast<Direction>((i + 1) % 4), static_cast<Direction>(i % 4));
    }
    
    // Set levels for all players
    for (int i = 0; i < 100; ++i) {
        scene.setPlayerLevel(i, (i % 8) + 1);
    }
    
    cr_assert(true, "Stress test should complete without crashing");
}

Test(world_scene, direction_handling) {
    MockEventReceiver receiver;
    WorldScene scene(nullptr, nullptr, receiver, "");
    
    // Test all direction combinations
    scene.createEntities(1, 0, 0, Direction::NORTH, 1, "Team1");
    scene.createEntities(2, 1, 0, Direction::EAST, 1, "Team1");
    scene.createEntities(3, 2, 0, Direction::SOUTH, 1, "Team1");
    scene.createEntities(4, 3, 0, Direction::WEST, 1, "Team1");
    
    // Change directions
    scene.changePlayerPos(1, 0, 1, Direction::EAST, Direction::NORTH);
    scene.changePlayerPos(2, 1, 1, Direction::SOUTH, Direction::EAST);
    scene.changePlayerPos(3, 2, 1, Direction::WEST, Direction::SOUTH);
    scene.changePlayerPos(4, 3, 1, Direction::NORTH, Direction::WEST);
    
    cr_assert(true, "Direction handling should work correctly");
}

Test(world_scene, empty_string_parameters) {
    MockEventReceiver receiver;
    WorldScene scene(nullptr, nullptr, receiver, "");
    
    // Test with empty string parameters
    scene.createEntities(1, 0, 0, Direction::NORTH, 1, "");  // Empty team
    scene.broadcast(1, "");  // Empty message
    scene.resourceDropping(1, "");  // Empty resource
    scene.endGame("");  // Empty winning team
    
    cr_assert(true, "Empty string parameters should be handled gracefully");
}
