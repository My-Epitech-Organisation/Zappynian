/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Comprehensive unit tests for PlayerEntity class
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "../../gui/Entities/PlayerEntity.hpp"

TestSuite(PlayerEntity, .timeout = 10);

Test(PlayerEntity, construction) {
    irr::core::vector3df position(5, 0, 3);
    irr::core::vector3df scale(1, 1, 1);
    std::vector<irr::io::path> textures = {"texture1.png", "texture2.png"};
    irr::io::path modelPath("player.3ds");
    
    PlayerEntity player(1, position, scale, Direction::NORTH, "team1", textures, 1, modelPath);
    
    cr_assert_eq(player.getId(), 1);
    cr_assert_eq(player.getPosition().X, 5);
    cr_assert_eq(player.getPosition().Y, 0);
    cr_assert_eq(player.getPosition().Z, 3);
    cr_assert_eq(player.getDirection(), Direction::NORTH);
    cr_assert_str_eq(player.getTeam().c_str(), "team1");
    cr_assert_eq(player.getLevel(), 1);
    cr_assert_str_eq(player.getName().c_str(), "Player_1");
}

Test(PlayerEntity, different_directions) {
    irr::core::vector3df position(0, 0, 0);
    irr::core::vector3df scale(1, 1, 1);
    std::vector<irr::io::path> textures;
    irr::io::path modelPath("player.3ds");
    
    // Test all directions
    PlayerEntity playerNorth(1, position, scale, Direction::NORTH, "team1", textures, 1, modelPath);
    PlayerEntity playerSouth(2, position, scale, Direction::SOUTH, "team1", textures, 1, modelPath);
    PlayerEntity playerEast(3, position, scale, Direction::EAST, "team1", textures, 1, modelPath);
    PlayerEntity playerWest(4, position, scale, Direction::WEST, "team1", textures, 1, modelPath);
    
    cr_assert_eq(playerNorth.getDirection(), Direction::NORTH);
    cr_assert_eq(playerSouth.getDirection(), Direction::SOUTH);
    cr_assert_eq(playerEast.getDirection(), Direction::EAST);
    cr_assert_eq(playerWest.getDirection(), Direction::WEST);
}

Test(PlayerEntity, different_levels) {
    irr::core::vector3df position(0, 0, 0);
    irr::core::vector3df scale(1, 1, 1);
    std::vector<irr::io::path> textures;
    irr::io::path modelPath("player.3ds");
    
    // Test different levels (1-8 in Zappy)
    for (int level = 1; level <= 8; ++level) {
        PlayerEntity player(level + 100, position, scale, Direction::NORTH, "team1", textures, level, modelPath);
        cr_assert_eq(player.getLevel(), level);
    }
}

Test(PlayerEntity, different_teams) {
    irr::core::vector3df position(0, 0, 0);
    irr::core::vector3df scale(1, 1, 1);
    std::vector<irr::io::path> textures;
    irr::io::path modelPath("player.3ds");
    
    PlayerEntity player1(1, position, scale, Direction::NORTH, "team_red", textures, 1, modelPath);
    PlayerEntity player2(2, position, scale, Direction::NORTH, "team_blue", textures, 1, modelPath);
    PlayerEntity player3(3, position, scale, Direction::NORTH, "team_green", textures, 1, modelPath);
    
    cr_assert_str_eq(player1.getTeam().c_str(), "team_red");
    cr_assert_str_eq(player2.getTeam().c_str(), "team_blue");
    cr_assert_str_eq(player3.getTeam().c_str(), "team_green");
}

Test(PlayerEntity, unique_names) {
    irr::core::vector3df position(0, 0, 0);
    irr::core::vector3df scale(1, 1, 1);
    std::vector<irr::io::path> textures;
    irr::io::path modelPath("player.3ds");
    
    PlayerEntity player1(1, position, scale, Direction::NORTH, "team1", textures, 1, modelPath);
    PlayerEntity player2(2, position, scale, Direction::NORTH, "team1", textures, 1, modelPath);
    PlayerEntity player100(100, position, scale, Direction::NORTH, "team1", textures, 1, modelPath);
    
    cr_assert_str_eq(player1.getName().c_str(), "Player_1");
    cr_assert_str_eq(player2.getName().c_str(), "Player_2");
    cr_assert_str_eq(player100.getName().c_str(), "Player_100");
}

Test(PlayerEntity, position_and_scale) {
    std::vector<irr::io::path> textures;
    irr::io::path modelPath("player.3ds");
    
    // Test different positions
    irr::core::vector3df pos1(10, 5, 15);
    irr::core::vector3df pos2(-5, 0, 20);
    irr::core::vector3df scale1(2, 2, 2);
    irr::core::vector3df scale2(0.5, 0.5, 0.5);
    
    PlayerEntity player1(1, pos1, scale1, Direction::NORTH, "team1", textures, 1, modelPath);
    PlayerEntity player2(2, pos2, scale2, Direction::SOUTH, "team2", textures, 2, modelPath);
    
    cr_assert_eq(player1.getPosition().X, 10);
    cr_assert_eq(player1.getPosition().Y, 5);
    cr_assert_eq(player1.getPosition().Z, 15);
    cr_assert_eq(player1.getScale().X, 2);
    cr_assert_eq(player1.getScale().Y, 2);
    cr_assert_eq(player1.getScale().Z, 2);
    
    cr_assert_eq(player2.getPosition().X, -5);
    cr_assert_eq(player2.getPosition().Y, 0);
    cr_assert_eq(player2.getPosition().Z, 20);
    cr_assert_eq(player2.getScale().X, 0.5f);
    cr_assert_eq(player2.getScale().Y, 0.5f);
    cr_assert_eq(player2.getScale().Z, 0.5f);
}

Test(PlayerEntity, inheritance_from_aentity) {
    irr::core::vector3df position(5, 0, 3);
    irr::core::vector3df scale(1, 1, 1);
    std::vector<irr::io::path> textures;
    irr::io::path modelPath("player.3ds");
    
    PlayerEntity player(42, position, scale, Direction::EAST, "test_team", textures, 3, modelPath);
    
    // Test inherited AEntity methods
    cr_assert_eq(player.getId(), 42);
    cr_assert_str_eq(player.getName().c_str(), "Player_42");
    cr_assert_eq(player.getLevel(), 3);
    
    // Test position and scale inheritance
    auto playerPos = player.getPosition();
    auto playerScale = player.getScale();
    cr_assert_eq(playerPos.X, 5);
    cr_assert_eq(playerPos.Y, 0);
    cr_assert_eq(playerPos.Z, 3);
    cr_assert_eq(playerScale.X, 1);
    cr_assert_eq(playerScale.Y, 1);
    cr_assert_eq(playerScale.Z, 1);
}

Test(PlayerEntity, set_position_after_creation) {
    irr::core::vector3df position(0, 0, 0);
    irr::core::vector3df scale(1, 1, 1);
    std::vector<irr::io::path> textures;
    irr::io::path modelPath("player.3ds");
    
    PlayerEntity player(1, position, scale, Direction::NORTH, "team1", textures, 1, modelPath);
    
    // Change position
    irr::core::vector3df newPosition(10, 5, 8);
    player.setPosition(newPosition);
    
    auto updatedPos = player.getPosition();
    cr_assert_eq(updatedPos.X, 10);
    cr_assert_eq(updatedPos.Y, 5);
    cr_assert_eq(updatedPos.Z, 8);
}

Test(PlayerEntity, set_scale_after_creation) {
    irr::core::vector3df position(0, 0, 0);
    irr::core::vector3df scale(1, 1, 1);
    std::vector<irr::io::path> textures;
    irr::io::path modelPath("player.3ds");
    
    PlayerEntity player(1, position, scale, Direction::NORTH, "team1", textures, 1, modelPath);
    
    // Change scale
    irr::core::vector3df newScale(3, 3, 3);
    player.setScale(newScale);
    
    auto updatedScale = player.getScale();
    cr_assert_eq(updatedScale.X, 3);
    cr_assert_eq(updatedScale.Y, 3);
    cr_assert_eq(updatedScale.Z, 3);
}

Test(PlayerEntity, set_level_after_creation) {
    irr::core::vector3df position(0, 0, 0);
    irr::core::vector3df scale(1, 1, 1);
    std::vector<irr::io::path> textures;
    irr::io::path modelPath("player.3ds");
    
    PlayerEntity player(1, position, scale, Direction::NORTH, "team1", textures, 1, modelPath);
    
    // Level up the player
    player.setLevel(5);
    cr_assert_eq(player.getLevel(), 5);
    
    player.setLevel(8);
    cr_assert_eq(player.getLevel(), 8);
}

Test(PlayerEntity, inventory_access) {
    irr::core::vector3df position(0, 0, 0);
    irr::core::vector3df scale(1, 1, 1);
    std::vector<irr::io::path> textures;
    irr::io::path modelPath("player.3ds");
    
    PlayerEntity player(1, position, scale, Direction::NORTH, "team1", textures, 1, modelPath);
    
    // Test inventory access (inherited from AEntity)
    auto& inventory = player.getInventory();
    inventory.addItem("food", 10);
    inventory.addItem("linemate", 5);
    
    cr_assert_eq(inventory.getItemQuantity("food"), 10);
    cr_assert_eq(inventory.getItemQuantity("linemate"), 5);
    
    // Test const inventory access
    const PlayerEntity& constPlayer = player;
    const auto& constInventory = constPlayer.getInventory();
    cr_assert_eq(constInventory.getItemQuantity("food"), 10);
    cr_assert_eq(constInventory.getItemQuantity("linemate"), 5);
}

Test(PlayerEntity, empty_team_name) {
    irr::core::vector3df position(0, 0, 0);
    irr::core::vector3df scale(1, 1, 1);
    std::vector<irr::io::path> textures;
    irr::io::path modelPath("player.3ds");
    
    PlayerEntity player(1, position, scale, Direction::NORTH, "", textures, 1, modelPath);
    
    cr_assert_str_eq(player.getTeam().c_str(), "");
}

Test(PlayerEntity, zero_level) {
    irr::core::vector3df position(0, 0, 0);
    irr::core::vector3df scale(1, 1, 1);
    std::vector<irr::io::path> textures;
    irr::io::path modelPath("player.3ds");
    
    PlayerEntity player(1, position, scale, Direction::NORTH, "team1", textures, 0, modelPath);
    
    cr_assert_eq(player.getLevel(), 0);
}

Test(PlayerEntity, large_coordinates) {
    irr::core::vector3df position(1000, 2000, 3000);
    irr::core::vector3df scale(10, 20, 30);
    std::vector<irr::io::path> textures;
    irr::io::path modelPath("player.3ds");
    
    PlayerEntity player(999999, position, scale, Direction::WEST, "large_team", textures, 8, modelPath);
    
    cr_assert_eq(player.getId(), 999999);
    cr_assert_eq(player.getPosition().X, 1000);
    cr_assert_eq(player.getPosition().Y, 2000);
    cr_assert_eq(player.getPosition().Z, 3000);
    cr_assert_eq(player.getScale().X, 10);
    cr_assert_eq(player.getScale().Y, 20);
    cr_assert_eq(player.getScale().Z, 30);
    cr_assert_eq(player.getDirection(), Direction::WEST);
    cr_assert_eq(player.getLevel(), 8);
}
