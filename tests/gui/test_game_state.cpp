/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** Tests for GameState class
*/

#include <criterion/criterion.h>
#include "Core/GameState.hpp"
#include "Entities/Direction.hpp"

TestSuite(game_state);

Test(game_state, default_initialization) {
    Zappy::GameState gameState;
    
    // Map should be empty initially
    irr::core::vector3df mapSize = gameState.getMapSize();
    cr_assert_float_eq(mapSize.X, 0.0f, 0.001f);
    cr_assert_float_eq(mapSize.Y, 0.0f, 0.001f);
    cr_assert_float_eq(mapSize.Z, 0.0f, 0.001f);
    
    // Default time unit
    cr_assert_eq(gameState.getTimeUnit(), 100);
    
    // Should be empty
    cr_assert(gameState.isEmpty());
}

Test(game_state, set_map_size) {
    Zappy::GameState gameState;
    
    gameState.setMapSize(10, 15);
    
    irr::core::vector3df mapSize = gameState.getMapSize();
    cr_assert_float_eq(mapSize.X, 10.0f, 0.001f);
    cr_assert_float_eq(mapSize.Y, 15.0f, 0.001f);
    cr_assert_float_eq(mapSize.Z, 0.0f, 0.001f);
    
    cr_assert(!gameState.isEmpty());
}

Test(game_state, time_unit_operations) {
    Zappy::GameState gameState;
    
    gameState.setTimeUnit(200);
    cr_assert_eq(gameState.getTimeUnit(), 200);
    
    gameState.setTimeUnit(50);
    cr_assert_eq(gameState.getTimeUnit(), 50);
}

Test(game_state, valid_position_check) {
    Zappy::GameState gameState;
    gameState.setMapSize(5, 3);
    
    // Valid positions
    cr_assert(gameState.isValidPosition(irr::core::vector3df(0, 0, 0)));
    cr_assert(gameState.isValidPosition(irr::core::vector3df(4, 2, 0)));
    cr_assert(gameState.isValidPosition(irr::core::vector3df(2, 1, 0)));
    
    // Invalid positions
    cr_assert(!gameState.isValidPosition(irr::core::vector3df(-1, 0, 0)));
    cr_assert(!gameState.isValidPosition(irr::core::vector3df(5, 0, 0)));
    cr_assert(!gameState.isValidPosition(irr::core::vector3df(0, -1, 0)));
    cr_assert(!gameState.isValidPosition(irr::core::vector3df(0, 3, 0)));
}

Test(game_state, add_player) {
    Zappy::GameState gameState;
    gameState.setMapSize(5, 5);
    
    irr::core::vector3df pos(2, 3, 0);
    gameState.addPlayer(1, pos, Direction::NORTH, 2, "Red");
    
    const PlayerEntity* player = gameState.getPlayer(1);
    cr_assert_not_null(player);
    cr_assert_eq(player->getId(), 1);
    cr_assert_eq(player->getDirection(), Direction::NORTH);
    cr_assert_str_eq(player->getTeam().c_str(), "Red");
}

Test(game_state, get_nonexistent_player) {
    Zappy::GameState gameState;
    
    const PlayerEntity* player = gameState.getPlayer(999);
    cr_assert_null(player);
}

Test(game_state, update_player_position) {
    Zappy::GameState gameState;
    gameState.setMapSize(5, 5);
    
    irr::core::vector3df pos1(1, 1, 0);
    irr::core::vector3df pos2(3, 4, 0);
    
    gameState.addPlayer(1, pos1, Direction::NORTH, 1, "Blue");
    
    const PlayerEntity* player = gameState.getPlayer(1);
    irr::core::vector3df retrievedPos = player->getPosition();
    cr_assert_float_eq(retrievedPos.X, 1.0f, 0.001f);
    cr_assert_float_eq(retrievedPos.Y, 1.0f, 0.001f);
    
    gameState.updatePlayerPosition(1, pos2, Direction::EAST);
    
    player = gameState.getPlayer(1);
    retrievedPos = player->getPosition();
    cr_assert_float_eq(retrievedPos.X, 3.0f, 0.001f);
    cr_assert_float_eq(retrievedPos.Y, 4.0f, 0.001f);
    cr_assert_eq(player->getDirection(), Direction::EAST);
}

Test(game_state, update_player_level) {
    Zappy::GameState gameState;
    gameState.setMapSize(5, 5);
    
    gameState.addPlayer(1, irr::core::vector3df(0, 0, 0), Direction::NORTH, 1, "Green");
    
    const PlayerEntity* player = gameState.getPlayer(1);
    cr_assert_eq(player->getLevel(), 1);
    
    gameState.updatePlayerLevel(1, 5);
    
    player = gameState.getPlayer(1);
    cr_assert_eq(player->getLevel(), 5);
}

Test(game_state, update_nonexistent_player) {
    Zappy::GameState gameState;
    gameState.setMapSize(5, 5);
    
    // These should not crash
    gameState.updatePlayerPosition(999, irr::core::vector3df(0, 0, 0), Direction::NORTH);
    gameState.updatePlayerLevel(999, 3);
    
    // Player should still not exist
    const PlayerEntity* player = gameState.getPlayer(999);
    cr_assert_null(player);
}

Test(game_state, remove_player) {
    Zappy::GameState gameState;
    gameState.setMapSize(5, 5);
    
    gameState.addPlayer(1, irr::core::vector3df(0, 0, 0), Direction::NORTH, 1, "Red");
    
    const PlayerEntity* player = gameState.getPlayer(1);
    cr_assert_not_null(player);
    
    gameState.removePlayer(1);
    
    player = gameState.getPlayer(1);
    cr_assert_null(player);
}

Test(game_state, get_all_players) {
    Zappy::GameState gameState;
    gameState.setMapSize(5, 5);
    
    gameState.addPlayer(1, irr::core::vector3df(0, 0, 0), Direction::NORTH, 1, "Red");
    gameState.addPlayer(2, irr::core::vector3df(1, 1, 0), Direction::EAST, 2, "Blue");
    gameState.addPlayer(3, irr::core::vector3df(2, 2, 0), Direction::SOUTH, 3, "Green");
    
    std::vector<const PlayerEntity*> players = gameState.getAllPlayers();
    cr_assert_eq(players.size(), 3);
    
    // Verify all players are present
    bool found1 = false, found2 = false, found3 = false;
    for (const auto* player : players) {
        if (player->getId() == 1) found1 = true;
        if (player->getId() == 2) found2 = true;
        if (player->getId() == 3) found3 = true;
    }
    cr_assert(found1);
    cr_assert(found2);
    cr_assert(found3);
}

Test(game_state, add_team) {
    Zappy::GameState gameState;
    
    gameState.addTeam("Red");
    gameState.addTeam("Blue");
    gameState.addTeam("Green");
    
    std::vector<std::string> teams = gameState.getAllTeamNames();
    cr_assert_eq(teams.size(), 3);
    
    // Check if all teams are present
    bool foundRed = false, foundBlue = false, foundGreen = false;
    for (const auto& team : teams) {
        if (team == "Red") foundRed = true;
        if (team == "Blue") foundBlue = true;
        if (team == "Green") foundGreen = true;
    }
    cr_assert(foundRed);
    cr_assert(foundBlue);
    cr_assert(foundGreen);
}

Test(game_state, add_duplicate_team) {
    Zappy::GameState gameState;
    
    gameState.addTeam("Red");
    gameState.addTeam("Red");  // Duplicate
    gameState.addTeam("Blue");
    gameState.addTeam("Red");  // Another duplicate
    
    std::vector<std::string> teams = gameState.getAllTeamNames();
    cr_assert_eq(teams.size(), 2);  // Should only have "Red" and "Blue"
}

Test(game_state, add_egg) {
    Zappy::GameState gameState;
    
    irr::core::vector3df pos(1, 2, 0);
    gameState.addEgg(10, pos, "Red");
    
    const Egg* egg = gameState.getEgg(10);
    cr_assert_not_null(egg);
    cr_assert_eq(egg->getId(), 10);
    
    std::string team = gameState.getEggTeam(10);
    cr_assert_str_eq(team.c_str(), "Red");
}

Test(game_state, get_nonexistent_egg) {
    Zappy::GameState gameState;
    
    const Egg* egg = gameState.getEgg(999);
    cr_assert_null(egg);
    
    std::string team = gameState.getEggTeam(999);
    cr_assert_str_eq(team.c_str(), "");
}

Test(game_state, set_egg_hatching) {
    Zappy::GameState gameState;
    
    gameState.addEgg(5, irr::core::vector3df(0, 0, 0), "Blue");
    
    const Egg* egg = gameState.getEgg(5);
    cr_assert_str_eq(egg->getType().c_str(), "egg");
    
    gameState.setEggHatching(5, true);
    egg = gameState.getEgg(5);
    cr_assert_str_eq(egg->getType().c_str(), "hatching");
    
    gameState.setEggHatching(5, false);
    egg = gameState.getEgg(5);
    cr_assert_str_eq(egg->getType().c_str(), "egg");
}

Test(game_state, remove_egg) {
    Zappy::GameState gameState;
    
    gameState.addEgg(7, irr::core::vector3df(0, 0, 0), "Green");
    
    const Egg* egg = gameState.getEgg(7);
    cr_assert_not_null(egg);
    
    gameState.removeEgg(7);
    
    egg = gameState.getEgg(7);
    cr_assert_null(egg);
    
    std::string team = gameState.getEggTeam(7);
    cr_assert_str_eq(team.c_str(), "");
}

Test(game_state, get_all_eggs) {
    Zappy::GameState gameState;
    
    gameState.addEgg(1, irr::core::vector3df(0, 0, 0), "Red");
    gameState.addEgg(2, irr::core::vector3df(1, 1, 0), "Blue");
    
    std::vector<const Egg*> eggs = gameState.getAllEggs();
    cr_assert_eq(eggs.size(), 2);
    
    // Verify all eggs are present
    bool found1 = false, found2 = false;
    for (const auto* egg : eggs) {
        if (egg->getId() == 1) found1 = true;
        if (egg->getId() == 2) found2 = true;
    }
    cr_assert(found1);
    cr_assert(found2);
}

Test(game_state, clear_all_data) {
    Zappy::GameState gameState;
    gameState.setMapSize(5, 5);
    
    gameState.addPlayer(1, irr::core::vector3df(0, 0, 0), Direction::NORTH, 1, "Red");
    gameState.addEgg(1, irr::core::vector3df(0, 0, 0), "Red");
    gameState.addTeam("Red");
    
    cr_assert(!gameState.isEmpty());
    
    gameState.clear();
    
    cr_assert(gameState.isEmpty());
    cr_assert_null(gameState.getPlayer(1));
    cr_assert_null(gameState.getEgg(1));
}

Test(game_state, spawn_from_egg) {
    Zappy::GameState gameState;
    gameState.setMapSize(5, 5);
    
    // Add an egg
    gameState.addEgg(10, irr::core::vector3df(2, 3, 0), "Blue");
    
    const Egg* egg = gameState.getEgg(10);
    cr_assert_not_null(egg);
    
    // Spawn player from egg
    gameState.spawnFromEgg(10, 50, irr::core::vector3df(2, 3, 0), Direction::EAST, 1, "Blue");
    
    // Egg should be removed
    egg = gameState.getEgg(10);
    cr_assert_null(egg);
    
    // Player should exist
    const PlayerEntity* player = gameState.getPlayer(50);
    cr_assert_not_null(player);
    cr_assert_eq(player->getDirection(), Direction::EAST);
    cr_assert_str_eq(player->getTeam().c_str(), "Blue");
}

Test(game_state, update_tile_resources) {
    Zappy::GameState gameState;
    gameState.setMapSize(3, 3);
    
    // Create a tile with resources
    Inventory resources;
    resources.addItem("food", 5);
    resources.addItem("linemate", 2);
    
    irr::core::vector3df pos(1, 1, 0);
    gameState.updateTile(pos, resources);
    
    const TileEntity* tile = gameState.getTile(pos);
    cr_assert_not_null(tile);
    cr_assert_eq(tile->getInventory().getItemQuantity("food"), 5);
    cr_assert_eq(tile->getInventory().getItemQuantity("linemate"), 2);
}

Test(game_state, get_tile_invalid_position) {
    Zappy::GameState gameState;
    gameState.setMapSize(3, 3);
    
    // Test with invalid position
    irr::core::vector3df invalidPos(-1, -1, 0);
    const TileEntity* tile = gameState.getTile(invalidPos);
    cr_assert_null(tile);
    
    // Test with out-of-bounds position
    irr::core::vector3df outOfBounds(10, 10, 0);
    tile = gameState.getTile(outOfBounds);
    cr_assert_null(tile);
}

Test(game_state, get_all_tiles) {
    Zappy::GameState gameState;
    gameState.setMapSize(2, 2);
    
    std::vector<const TileEntity*> tiles = gameState.getAllTiles();
    cr_assert_eq(tiles.size(), 4); // 2x2 = 4 tiles
    
    // Verify all tiles are valid
    for (const TileEntity* tile : tiles) {
        cr_assert_not_null(tile);
    }
}

Test(game_state, add_player_invalid_position) {
    Zappy::GameState gameState;
    gameState.setMapSize(3, 3);
    
    // Try to add player at invalid position
    irr::core::vector3df invalidPos(-1, 5, 0);
    gameState.addPlayer(1, invalidPos, Direction::NORTH, 1, "Red");
    
    // Player should not be added
    const PlayerEntity* player = gameState.getPlayer(1);
    cr_assert_null(player);
}

Test(game_state, update_player_inventory) {
    Zappy::GameState gameState;
    gameState.setMapSize(3, 3);
    
    // Add a player
    irr::core::vector3df pos(1, 1, 0);
    gameState.addPlayer(1, pos, Direction::NORTH, 1, "Red");
    
    // Update player inventory
    Inventory inventory;
    inventory.addItem("food", 10);
    inventory.addItem("linemate", 3);
    
    gameState.updatePlayerInventory(1, pos, inventory);
    
    const PlayerEntity* player = gameState.getPlayer(1);
    cr_assert_not_null(player);
    cr_assert_eq(player->getInventory().getItemQuantity("food"), 10);
    cr_assert_eq(player->getInventory().getItemQuantity("linemate"), 3);
}

Test(game_state, update_nonexistent_player_inventory) {
    Zappy::GameState gameState;
    gameState.setMapSize(3, 3);
    
    // Try to update inventory of nonexistent player
    Inventory inventory;
    inventory.addItem("food", 5);
    
    irr::core::vector3df pos(1, 1, 0);
    gameState.updatePlayerInventory(999, pos, inventory);
    
    // Should not crash, just log error
    cr_assert(true); // Test passes if we get here
}

Test(game_state, remove_nonexistent_player) {
    Zappy::GameState gameState;
    gameState.setMapSize(3, 3);
    
    // Try to remove player that doesn't exist
    gameState.removePlayer(999);
    
    // Should not crash, just log error
    cr_assert(true); // Test passes if we get here
}

Test(game_state, get_players_on_tile) {
    Zappy::GameState gameState;
    gameState.setMapSize(3, 3);
    
    irr::core::vector3df pos(1, 1, 0);
    
    // Add multiple players on the same tile
    gameState.addPlayer(1, pos, Direction::NORTH, 1, "Red");
    gameState.addPlayer(2, pos, Direction::SOUTH, 2, "Blue");
    
    // Add a player on a different tile
    irr::core::vector3df otherPos(2, 2, 0);
    gameState.addPlayer(3, otherPos, Direction::EAST, 1, "Green");
    
    std::vector<const PlayerEntity*> playersOnTile = gameState.getPlayersOnTile(pos);
    cr_assert_eq(playersOnTile.size(), 2);
    
    // Verify the players are the correct ones
    bool foundPlayer1 = false, foundPlayer2 = false;
    for (const PlayerEntity* player : playersOnTile) {
        if (player->getId() == 1) foundPlayer1 = true;
        if (player->getId() == 2) foundPlayer2 = true;
    }
    cr_assert(foundPlayer1);
    cr_assert(foundPlayer2);
    
    // Check other tile has only one player
    std::vector<const PlayerEntity*> playersOnOtherTile = gameState.getPlayersOnTile(otherPos);
    cr_assert_eq(playersOnOtherTile.size(), 1);
    cr_assert_eq(playersOnOtherTile[0]->getId(), 3);
}

Test(game_state, get_players_on_empty_tile) {
    Zappy::GameState gameState;
    gameState.setMapSize(3, 3);
    
    irr::core::vector3df pos(1, 1, 0);
    std::vector<const PlayerEntity*> playersOnTile = gameState.getPlayersOnTile(pos);
    cr_assert_eq(playersOnTile.size(), 0);
}
