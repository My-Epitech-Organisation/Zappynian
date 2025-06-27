/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Comprehensive unit tests for GameState class
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "../../gui/Core/GameState.hpp"

using namespace Zappy;

TestSuite(GameState, .timeout = 10);

Test(GameState, default_construction) {
    GameState gameState;
    
    cr_assert_eq(gameState.getMapSize().X, 0);
    cr_assert_eq(gameState.getMapSize().Y, 0);
    cr_assert_eq(gameState.getMapSize().Z, 0);
    cr_assert_eq(gameState.getTimeUnit(), 100);
    cr_assert(gameState.isEmpty());
}

Test(GameState, set_map_size) {
    GameState gameState;
    
    gameState.setMapSize(10, 15);
    
    cr_assert_eq(gameState.getMapSize().X, 10);
    cr_assert_eq(gameState.getMapSize().Y, 15);
    cr_assert_eq(gameState.getMapSize().Z, 0);
}

Test(GameState, valid_position_check) {
    GameState gameState;
    gameState.setMapSize(10, 10);
    
    // Valid positions
    cr_assert(gameState.isValidPosition(irr::core::vector3df(0, 0, 0)));
    cr_assert(gameState.isValidPosition(irr::core::vector3df(5, 5, 0)));
    cr_assert(gameState.isValidPosition(irr::core::vector3df(9, 9, 0)));
    
    // Invalid positions
    cr_assert_not(gameState.isValidPosition(irr::core::vector3df(-1, 0, 0)));
    cr_assert_not(gameState.isValidPosition(irr::core::vector3df(0, -1, 0)));
    cr_assert_not(gameState.isValidPosition(irr::core::vector3df(10, 0, 0)));
    cr_assert_not(gameState.isValidPosition(irr::core::vector3df(0, 10, 0)));
}

Test(GameState, add_player) {
    GameState gameState;
    gameState.setMapSize(10, 10);
    
    gameState.addPlayer(1, irr::core::vector3df(5, 5, 0), Direction::NORTH, 1, "team1");
    
    const PlayerEntity* player = gameState.getPlayer(1);
    cr_assert_not_null(player);
    cr_assert_eq(player->getId(), 1);
    cr_assert_eq(player->getLevel(), 1);
    cr_assert_str_eq(player->getTeam().c_str(), "team1");
}

Test(GameState, add_multiple_players) {
    GameState gameState;
    gameState.setMapSize(10, 10);
    
    gameState.addPlayer(1, irr::core::vector3df(1, 1, 0), Direction::NORTH, 1, "team1");
    gameState.addPlayer(2, irr::core::vector3df(2, 2, 0), Direction::SOUTH, 2, "team2");
    gameState.addPlayer(3, irr::core::vector3df(3, 3, 0), Direction::EAST, 3, "team1");
    
    cr_assert_not_null(gameState.getPlayer(1));
    cr_assert_not_null(gameState.getPlayer(2));
    cr_assert_not_null(gameState.getPlayer(3));
    
    auto allPlayers = gameState.getAllPlayers();
    cr_assert_eq(allPlayers.size(), 3);
}

Test(GameState, update_player_position) {
    GameState gameState;
    gameState.setMapSize(10, 10);
    
    gameState.addPlayer(1, irr::core::vector3df(5, 5, 0), Direction::NORTH, 1, "team1");
    gameState.updatePlayerPosition(1, irr::core::vector3df(6, 7, 0), Direction::EAST);
    
    const PlayerEntity* player = gameState.getPlayer(1);
    cr_assert_not_null(player);
    cr_assert_eq(player->getPosition().X, 6);
    cr_assert_eq(player->getPosition().Y, 7);
    cr_assert_eq(player->getDirection(), Direction::EAST);
}

Test(GameState, update_player_level) {
    GameState gameState;
    gameState.setMapSize(10, 10);
    
    gameState.addPlayer(1, irr::core::vector3df(5, 5, 0), Direction::NORTH, 1, "team1");
    gameState.updatePlayerLevel(1, 5);
    
    const PlayerEntity* player = gameState.getPlayer(1);
    cr_assert_not_null(player);
    cr_assert_eq(player->getLevel(), 5);
}

Test(GameState, remove_player) {
    GameState gameState;
    gameState.setMapSize(10, 10);
    
    gameState.addPlayer(1, irr::core::vector3df(5, 5, 0), Direction::NORTH, 1, "team1");
    cr_assert_not_null(gameState.getPlayer(1));
    
    gameState.removePlayer(1);
    cr_assert_null(gameState.getPlayer(1));
}

Test(GameState, get_nonexistent_player) {
    GameState gameState;
    
    cr_assert_null(gameState.getPlayer(999));
}

Test(GameState, update_tile) {
    GameState gameState;
    gameState.setMapSize(10, 10);
    
    Inventory resources;
    resources.addItem("food", 5);
    resources.addItem("linemate", 3);
    
    gameState.updateTile(irr::core::vector3df(5, 5, 0), resources);
    
    const TileEntity* tile = gameState.getTile(irr::core::vector3df(5, 5, 0));
    cr_assert_not_null(tile);
}

Test(GameState, time_unit) {
    GameState gameState;
    
    gameState.setTimeUnit(200);
    cr_assert_eq(gameState.getTimeUnit(), 200);
    
    gameState.setTimeUnit(50);
    cr_assert_eq(gameState.getTimeUnit(), 50);
}

Test(GameState, teams_management) {
    GameState gameState;
    
    gameState.addTeam("team1");
    gameState.addTeam("team2");
    gameState.addTeam("team1"); // Duplicate, should not be added again
    
    auto teams = gameState.getAllTeamNames();
    cr_assert_eq(teams.size(), 2);
    
    bool hasTeam1 = false, hasTeam2 = false;
    for (const auto& team : teams) {
        if (team == "team1") hasTeam1 = true;
        if (team == "team2") hasTeam2 = true;
    }
    cr_assert(hasTeam1);
    cr_assert(hasTeam2);
}

Test(GameState, eggs_management) {
    GameState gameState;
    gameState.setMapSize(10, 10);
    
    // Add egg
    gameState.addEgg(1, irr::core::vector3df(5, 5, 0), "team1");
    
    const Egg* egg = gameState.getEgg(1);
    cr_assert_not_null(egg);
    cr_assert_str_eq(gameState.getEggTeam(1).c_str(), "team1");
    
    // Test hatching
    gameState.setEggHatching(1, true);
    cr_assert_str_eq(egg->getType().c_str(), "hatching");
    
    gameState.setEggHatching(1, false);
    cr_assert_str_eq(egg->getType().c_str(), "egg");
    
    // Remove egg
    gameState.removeEgg(1);
    cr_assert_null(gameState.getEgg(1));
    cr_assert_str_eq(gameState.getEggTeam(1).c_str(), "");
}

Test(GameState, spawn_from_egg) {
    GameState gameState;
    gameState.setMapSize(10, 10);
    
    // Add egg
    gameState.addEgg(1, irr::core::vector3df(5, 5, 0), "team1");
    cr_assert_not_null(gameState.getEgg(1));
    
    // Spawn player from egg
    gameState.spawnFromEgg(1, 100, irr::core::vector3df(5, 5, 0), Direction::NORTH, 1, "team1");
    
    // Egg should be removed
    cr_assert_null(gameState.getEgg(1));
    
    // Player should be created
    const PlayerEntity* player = gameState.getPlayer(100);
    cr_assert_not_null(player);
    cr_assert_eq(player->getId(), 100);
    cr_assert_str_eq(player->getTeam().c_str(), "team1");
}

Test(GameState, incantations) {
    GameState gameState;
    gameState.setMapSize(10, 10);
    
    // Add players for incantation
    gameState.addPlayer(1, irr::core::vector3df(5, 5, 0), Direction::NORTH, 1, "team1");
    gameState.addPlayer(2, irr::core::vector3df(5, 5, 0), Direction::NORTH, 1, "team1");
    
    std::vector<int> playerIds = {1, 2};
    
    // Start incantation
    gameState.startIncantation(irr::core::vector3df(5, 5, 0), 1, playerIds);
    
    auto incantations = gameState.getAllIncantations();
    cr_assert_eq(incantations.size(), 1);
    cr_assert(incantations[0].active);
    cr_assert_eq(incantations[0].level, 1);
    cr_assert_eq(incantations[0].playerIds.size(), 2);
    
    // End incantation successfully
    gameState.endIncantation(irr::core::vector3df(5, 5, 0), true);
    
    // Players should be leveled up
    cr_assert_eq(gameState.getPlayer(1)->getLevel(), 2);
    cr_assert_eq(gameState.getPlayer(2)->getLevel(), 2);
}

Test(GameState, clear_state) {
    GameState gameState;
    gameState.setMapSize(10, 10);
    
    // Add some data
    gameState.addPlayer(1, irr::core::vector3df(5, 5, 0), Direction::NORTH, 1, "team1");
    gameState.addEgg(1, irr::core::vector3df(3, 3, 0), "team1");
    gameState.addTeam("team1");
    
    cr_assert_not(gameState.isEmpty());
    
    // Clear everything
    gameState.clear();
    
    cr_assert(gameState.isEmpty());
    cr_assert_null(gameState.getPlayer(1));
    cr_assert_null(gameState.getEgg(1));
}

Test(GameState, get_all_eggs) {
    GameState gameState;
    gameState.setMapSize(10, 10);
    
    gameState.addEgg(1, irr::core::vector3df(1, 1, 0), "team1");
    gameState.addEgg(2, irr::core::vector3df(2, 2, 0), "team2");
    gameState.addEgg(3, irr::core::vector3df(3, 3, 0), "team1");
    
    auto allEggs = gameState.getAllEggs();
    cr_assert_eq(allEggs.size(), 3);
}

Test(GameState, edge_cases) {
    GameState gameState;
    
    // Test operations on empty game state
    gameState.updatePlayerPosition(999, irr::core::vector3df(0, 0, 0), Direction::NORTH);
    gameState.updatePlayerLevel(999, 5);
    gameState.removePlayer(999);
    
    // These should not crash
    cr_assert_null(gameState.getPlayer(999));
    cr_assert_null(gameState.getTile(irr::core::vector3df(0, 0, 0)));
}
