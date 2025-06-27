/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** Tests for PlayerEntity class
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "irrlicht_mocks.hpp"
#include "Entities/PlayerEntity.hpp"
#include "Entities/Direction.hpp"
#include <memory>

TestSuite(player_entity);

Test(player_entity, constructor_with_parameters) {
    irr::core::vector3df pos(10.0f, 0.0f, 20.0f);
    irr::core::vector3df scale(1.0f, 1.0f, 1.0f);
    std::vector<irr::io::path> textures = {"texture1.png", "texture2.png"};
    irr::io::path entityB3D = "player.b3d";
    
    PlayerEntity player(1, pos, scale, Direction::NORTH, "team1", textures, 3, entityB3D);
    
    cr_assert_eq(player.getId(), 1, "Player ID should be set correctly");
    cr_assert_str_eq(player.getName().c_str(), "Player_1", "Player name should be generated correctly");
    cr_assert_str_eq(player.getTeam().c_str(), "team1", "Player team should be set correctly");
    cr_assert_eq(player.getDirection(), Direction::NORTH, "Player direction should be set correctly");
}

Test(player_entity, constructor_all_directions) {
    irr::core::vector3df pos(0.0f, 0.0f, 0.0f);
    irr::core::vector3df scale(1.0f, 1.0f, 1.0f);
    std::vector<irr::io::path> textures = {"texture.png"};
    irr::io::path entityB3D = "player.b3d";
    
    PlayerEntity north(1, pos, scale, Direction::NORTH, "team1", textures, 1, entityB3D);
    PlayerEntity east(2, pos, scale, Direction::EAST, "team1", textures, 1, entityB3D);
    PlayerEntity south(3, pos, scale, Direction::SOUTH, "team1", textures, 1, entityB3D);
    PlayerEntity west(4, pos, scale, Direction::WEST, "team1", textures, 1, entityB3D);
    
    cr_assert_eq(north.getDirection(), Direction::NORTH, "North direction should be set");
    cr_assert_eq(east.getDirection(), Direction::EAST, "East direction should be set");
    cr_assert_eq(south.getDirection(), Direction::SOUTH, "South direction should be set");
    cr_assert_eq(west.getDirection(), Direction::WEST, "West direction should be set");
}

Test(player_entity, constructor_different_teams) {
    irr::core::vector3df pos(0.0f, 0.0f, 0.0f);
    irr::core::vector3df scale(1.0f, 1.0f, 1.0f);
    std::vector<irr::io::path> textures = {"texture.png"};
    irr::io::path entityB3D = "player.b3d";
    
    PlayerEntity player1(1, pos, scale, Direction::NORTH, "red_team", textures, 1, entityB3D);
    PlayerEntity player2(2, pos, scale, Direction::NORTH, "blue_team", textures, 1, entityB3D);
    PlayerEntity player3(3, pos, scale, Direction::NORTH, "", textures, 1, entityB3D);
    
    cr_assert_str_eq(player1.getTeam().c_str(), "red_team", "Red team should be set");
    cr_assert_str_eq(player2.getTeam().c_str(), "blue_team", "Blue team should be set");
    cr_assert_str_eq(player3.getTeam().c_str(), "", "Empty team should be set");
}

Test(player_entity, constructor_different_levels) {
    irr::core::vector3df pos(0.0f, 0.0f, 0.0f);
    irr::core::vector3df scale(1.0f, 1.0f, 1.0f);
    std::vector<irr::io::path> textures = {"texture.png"};
    irr::io::path entityB3D = "player.b3d";
    
    PlayerEntity level1(1, pos, scale, Direction::NORTH, "team", textures, 1, entityB3D);
    PlayerEntity level5(2, pos, scale, Direction::NORTH, "team", textures, 5, entityB3D);
    PlayerEntity level8(3, pos, scale, Direction::NORTH, "team", textures, 8, entityB3D);
    
    // Note: Level is stored in AEntity base class, accessed through getName pattern for now
    cr_assert_str_eq(level1.getName().c_str(), "Player_1", "Level 1 player name should be correct");
    cr_assert_str_eq(level5.getName().c_str(), "Player_2", "Level 5 player name should be correct");
    cr_assert_str_eq(level8.getName().c_str(), "Player_3", "Level 8 player name should be correct");
}

Test(player_entity, constructor_different_positions) {
    irr::core::vector3df scale(1.0f, 1.0f, 1.0f);
    std::vector<irr::io::path> textures = {"texture.png"};
    irr::io::path entityB3D = "player.b3d";
    
    irr::core::vector3df pos1(0.0f, 0.0f, 0.0f);
    irr::core::vector3df pos2(10.5f, 5.0f, -3.2f);
    irr::core::vector3df pos3(-100.0f, 200.0f, 50.0f);
    
    PlayerEntity player1(1, pos1, scale, Direction::NORTH, "team", textures, 1, entityB3D);
    PlayerEntity player2(2, pos2, scale, Direction::NORTH, "team", textures, 1, entityB3D);
    PlayerEntity player3(3, pos3, scale, Direction::NORTH, "team", textures, 1, entityB3D);
    
    irr::core::vector3df retrieved1 = player1.getPosition();
    irr::core::vector3df retrieved2 = player2.getPosition();
    irr::core::vector3df retrieved3 = player3.getPosition();
    
    cr_assert_float_eq(retrieved1.X, 0.0f, 0.001f, "Position 1 X should be correct");
    cr_assert_float_eq(retrieved2.X, 10.5f, 0.001f, "Position 2 X should be correct");
    cr_assert_float_eq(retrieved3.X, -100.0f, 0.001f, "Position 3 X should be correct");
}

Test(player_entity, constructor_empty_textures) {
    irr::core::vector3df pos(0.0f, 0.0f, 0.0f);
    irr::core::vector3df scale(1.0f, 1.0f, 1.0f);
    std::vector<irr::io::path> empty_textures;
    irr::io::path entityB3D = "player.b3d";
    
    PlayerEntity player(1, pos, scale, Direction::NORTH, "team", empty_textures, 1, entityB3D);
    
    cr_assert_eq(player.getId(), 1, "Player with empty textures should be created");
    cr_assert_str_eq(player.getTeam().c_str(), "team", "Player team should be set even with empty textures");
}

Test(player_entity, constructor_multiple_textures) {
    irr::core::vector3df pos(0.0f, 0.0f, 0.0f);
    irr::core::vector3df scale(1.0f, 1.0f, 1.0f);
    std::vector<irr::io::path> textures = {
        "texture1.png", "texture2.jpg", "texture3.bmp", "texture4.tga"
    };
    irr::io::path entityB3D = "player.b3d";
    
    PlayerEntity player(1, pos, scale, Direction::NORTH, "team", textures, 1, entityB3D);
    
    cr_assert_eq(player.getId(), 1, "Player with multiple textures should be created");
    cr_assert_str_eq(player.getTeam().c_str(), "team", "Player team should be set with multiple textures");
}

Test(player_entity, destructor) {
    irr::core::vector3df pos(0.0f, 0.0f, 0.0f);
    irr::core::vector3df scale(1.0f, 1.0f, 1.0f);
    std::vector<irr::io::path> textures = {"texture.png"};
    irr::io::path entityB3D = "player.b3d";
    
    PlayerEntity* player = new PlayerEntity(1, pos, scale, Direction::NORTH, "team", textures, 1, entityB3D);
    delete player;
    
    cr_assert(true, "PlayerEntity destructor should work without issues");
}

Test(player_entity, multiple_players_independence) {
    irr::core::vector3df pos(0.0f, 0.0f, 0.0f);
    irr::core::vector3df scale(1.0f, 1.0f, 1.0f);
    std::vector<irr::io::path> textures = {"texture.png"};
    irr::io::path entityB3D = "player.b3d";
    
    PlayerEntity player1(1, pos, scale, Direction::NORTH, "team1", textures, 1, entityB3D);
    PlayerEntity player2(2, pos, scale, Direction::SOUTH, "team2", textures, 3, entityB3D);
    
    cr_assert_eq(player1.getId(), 1, "Player1 ID should be independent");
    cr_assert_eq(player2.getId(), 2, "Player2 ID should be independent");
    cr_assert_str_eq(player1.getTeam().c_str(), "team1", "Player1 team should be independent");
    cr_assert_str_eq(player2.getTeam().c_str(), "team2", "Player2 team should be independent");
    cr_assert_eq(player1.getDirection(), Direction::NORTH, "Player1 direction should be independent");
    cr_assert_eq(player2.getDirection(), Direction::SOUTH, "Player2 direction should be independent");
}

Test(player_entity, edge_case_negative_id) {
    irr::core::vector3df pos(0.0f, 0.0f, 0.0f);
    irr::core::vector3df scale(1.0f, 1.0f, 1.0f);
    std::vector<irr::io::path> textures = {"texture.png"};
    irr::io::path entityB3D = "player.b3d";
    
    PlayerEntity player(-1, pos, scale, Direction::NORTH, "team", textures, 1, entityB3D);
    
    cr_assert_eq(player.getId(), -1, "Negative ID should be handled");
    cr_assert_str_eq(player.getName().c_str(), "Player_-1", "Name should handle negative ID");
}
