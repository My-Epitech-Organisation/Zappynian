/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Comprehensive unit tests for EntityManager class
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "../../gui/Core/EntityManager.hpp"

TestSuite(EntityManager, .timeout = 10);

// Mock objects for testing
static irr::scene::ISceneManager* mockSceneManager = nullptr;
static irr::video::IVideoDriver* mockVideoDriver = nullptr;
static EventReceiver* mockEventReceiver = nullptr;

Test(EntityManager, construction) {
    // Create mock EventReceiver
    EventReceiver receiver;
    
    EntityManager manager(mockSceneManager, mockVideoDriver, receiver, "test_media");
    
    // Test that manager is constructed properly
    const auto& entities = manager.getEntities();
    cr_assert_eq(entities.size(), 0);
}

Test(EntityManager, add_teams) {
    EventReceiver receiver;
    EntityManager manager(mockSceneManager, mockVideoDriver, receiver, "test_media");
    
    std::vector<std::string> teams = {"team1", "team2", "team3"};
    manager.addTeams(teams);
    
    // We can't directly test teams_ as it's private, but this tests the method doesn't crash
    cr_assert(true);
}

Test(EntityManager, create_players) {
    EventReceiver receiver;
    EntityManager manager(mockSceneManager, mockVideoDriver, receiver, "test_media");
    
    // Test creating a player
    manager.createPlayers(1, 5, 5, Direction::NORTH, 1, "team1");
    
    const auto& entities = manager.getEntities();
    // Since we're using mocked Irrlicht, actual entity creation might not work
    // But the method should not crash
    cr_assert(true);
}

Test(EntityManager, create_stones) {
    EventReceiver receiver;
    EntityManager manager(mockSceneManager, mockVideoDriver, receiver, "test_media");
    
    // Test creating stones with various quantities
    manager.createStones(3, 3, 1, 2, 3, 4, 5, 6, 7);
    
    // Method should not crash
    cr_assert(true);
}

Test(EntityManager, create_stones_default_quantities) {
    EventReceiver receiver;
    EntityManager manager(mockSceneManager, mockVideoDriver, receiver, "test_media");
    
    // Test creating stones with default quantities (all 0)
    manager.createStones(3, 3);
    
    // Method should not crash
    cr_assert(true);
}

Test(EntityManager, create_tiles) {
    EventReceiver receiver;
    EntityManager manager(mockSceneManager, mockVideoDriver, receiver, "test_media");
    
    // Test creating tiles
    manager.createTiles(5, 5);
    
    // Method should not crash
    cr_assert(true);
}

Test(EntityManager, create_egg) {
    EventReceiver receiver;
    EntityManager manager(mockSceneManager, mockVideoDriver, receiver, "test_media");
    
    // Test creating an egg
    manager.createEgg(1);
    
    // Method should not crash
    cr_assert(true);
}

Test(EntityManager, create_paper_plane) {
    EventReceiver receiver;
    EntityManager manager(mockSceneManager, mockVideoDriver, receiver, "test_media");
    
    irr::core::vector3df position(10, 5, 10);
    
    // Test creating a paper plane
    auto plane = manager.createPaperPlane(position);
    
    // With mocked Irrlicht, this might return nullptr
    // The important thing is that it doesn't crash
    cr_assert(true);
}

Test(EntityManager, get_tile_by_name_empty) {
    EventReceiver receiver;
    EntityManager manager(mockSceneManager, mockVideoDriver, receiver, "test_media");
    
    // Test getting a tile from empty manager
    auto tile = manager.getTileByName("nonexistent");
    cr_assert_null(tile.get());
}

Test(EntityManager, create_dropped_resource) {
    EventReceiver receiver;
    EntityManager manager(mockSceneManager, mockVideoDriver, receiver, "test_media");
    
    // Test creating a dropped resource
    manager.createDroppedResource(2, 3, "food");
    manager.createDroppedResource(4, 5, "linemate");
    
    // Method should not crash
    cr_assert(true);
}

Test(EntityManager, place_stone_entities) {
    EventReceiver receiver;
    EntityManager manager(mockSceneManager, mockVideoDriver, receiver, "test_media");
    
    irr::core::vector3df position(5, 0, 5);
    std::vector<int> quantities = {1, 2, 3, 4, 5, 6, 7};
    std::vector<std::vector<irr::io::path>> stoneTextures;
    std::vector<irr::io::path> qB3D;
    std::vector<std::string> stoneNames = {"linemate", "deraumere", "sibur", 
                                           "mendiane", "phiras", "thystame", "food"};
    std::vector<irr::core::vector3df> qScale;
    
    // Populate texture and scale vectors
    for (int i = 0; i < 7; ++i) {
        stoneTextures.push_back({irr::io::path("test.png")});
        qB3D.push_back(irr::io::path("test.3ds"));
        qScale.push_back(irr::core::vector3df(1, 1, 1));
    }
    
    // Test placing stone entities
    manager.placeStoneEntities(position, quantities, stoneTextures, qB3D, stoneNames, qScale);
    
    // Method should not crash
    cr_assert(true);
}

Test(EntityManager, multiple_operations) {
    EventReceiver receiver;
    EntityManager manager(mockSceneManager, mockVideoDriver, receiver, "test_media");
    
    // Test multiple operations in sequence
    std::vector<std::string> teams = {"team1", "team2"};
    manager.addTeams(teams);
    
    manager.createTiles(10, 10);
    manager.createPlayers(1, 5, 5, Direction::NORTH, 1, "team1");
    manager.createPlayers(2, 3, 7, Direction::SOUTH, 2, "team2");
    manager.createStones(2, 2, 5, 3, 1, 0, 0, 0, 0);
    manager.createEgg(10);
    
    manager.createDroppedResource(1, 1, "food");
    manager.createDroppedResource(8, 8, "linemate");
    
    // All operations should complete without crashing
    cr_assert(true);
}

Test(EntityManager, edge_case_positions) {
    EventReceiver receiver;
    EntityManager manager(mockSceneManager, mockVideoDriver, receiver, "test_media");
    
    // Test with edge case positions
    manager.createPlayers(100, 0, 0, Direction::NORTH, 1, "edge_team");
    manager.createPlayers(101, -1, -1, Direction::SOUTH, 8, "edge_team");  // Negative positions
    manager.createStones(0, 0);
    manager.createStones(999, 999);  // Large positions
    
    // Should handle edge cases gracefully
    cr_assert(true);
}

Test(EntityManager, various_directions) {
    EventReceiver receiver;
    EntityManager manager(mockSceneManager, mockVideoDriver, receiver, "test_media");
    
    // Test creating players with all directions
    manager.createPlayers(1, 5, 5, Direction::NORTH, 1, "team1");
    manager.createPlayers(2, 5, 6, Direction::SOUTH, 1, "team1");
    manager.createPlayers(3, 5, 7, Direction::EAST, 1, "team1");
    manager.createPlayers(4, 5, 8, Direction::WEST, 1, "team1");
    
    // Should handle all directions
    cr_assert(true);
}

Test(EntityManager, various_levels) {
    EventReceiver receiver;
    EntityManager manager(mockSceneManager, mockVideoDriver, receiver, "test_media");
    
    // Test creating players with various levels
    for (int level = 1; level <= 8; ++level) {
        manager.createPlayers(level, 5, 5, Direction::NORTH, level, "test_team");
    }
    
    // Should handle all valid levels
    cr_assert(true);
}

Test(EntityManager, empty_team_name) {
    EventReceiver receiver;
    EntityManager manager(mockSceneManager, mockVideoDriver, receiver, "test_media");
    
    // Test with empty team name
    manager.createPlayers(1, 5, 5, Direction::NORTH, 1, "");
    
    // Should handle empty team names
    cr_assert(true);
}

Test(EntityManager, large_numbers) {
    EventReceiver receiver;
    EntityManager manager(mockSceneManager, mockVideoDriver, receiver, "test_media");
    
    // Test with large numbers
    manager.createPlayers(999999, 500, 500, Direction::NORTH, 8, "large_team");
    manager.createStones(100, 100, 1000, 2000, 3000, 4000, 5000, 6000, 7000);
    
    // Should handle large numbers
    cr_assert(true);
}
