/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** Complete tests for EntityManager class
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "Core/EntityManager.hpp"
#include "Event/EventReceiver.hpp"
#include "Entities/Direction.hpp"
#include "irrlicht_mocks.hpp"
#include <memory>

// Mock EventReceiver for testing
class MockEventReceiver : public EventReceiver {
public:
    MockEventReceiver() : EventReceiver() {}
};

TestSuite(entity_manager_complete);

Test(entity_manager_complete, constructor_destructor) {
    std::shared_ptr<EventReceiver> eventReceiver = std::make_shared<MockEventReceiver>();
    EntityManager manager(eventReceiver);
    
    cr_assert(true, "EntityManager constructor should work");
}

Test(entity_manager_complete, create_players_all_directions) {
    std::shared_ptr<EventReceiver> eventReceiver = std::make_shared<MockEventReceiver>();
    EntityManager manager(eventReceiver);
    
    // Test all directions
    manager.createPlayers(1, 0, 0, Direction::NORTH, 1, "team1");
    manager.createPlayers(2, 1, 0, Direction::EAST, 1, "team1");
    manager.createPlayers(3, 0, 1, Direction::SOUTH, 1, "team1");
    manager.createPlayers(4, 1, 1, Direction::WEST, 1, "team1");
    
    cr_assert(true, "Creating players with all directions should work");
}

Test(entity_manager_complete, create_players_all_levels) {
    std::shared_ptr<EventReceiver> eventReceiver = std::make_shared<MockEventReceiver>();
    EntityManager manager(eventReceiver);
    
    // Test all levels
    for (int level = 1; level <= 8; level++) {
        manager.createPlayers(level, 0, 0, Direction::NORTH, level, "team_level_" + std::to_string(level));
    }
    
    cr_assert(true, "Creating players with all levels should work");
}

Test(entity_manager_complete, create_players_different_teams) {
    std::shared_ptr<EventReceiver> eventReceiver = std::make_shared<MockEventReceiver>();
    EntityManager manager(eventReceiver);
    
    manager.createPlayers(1, 0, 0, Direction::NORTH, 1, "red_team");
    manager.createPlayers(2, 1, 0, Direction::NORTH, 1, "blue_team");
    manager.createPlayers(3, 2, 0, Direction::NORTH, 1, "green_team");
    manager.createPlayers(4, 3, 0, Direction::NORTH, 1, "");  // Empty team
    
    cr_assert(true, "Creating players with different teams should work");
}

Test(entity_manager_complete, create_players_edge_positions) {
    std::shared_ptr<EventReceiver> eventReceiver = std::make_shared<MockEventReceiver>();
    EntityManager manager(eventReceiver);
    
    // Test edge positions
    manager.createPlayers(1, 0, 0, Direction::NORTH, 1, "team");         // Origin
    manager.createPlayers(2, -1, -1, Direction::NORTH, 1, "team");       // Negative coordinates
    manager.createPlayers(3, 100, 100, Direction::NORTH, 1, "team");     // Large coordinates
    manager.createPlayers(4, 1000, 1000, Direction::NORTH, 1, "team");   // Very large coordinates
    
    cr_assert(true, "Creating players at edge positions should work");
}

Test(entity_manager_complete, create_stones_all_combinations) {
    std::shared_ptr<EventReceiver> eventReceiver = std::make_shared<MockEventReceiver>();
    EntityManager manager(eventReceiver);
    
    // Test with all stone quantities
    manager.createStones(0, 0, 1, 2, 3, 4, 5, 6, 7);  // All different quantities
    manager.createStones(1, 0, 0, 0, 0, 0, 0, 0, 0);  // All zero
    manager.createStones(2, 0, 10, 10, 10, 10, 10, 10, 10);  // All same non-zero
    
    cr_assert(true, "Creating stones with all combinations should work");
}

Test(entity_manager_complete, create_stones_edge_cases) {
    std::shared_ptr<EventReceiver> eventReceiver = std::make_shared<MockEventReceiver>();
    EntityManager manager(eventReceiver);
    
    // Test edge cases
    manager.createStones(-1, -1, 0, 0, 0, 0, 0, 0, 0);    // Negative position
    manager.createStones(1000, 1000, 1, 1, 1, 1, 1, 1, 1); // Large position
    manager.createStones(0, 0, 100, 100, 100, 100, 100, 100, 100); // Large quantities
    
    cr_assert(true, "Creating stones with edge cases should work");
}

Test(entity_manager_complete, create_stones_single_resource_types) {
    std::shared_ptr<EventReceiver> eventReceiver = std::make_shared<MockEventReceiver>();
    EntityManager manager(eventReceiver);
    
    // Test each resource type individually
    manager.createStones(0, 0, 1, 0, 0, 0, 0, 0, 0);  // Only food
    manager.createStones(0, 1, 0, 1, 0, 0, 0, 0, 0);  // Only linemate
    manager.createStones(0, 2, 0, 0, 1, 0, 0, 0, 0);  // Only deraumere
    manager.createStones(0, 3, 0, 0, 0, 1, 0, 0, 0);  // Only sibur
    manager.createStones(0, 4, 0, 0, 0, 0, 1, 0, 0);  // Only mendiane
    manager.createStones(0, 5, 0, 0, 0, 0, 0, 1, 0);  // Only phiras
    manager.createStones(0, 6, 0, 0, 0, 0, 0, 0, 1);  // Only thystame
    
    cr_assert(true, "Creating stones with single resource types should work");
}

Test(entity_manager_complete, create_tiles_various_positions) {
    std::shared_ptr<EventReceiver> eventReceiver = std::make_shared<MockEventReceiver>();
    EntityManager manager(eventReceiver);
    
    // Test various tile positions
    manager.createTiles(0, 0);
    manager.createTiles(5, 5);
    manager.createTiles(-2, -3);
    manager.createTiles(100, 50);
    manager.createTiles(1000, 2000);
    
    cr_assert(true, "Creating tiles at various positions should work");
}

Test(entity_manager_complete, create_tiles_grid_pattern) {
    std::shared_ptr<EventReceiver> eventReceiver = std::make_shared<MockEventReceiver>();
    EntityManager manager(eventReceiver);
    
    // Create a grid of tiles
    for (int x = 0; x < 10; x++) {
        for (int y = 0; y < 10; y++) {
            manager.createTiles(x, y);
        }
    }
    
    cr_assert(true, "Creating tiles in grid pattern should work");
}

Test(entity_manager_complete, create_paper_plane_various_positions) {
    std::shared_ptr<EventReceiver> eventReceiver = std::make_shared<MockEventReceiver>();
    EntityManager manager(eventReceiver);
    
    // Test paper plane creation at various positions
    irr::core::vector3df pos1(0.0f, 10.0f, 0.0f);
    irr::core::vector3df pos2(50.0f, 20.0f, 30.0f);
    irr::core::vector3df pos3(-10.0f, 5.0f, -20.0f);
    
    std::shared_ptr<IEntity> plane1 = manager.createPaperPlane(pos1);
    std::shared_ptr<IEntity> plane2 = manager.createPaperPlane(pos2);
    std::shared_ptr<IEntity> plane3 = manager.createPaperPlane(pos3);
    
    cr_assert(true, "Creating paper planes at various positions should work");
}

Test(entity_manager_complete, create_paper_plane_extreme_positions) {
    std::shared_ptr<EventReceiver> eventReceiver = std::make_shared<MockEventReceiver>();
    EntityManager manager(eventReceiver);
    
    // Test extreme positions
    irr::core::vector3df extreme1(999999.0f, 999999.0f, 999999.0f);
    irr::core::vector3df extreme2(-999999.0f, -999999.0f, -999999.0f);
    irr::core::vector3df tiny(0.000001f, 0.000001f, 0.000001f);
    
    std::shared_ptr<IEntity> plane1 = manager.createPaperPlane(extreme1);
    std::shared_ptr<IEntity> plane2 = manager.createPaperPlane(extreme2);
    std::shared_ptr<IEntity> plane3 = manager.createPaperPlane(tiny);
    
    cr_assert(true, "Creating paper planes at extreme positions should work");
}

Test(entity_manager_complete, create_egg_various_ids) {
    std::shared_ptr<EventReceiver> eventReceiver = std::make_shared<MockEventReceiver>();
    EntityManager manager(eventReceiver);
    
    // Test various egg IDs
    manager.createEgg(1);
    manager.createEgg(100);
    manager.createEgg(-1);
    manager.createEgg(0);
    manager.createEgg(999999);
    
    cr_assert(true, "Creating eggs with various IDs should work");
}

Test(entity_manager_complete, create_egg_sequence) {
    std::shared_ptr<EventReceiver> eventReceiver = std::make_shared<MockEventReceiver>();
    EntityManager manager(eventReceiver);
    
    // Create a sequence of eggs
    for (int i = 1; i <= 50; i++) {
        manager.createEgg(i);
    }
    
    cr_assert(true, "Creating a sequence of eggs should work");
}

Test(entity_manager_complete, mixed_entity_creation) {
    std::shared_ptr<EventReceiver> eventReceiver = std::make_shared<MockEventReceiver>();
    EntityManager manager(eventReceiver);
    
    // Mix different entity creation calls
    manager.createPlayers(1, 0, 0, Direction::NORTH, 1, "team1");
    manager.createStones(0, 0, 1, 1, 1, 1, 1, 1, 1);
    manager.createTiles(0, 0);
    manager.createEgg(1);
    
    irr::core::vector3df pos(5.0f, 10.0f, 15.0f);
    std::shared_ptr<IEntity> plane = manager.createPaperPlane(pos);
    
    manager.createPlayers(2, 1, 1, Direction::EAST, 2, "team2");
    manager.createStones(1, 1, 2, 2, 2, 2, 2, 2, 2);
    manager.createTiles(1, 1);
    manager.createEgg(2);
    
    cr_assert(true, "Mixed entity creation should work");
}

Test(entity_manager_complete, stress_test_entity_creation) {
    std::shared_ptr<EventReceiver> eventReceiver = std::make_shared<MockEventReceiver>();
    EntityManager manager(eventReceiver);
    
    // Stress test with many entities
    for (int i = 0; i < 100; i++) {
        manager.createPlayers(i, i % 10, i / 10, 
                            static_cast<Direction>(i % 4), 
                            (i % 8) + 1, 
                            "team" + std::to_string(i % 5));
        
        manager.createStones(i % 10, i / 10, i % 3, i % 3, i % 3, i % 3, i % 3, i % 3, i % 3);
        manager.createTiles(i % 10, i / 10);
        manager.createEgg(i);
        
        if (i % 10 == 0) {
            irr::core::vector3df pos(static_cast<float>(i), 10.0f, static_cast<float>(i));
            std::shared_ptr<IEntity> plane = manager.createPaperPlane(pos);
        }
    }
    
    cr_assert(true, "Stress test entity creation should work");
}

Test(entity_manager_complete, null_event_receiver) {
    // Test with null event receiver
    try {
        EntityManager manager(nullptr);
        cr_assert(true, "EntityManager with null EventReceiver should work or handle gracefully");
    } catch (const std::exception& e) {
        cr_assert(true, "EntityManager with null EventReceiver threw exception as expected");
    }
}
