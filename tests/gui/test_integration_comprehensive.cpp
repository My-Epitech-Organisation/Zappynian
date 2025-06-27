/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Integration tests showcasing the comprehensive testing infrastructure
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "../../gui/Core/GameState.hpp"
#include "../../gui/Entities/PlayerEntity.hpp"
#include "../../gui/Entities/Inventory.hpp"

using namespace Zappy;

TestSuite(IntegrationTests, .timeout = 15);

Test(IntegrationTests, game_state_player_inventory_integration) {
    // Create a complete game scenario
    GameState gameState;
    gameState.setMapSize(20, 20);
    gameState.setTimeUnit(100);
    
    // Add teams
    gameState.addTeam("TeamAlpha");
    gameState.addTeam("TeamBeta");
    
    // Create players with different levels and positions
    gameState.addPlayer(1, irr::core::vector3df(5, 0, 5), Direction::NORTH, 1, "TeamAlpha");
    gameState.addPlayer(2, irr::core::vector3df(10, 0, 10), Direction::SOUTH, 3, "TeamBeta");
    gameState.addPlayer(3, irr::core::vector3df(15, 0, 15), Direction::EAST, 5, "TeamAlpha");
    
    // Verify players exist
    cr_assert_not_null(gameState.getPlayer(1));
    cr_assert_not_null(gameState.getPlayer(2));
    cr_assert_not_null(gameState.getPlayer(3));
    
    // Test player inventory integration
    Inventory inventory1;
    inventory1.addItem("food", 10);
    inventory1.addItem("linemate", 5);
    inventory1.addItem("deraumere", 3);
    
    gameState.updatePlayerInventory(1, irr::core::vector3df(5, 0, 5), inventory1);
    
    // Verify team distribution
    auto allPlayers = gameState.getAllPlayers();
    cr_assert_eq(allPlayers.size(), 3);
    
    int teamAlphaCount = 0, teamBetaCount = 0;
    for (const auto* player : allPlayers) {
        if (player->getTeam() == "TeamAlpha") teamAlphaCount++;
        else if (player->getTeam() == "TeamBeta") teamBetaCount++;
    }
    
    cr_assert_eq(teamAlphaCount, 2);
    cr_assert_eq(teamBetaCount, 1);
}

Test(IntegrationTests, player_entity_inventory_lifecycle) {
    // Test complete player lifecycle with inventory management
    irr::core::vector3df position(8, 0, 12);
    irr::core::vector3df scale(1.5, 1.5, 1.5);
    std::vector<irr::io::path> textures = {"warrior.png", "warrior_normal.png"};
    irr::io::path modelPath("warrior.b3d");
    
    PlayerEntity player(42, position, scale, Direction::WEST, "AdvancedTeam", textures, 4, modelPath);
    
    // Verify initial state
    cr_assert_eq(player.getId(), 42);
    cr_assert_eq(player.getLevel(), 4);
    cr_assert_eq(player.getDirection(), Direction::WEST);
    cr_assert_str_eq(player.getTeam().c_str(), "AdvancedTeam");
    
    // Test inventory management through player
    auto& inventory = player.getInventory();
    
    // Simulate resource gathering
    inventory.addItem("food", 25);
    inventory.addItem("linemate", 10);
    inventory.addItem("deraumere", 8);
    inventory.addItem("sibur", 6);
    inventory.addItem("mendiane", 4);
    inventory.addItem("phiras", 3);
    inventory.addItem("thystame", 2);
    
    // Verify all resources
    cr_assert_eq(inventory.getItemQuantity("food"), 25);
    cr_assert_eq(inventory.getItemQuantity("linemate"), 10);
    cr_assert_eq(inventory.getItemQuantity("deraumere"), 8);
    cr_assert_eq(inventory.getItemQuantity("sibur"), 6);
    cr_assert_eq(inventory.getItemQuantity("mendiane"), 4);
    cr_assert_eq(inventory.getItemQuantity("phiras"), 3);
    cr_assert_eq(inventory.getItemQuantity("thystame"), 2);
    
    // Simulate level-up requirements consumption
    inventory.removeItem("linemate", 1);
    inventory.removeItem("deraumere", 1);
    inventory.removeItem("sibur", 2);
    
    cr_assert_eq(inventory.getItemQuantity("linemate"), 9);
    cr_assert_eq(inventory.getItemQuantity("deraumere"), 7);
    cr_assert_eq(inventory.getItemQuantity("sibur"), 4);
    
    // Level up player
    player.setLevel(5);
    cr_assert_eq(player.getLevel(), 5);
    
    // Move player
    player.setPosition(irr::core::vector3df(12, 0, 8));
    auto newPos = player.getPosition();
    cr_assert_eq(newPos.X, 12);
    cr_assert_eq(newPos.Y, 0);
    cr_assert_eq(newPos.Z, 8);
}

Test(IntegrationTests, complex_game_scenario) {
    // Simulate a complex game scenario with multiple interactions
    GameState gameState;
    gameState.setMapSize(30, 30);
    gameState.setTimeUnit(50);
    
    // Create multiple teams
    std::vector<std::string> teams = {"Red", "Blue", "Green", "Yellow"};
    for (const auto& team : teams) {
        gameState.addTeam(team);
    }
    
    // Add players distributed across teams
    int playerId = 1;
    for (int teamIdx = 0; teamIdx < 4; ++teamIdx) {
        for (int playerInTeam = 0; playerInTeam < 3; ++playerInTeam) {
            irr::core::vector3df pos(5 + teamIdx * 7, 0, 5 + playerInTeam * 5);
            Direction dir = static_cast<Direction>(teamIdx);
            gameState.addPlayer(playerId++, pos, dir, 1 + playerInTeam, teams[teamIdx]);
        }
    }
    
    // Verify 12 players total
    auto allPlayers = gameState.getAllPlayers();
    cr_assert_eq(allPlayers.size(), 12);
    
    // Add eggs for each team
    int eggId = 100;
    for (int teamIdx = 0; teamIdx < 4; ++teamIdx) {
        irr::core::vector3df eggPos(10 + teamIdx * 5, 0, 10);
        gameState.addEgg(eggId++, eggPos, teams[teamIdx]);
    }
    
    // Verify eggs
    auto allEggs = gameState.getAllEggs();
    cr_assert_eq(allEggs.size(), 4);
    
    // Simulate incantation
    std::vector<int> incantationPlayers = {1, 2, 3}; // Red team players
    gameState.startIncantation(irr::core::vector3df(5, 0, 5), 1, incantationPlayers);
    
    auto incantations = gameState.getAllIncantations();
    cr_assert_eq(incantations.size(), 1);
    cr_assert(incantations[0].active);
    
    // Complete successful incantation
    gameState.endIncantation(irr::core::vector3df(5, 0, 5), true);
    
    // Verify level ups
    cr_assert_eq(gameState.getPlayer(1)->getLevel(), 2);
    cr_assert_eq(gameState.getPlayer(2)->getLevel(), 3);
    cr_assert_eq(gameState.getPlayer(3)->getLevel(), 4);
    
    // Test resource distribution on tiles
    Inventory tileResources;
    tileResources.addItem("food", 5);
    tileResources.addItem("linemate", 2);
    
    gameState.updateTile(irr::core::vector3df(15, 0, 15), tileResources);
    
    const TileEntity* tile = gameState.getTile(irr::core::vector3df(15, 0, 15));
    cr_assert_not_null(tile);
}

Test(IntegrationTests, inventory_stress_test) {
    // Stress test inventory operations
    Inventory inventory;
    
    // Add large quantities of all items
    std::vector<std::string> items = {
        "food", "linemate", "deraumere", "sibur", 
        "mendiane", "phiras", "thystame", "egg"
    };
    
    for (size_t i = 0; i < items.size(); ++i) {
        size_t quantity = (i + 1) * 100;
        inventory.addItem(items[i], quantity);
        cr_assert_eq(inventory.getItemQuantity(items[i]), quantity);
    }
    
    // Perform many operations
    for (int operation = 0; operation < 1000; ++operation) {
        int itemIdx = operation % items.size();
        
        if (operation % 3 == 0) {
            inventory.addItem(items[itemIdx], 1);
        } else if (operation % 3 == 1) {
            inventory.removeItem(items[itemIdx], 1);
        }
        // Every third operation is a query (already done in loop condition)
    }
    
    // Verify final state is consistent
    size_t totalItems = 0;
    for (const auto& item : items) {
        totalItems += inventory.getItemQuantity(item);
    }
    
    cr_assert_gt(totalItems, 0);
    cr_assert_lt(totalItems, 10000); // Reasonable upper bound
}

Test(IntegrationTests, error_handling_robustness) {
    // Test error handling and robustness
    GameState gameState;
    gameState.setMapSize(10, 10);
    
    // Test operations on invalid/non-existent entities
    gameState.updatePlayerPosition(999, irr::core::vector3df(5, 0, 5), Direction::NORTH);
    gameState.updatePlayerLevel(999, 5);
    gameState.removePlayer(999);
    
    // These should not crash - just verify state is consistent
    cr_assert_null(gameState.getPlayer(999));
    cr_assert(gameState.isEmpty());
    
    // Test invalid positions
    gameState.addPlayer(1, irr::core::vector3df(-1, 0, 5), Direction::NORTH, 1, "test");
    gameState.addPlayer(2, irr::core::vector3df(5, 0, -1), Direction::NORTH, 1, "test");
    gameState.addPlayer(3, irr::core::vector3df(11, 0, 5), Direction::NORTH, 1, "test");
    
    // Only valid players should be added
    auto players = gameState.getAllPlayers();
    cr_assert_eq(players.size(), 0); // All positions were invalid
    
    // Test inventory edge cases
    Inventory inventory;
    inventory.removeItem("nonexistent", 100); // Should not crash
    cr_assert_eq(inventory.getItemQuantity("nonexistent"), 0);
    
    inventory.addItem("test", 10);
    inventory.removeItem("test", 20); // Remove more than available
    cr_assert_eq(inventory.getItemQuantity("test"), 10); // Should remain unchanged
}
