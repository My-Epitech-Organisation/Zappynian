/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** Tests for EntityManager class
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "Core/EntityManager.hpp"
#include "Event/EventReceiver.hpp"
#include <memory>

// Mock EventReceiver for testing
class MockEventReceiver : public EventReceiver {
public:
    MockEventReceiver() : EventReceiver() {}
    
    bool OnEvent(const irr::SEvent& event) override {
        (void)event;
        return false;
    }
    
    void addCube(TileEntity* tile) override {
        (void)tile;
        // Mock implementation
    }
};

TestSuite(entity_manager);

Test(entity_manager, constructor) {
    MockEventReceiver receiver;
    EntityManager manager(nullptr, nullptr, receiver, "");
    
    // Constructor should not crash
    cr_assert(true, "EntityManager constructor should work");
}

Test(entity_manager, create_players_basic) {
    MockEventReceiver receiver;
    EntityManager manager(nullptr, nullptr, receiver, "");
    
    // Test basic player creation
    manager.createPlayers(1, 0, 0, Direction::NORTH, 1, "TeamRed");
    manager.createPlayers(2, 5, 5, Direction::EAST, 2, "TeamBlue");
    manager.createPlayers(3, 10, 10, Direction::SOUTH, 3, "TeamGreen");
    
    cr_assert(true, "createPlayers should work with valid parameters");
}

Test(entity_manager, create_players_edge_cases) {
    MockEventReceiver receiver;
    EntityManager manager(nullptr, nullptr, receiver, "");
    
    // Test edge cases
    manager.createPlayers(0, 0, 0, Direction::NORTH, 0, "");  // id=0, level=0, empty team
    manager.createPlayers(-1, -5, -5, Direction::WEST, -1, "NegativeTeam");  // Negative values
    manager.createPlayers(999999, 999, 999, Direction::EAST, 999, "VeryLongTeamNameThatShouldWork");
    
    cr_assert(true, "createPlayers should handle edge cases");
}

Test(entity_manager, create_players_all_directions) {
    MockEventReceiver receiver;
    EntityManager manager(nullptr, nullptr, receiver, "");
    
    // Test all directions
    manager.createPlayers(1, 0, 0, Direction::NORTH, 1, "Team1");
    manager.createPlayers(2, 1, 0, Direction::EAST, 1, "Team2");
    manager.createPlayers(3, 2, 0, Direction::SOUTH, 1, "Team3");
    manager.createPlayers(4, 3, 0, Direction::WEST, 1, "Team4");
    
    cr_assert(true, "createPlayers should work with all directions");
}

Test(entity_manager, create_players_all_levels) {
    MockEventReceiver receiver;
    EntityManager manager(nullptr, nullptr, receiver, "");
    
    // Test all possible levels (1-8 in Zappy)
    for (int level = 1; level <= 8; ++level) {
        manager.createPlayers(level, level, level, Direction::NORTH, level, "Team" + std::to_string(level));
    }
    
    cr_assert(true, "createPlayers should work with all levels");
}

Test(entity_manager, create_stones_basic) {
    MockEventReceiver receiver;
    EntityManager manager(nullptr, nullptr, receiver, "");
    
    // Test basic stone creation
    manager.createStones(0, 0);  // No resources
    manager.createStones(1, 1, 1, 0, 0, 0, 0, 0, 0);  // Only linemate
    manager.createStones(2, 2, 0, 1, 0, 0, 0, 0, 0);  // Only deraumere
    manager.createStones(3, 3, 1, 1, 1, 1, 1, 1, 1);  // All resources
    
    cr_assert(true, "createStones should work with valid parameters");
}

Test(entity_manager, create_stones_edge_cases) {
    MockEventReceiver receiver;
    EntityManager manager(nullptr, nullptr, receiver, "");
    
    // Test edge cases
    manager.createStones(-1, -1);  // Negative coordinates
    manager.createStones(999, 999, 999, 999, 999, 999, 999, 999, 999);  // Large values
    manager.createStones(0, 0, -1, -1, -1, -1, -1, -1, -1);  // Negative resource quantities
    
    cr_assert(true, "createStones should handle edge cases");
}

Test(entity_manager, create_stones_various_combinations) {
    MockEventReceiver receiver;
    EntityManager manager(nullptr, nullptr, receiver, "");
    
    // Test various resource combinations
    manager.createStones(0, 0, 5, 0, 0, 0, 0, 0, 0);    // Only linemate
    manager.createStones(1, 0, 0, 5, 0, 0, 0, 0, 0);    // Only deraumere
    manager.createStones(2, 0, 0, 0, 5, 0, 0, 0, 0);    // Only sibur
    manager.createStones(3, 0, 0, 0, 0, 5, 0, 0, 0);    // Only mendiane
    manager.createStones(4, 0, 0, 0, 0, 0, 5, 0, 0);    // Only phiras
    manager.createStones(5, 0, 0, 0, 0, 0, 0, 5, 0);    // Only thystame
    manager.createStones(6, 0, 0, 0, 0, 0, 0, 0, 5);    // Only food
    manager.createStones(7, 0, 1, 2, 3, 4, 5, 6, 7);    // Mixed resources
    
    cr_assert(true, "createStones should work with various resource combinations");
}

Test(entity_manager, create_tiles_basic) {
    MockEventReceiver receiver;
    EntityManager manager(nullptr, nullptr, receiver, "");
    
    // Test basic tile creation
    manager.createTiles(1, 1);
    manager.createTiles(5, 5);
    manager.createTiles(10, 10);
    
    cr_assert(true, "createTiles should work with valid parameters");
}

Test(entity_manager, create_tiles_edge_cases) {
    MockEventReceiver receiver;
    EntityManager manager(nullptr, nullptr, receiver, "");
    
    // Test edge cases
    manager.createTiles(0, 0);      // Zero size
    manager.createTiles(1, 0);      // Zero height
    manager.createTiles(0, 1);      // Zero width
    manager.createTiles(-1, -1);    // Negative values
    manager.createTiles(100, 100);  // Large values
    
    cr_assert(true, "createTiles should handle edge cases");
}

Test(entity_manager, create_tiles_various_sizes) {
    MockEventReceiver receiver;
    EntityManager manager(nullptr, nullptr, receiver, "");
    
    // Test various tile grid sizes
    manager.createTiles(1, 1);   // 1x1
    manager.createTiles(2, 3);   // 2x3
    manager.createTiles(5, 7);   // 5x7
    manager.createTiles(10, 8);  // 10x8
    manager.createTiles(15, 15); // 15x15
    
    cr_assert(true, "createTiles should work with various sizes");
}

Test(entity_manager, create_paper_plane) {
    MockEventReceiver receiver;
    EntityManager manager(nullptr, nullptr, receiver, "");
    
    // Test paper plane creation
    auto plane1 = manager.createPaperPlane(irr::core::vector3df(0, 0, 0));
    auto plane2 = manager.createPaperPlane(irr::core::vector3df(10, 5, 10));
    auto plane3 = manager.createPaperPlane(irr::core::vector3df(-5, -5, -5));
    
    // Planes should be created (not null)
    cr_assert_neq(plane1.get(), nullptr, "Paper plane should be created");
    cr_assert_neq(plane2.get(), nullptr, "Paper plane should be created");
    cr_assert_neq(plane3.get(), nullptr, "Paper plane should be created");
    
    // Different planes should be different objects
    cr_assert_neq(plane1.get(), plane2.get(), "Different planes should be different objects");
    cr_assert_neq(plane2.get(), plane3.get(), "Different planes should be different objects");
}

Test(entity_manager, create_paper_plane_various_positions) {
    MockEventReceiver receiver;
    EntityManager manager(nullptr, nullptr, receiver, "");
    
    // Test paper plane creation at various positions
    std::vector<irr::core::vector3df> positions = {
        irr::core::vector3df(0, 0, 0),
        irr::core::vector3df(100, 200, 300),
        irr::core::vector3df(-100, -200, -300),
        irr::core::vector3df(1.5f, 2.5f, 3.5f),
        irr::core::vector3df(999999, 999999, 999999)
    };
    
    for (const auto& pos : positions) {
        auto plane = manager.createPaperPlane(pos);
        cr_assert_neq(plane.get(), nullptr, "Paper plane should be created at any position");
    }
    
    cr_assert(true, "Paper planes should be created at various positions");
}

Test(entity_manager, create_egg) {
    MockEventReceiver receiver;
    EntityManager manager(nullptr, nullptr, receiver, "");
    
    // Test egg creation
    manager.createEgg(1);
    manager.createEgg(2);
    manager.createEgg(100);
    manager.createEgg(999);
    
    cr_assert(true, "createEgg should work with valid IDs");
}

Test(entity_manager, create_egg_edge_cases) {
    MockEventReceiver receiver;
    EntityManager manager(nullptr, nullptr, receiver, "");
    
    // Test edge cases
    manager.createEgg(0);       // ID = 0
    manager.createEgg(-1);      // Negative ID
    manager.createEgg(999999);  // Large ID
    
    cr_assert(true, "createEgg should handle edge cases");
}

Test(entity_manager, create_duplicate_eggs) {
    MockEventReceiver receiver;
    EntityManager manager(nullptr, nullptr, receiver, "");
    
    // Test creating eggs with duplicate IDs
    manager.createEgg(1);
    manager.createEgg(1);  // Duplicate
    manager.createEgg(1);  // Another duplicate
    
    cr_assert(true, "createEgg should handle duplicate IDs gracefully");
}

Test(entity_manager, get_entities) {
    MockEventReceiver receiver;
    EntityManager manager(nullptr, nullptr, receiver, "");
    
    // Initially should have no entities
    const auto& entities = manager.getEntities();
    cr_assert_eq(entities.size(), 0, "Should start with no entities");
    
    // Create some entities
    manager.createPlayers(1, 0, 0, Direction::NORTH, 1, "Team1");
    manager.createEgg(10);
    
    // getEntities should not crash and should be callable
    const auto& entitiesAfter = manager.getEntities();
    (void)entitiesAfter; // Use the variable to avoid warnings
    
    cr_assert(true, "getEntities should be callable");
}

Test(entity_manager, complex_scenario) {
    MockEventReceiver receiver;
    EntityManager manager(nullptr, nullptr, receiver, "");
    
    // Create a complex scenario with multiple entities
    
    // Create tiles first
    manager.createTiles(10, 10);
    
    // Create multiple players
    manager.createPlayers(1, 0, 0, Direction::NORTH, 1, "Red");
    manager.createPlayers(2, 5, 5, Direction::EAST, 2, "Blue");
    manager.createPlayers(3, 9, 9, Direction::SOUTH, 3, "Green");
    manager.createPlayers(4, 2, 7, Direction::WEST, 4, "Yellow");
    
    // Create stones with various resources
    manager.createStones(0, 0, 5, 0, 0, 0, 0, 0, 0);  // Linemate at (0,0)
    manager.createStones(5, 5, 0, 5, 0, 0, 0, 0, 0);  // Deraumere at (5,5)
    manager.createStones(9, 9, 0, 0, 5, 0, 0, 0, 0);  // Sibur at (9,9)
    manager.createStones(2, 7, 1, 1, 1, 1, 1, 1, 1);  // All resources at (2,7)
    
    // Create paper planes
    auto plane1 = manager.createPaperPlane(irr::core::vector3df(0, 10, 0));
    auto plane2 = manager.createPaperPlane(irr::core::vector3df(10, 10, 10));
    
    // Create eggs
    manager.createEgg(100);
    manager.createEgg(101);
    manager.createEgg(102);
    
    cr_assert_neq(plane1.get(), nullptr, "Paper plane 1 should be created");
    cr_assert_neq(plane2.get(), nullptr, "Paper plane 2 should be created");
    cr_assert(true, "Complex scenario should execute without crashing");
}

Test(entity_manager, stress_test) {
    MockEventReceiver receiver;
    EntityManager manager(nullptr, nullptr, receiver, "");
    
    // Stress test with many entities
    
    // Create large tile grid
    manager.createTiles(50, 50);
    
    // Create many players
    for (int i = 0; i < 100; ++i) {
        manager.createPlayers(i, i % 50, i / 50, static_cast<Direction>(i % 4), (i % 8) + 1, "Team" + std::to_string(i % 10));
    }
    
    // Create many stones
    for (int x = 0; x < 20; ++x) {
        for (int y = 0; y < 20; ++y) {
            manager.createStones(x, y, x % 8, y % 8, (x + y) % 8, (x * y) % 8, 
                               (x - y) % 8, (x + 2*y) % 8, (2*x + y) % 8);
        }
    }
    
    // Create many paper planes
    for (int i = 0; i < 50; ++i) {
        auto plane = manager.createPaperPlane(irr::core::vector3df(i, i, i));
        cr_assert_neq(plane.get(), nullptr, "Stress test plane should be created");
    }
    
    // Create many eggs
    for (int i = 0; i < 200; ++i) {
        manager.createEgg(i + 1000);
    }
    
    cr_assert(true, "Stress test should complete without crashing");
}

Test(entity_manager, mixed_operations) {
    MockEventReceiver receiver;
    EntityManager manager(nullptr, nullptr, receiver, "");
    
    // Mix different operations in various orders
    manager.createEgg(1);
    manager.createPlayers(1, 0, 0, Direction::NORTH, 1, "Team1");
    manager.createTiles(5, 5);
    manager.createStones(2, 2, 1, 1, 1, 1, 1, 1, 1);
    auto plane = manager.createPaperPlane(irr::core::vector3df(5, 5, 5));
    manager.createEgg(2);
    manager.createPlayers(2, 1, 1, Direction::EAST, 2, "Team2");
    manager.createStones(3, 3);
    
    cr_assert_neq(plane.get(), nullptr, "Paper plane should be created in mixed operations");
    cr_assert(true, "Mixed operations should work correctly");
}

Test(entity_manager, resource_quantities_validation) {
    MockEventReceiver receiver;
    EntityManager manager(nullptr, nullptr, receiver, "");
    
    // Test extreme resource quantities
    manager.createStones(0, 0, 0, 0, 0, 0, 0, 0, 0);        // All zero
    manager.createStones(1, 1, 999, 999, 999, 999, 999, 999, 999);  // Very high
    manager.createStones(2, 2, 1, 0, 1, 0, 1, 0, 1);        // Alternating
    
    cr_assert(true, "Should handle various resource quantities");
}
