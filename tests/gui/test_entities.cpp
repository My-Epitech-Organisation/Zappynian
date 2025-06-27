/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** Tests for Entity classes (PlayerEntity, Stone, TileEntity, Egg)
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "irrlicht_mocks.hpp"  // Mock Irrlicht types for testing
#include "Entities/PlayerEntity.hpp"
#include "Entities/Stone.hpp"
#include "Entities/TileEntity.hpp"
#include "Entities/Egg.hpp"
#include "Entities/AEntity.hpp"
#include <memory>
#include <vector>
#include <string>

// Helper function to create a mock device-free entity
class MockEntity : public AEntity {
public:
    MockEntity() : AEntity() {}
    
    void createNode(irr::scene::ISceneManager *smgr, irr::video::IVideoDriver *driver) override {
        // Mock implementation that doesn't require real Irrlicht device
        (void)smgr;
        (void)driver;
    }
};

TestSuite(entities);

Test(entities, aentity_basic_properties) {
    MockEntity entity;
    
    entity.setId(42);
    cr_assert_eq(entity.getId(), 42, "ID should be set correctly");
    
    entity.setName("TestEntity");
    cr_assert_str_eq(entity.getName().c_str(), "TestEntity", "Name should be set correctly");
    
    irr::core::vector3df pos(1.0f, 2.0f, 3.0f);
    entity.setPosition(pos);
    irr::core::vector3df retrievedPos = entity.getPosition();
    cr_assert_float_eq(retrievedPos.X, 1.0f, 0.001f);
    cr_assert_float_eq(retrievedPos.Y, 2.0f, 0.001f);
    cr_assert_float_eq(retrievedPos.Z, 3.0f, 0.001f);
    
    irr::core::vector3df scale(2.0f, 2.0f, 2.0f);
    entity.setScale(scale);
    irr::core::vector3df retrievedScale = entity.getScale();
    cr_assert_float_eq(retrievedScale.X, 2.0f, 0.001f);
    cr_assert_float_eq(retrievedScale.Y, 2.0f, 0.001f);
    cr_assert_float_eq(retrievedScale.Z, 2.0f, 0.001f);
    
    entity.setLevel(5);
    cr_assert_eq(entity.getLevel(), 5, "Level should be set correctly");
}

Test(entities, aentity_direction_management) {
    MockEntity entity;
    
    entity.setDirection(Direction::NORTH);
    cr_assert_eq(entity.getDirection(), Direction::NORTH, "Direction should be NORTH");
    
    entity.setDirection(Direction::EAST);
    cr_assert_eq(entity.getDirection(), Direction::EAST, "Direction should be EAST");
    
    entity.setDirection(Direction::SOUTH);
    cr_assert_eq(entity.getDirection(), Direction::SOUTH, "Direction should be SOUTH");
    
    entity.setDirection(Direction::WEST);
    cr_assert_eq(entity.getDirection(), Direction::WEST, "Direction should be WEST");
}

Test(entities, aentity_inventory_access) {
    MockEntity entity;
    
    Inventory& inventory = entity.getInventory();
    inventory.addItem("food", 10);
    
    const Inventory& constInventory = entity.getInventory();
    cr_assert_eq(constInventory.getItemQuantity("food"), 10, "Inventory should be accessible");
}

Test(entities, player_entity_construction) {
    irr::core::vector3df pos(5.0f, 5.0f, 0.0f);
    irr::core::vector3df scale(1.0f, 1.0f, 1.0f);
    std::vector<irr::io::path> textures = {"texture1.png", "texture2.png"};
    
    PlayerEntity player(1, pos, scale, Direction::NORTH, "TeamRed", textures, 3, "player.b3d");
    
    cr_assert_eq(player.getId(), 1, "Player ID should be set correctly");
    cr_assert_str_eq(player.getName().c_str(), "Player_1", "Player name should be auto-generated");
    cr_assert_eq(player.getDirection(), Direction::NORTH, "Player direction should be set");
    cr_assert_str_eq(player.getTeam().c_str(), "TeamRed", "Player team should be set");
    cr_assert_eq(player.getLevel(), 3, "Player level should be set");
    
    irr::core::vector3df retrievedPos = player.getPosition();
    cr_assert_float_eq(retrievedPos.X, 5.0f, 0.001f);
    cr_assert_float_eq(retrievedPos.Y, 5.0f, 0.001f);
    cr_assert_float_eq(retrievedPos.Z, 0.0f, 0.001f);
}

Test(entities, player_entity_different_teams) {
    irr::core::vector3df pos(0, 0, 0);
    irr::core::vector3df scale(1, 1, 1);
    std::vector<irr::io::path> textures;
    
    PlayerEntity redPlayer(1, pos, scale, Direction::NORTH, "Red", textures, 1, "player.b3d");
    PlayerEntity bluePlayer(2, pos, scale, Direction::EAST, "Blue", textures, 2, "player.b3d");
    PlayerEntity greenPlayer(3, pos, scale, Direction::SOUTH, "Green", textures, 3, "player.b3d");
    
    cr_assert_str_eq(redPlayer.getTeam().c_str(), "Red", "Red player team should be correct");
    cr_assert_str_eq(bluePlayer.getTeam().c_str(), "Blue", "Blue player team should be correct");
    cr_assert_str_eq(greenPlayer.getTeam().c_str(), "Green", "Green player team should be correct");
    
    cr_assert_eq(redPlayer.getDirection(), Direction::NORTH, "Red player direction should be correct");
    cr_assert_eq(bluePlayer.getDirection(), Direction::EAST, "Blue player direction should be correct");
    cr_assert_eq(greenPlayer.getDirection(), Direction::SOUTH, "Green player direction should be correct");
}

Test(entities, player_entity_levels) {
    irr::core::vector3df pos(0, 0, 0);
    irr::core::vector3df scale(1, 1, 1);
    std::vector<irr::io::path> textures;
    
    // Test all possible levels (1-8 in Zappy)
    for (int level = 1; level <= 8; ++level) {
        PlayerEntity player(level, pos, scale, Direction::NORTH, "Team", textures, level, "player.b3d");
        cr_assert_eq(player.getLevel(), level, "Player level should be set correctly");
    }
}

Test(entities, tile_entity_construction) {
    irr::core::vector3df pos(2.0f, 3.0f, 0.0f);
    irr::core::vector3df scale(1.0f, 0.5f, 1.0f);
    std::vector<irr::io::path> textures = {"grass.png"};
    
    TileEntity tile(1, pos, scale, textures, "cube.b3d", nullptr, "TestTile");
    
    cr_assert_eq(tile.getId(), 1, "Tile ID should be set correctly");
    
    irr::core::vector3df retrievedPos = tile.getPosition();
    cr_assert_float_eq(retrievedPos.X, 2.0f, 0.001f);
    cr_assert_float_eq(retrievedPos.Y, 3.0f, 0.001f);
    cr_assert_float_eq(retrievedPos.Z, 0.0f, 0.001f);
    
    irr::core::vector3df retrievedScale = tile.getScale();
    cr_assert_float_eq(retrievedScale.X, 1.0f, 0.001f);
    cr_assert_float_eq(retrievedScale.Y, 0.5f, 0.001f);
    cr_assert_float_eq(retrievedScale.Z, 1.0f, 0.001f);
}

Test(entities, tile_entity_inventory) {
    irr::core::vector3df pos(0, 0, 0);
    irr::core::vector3df scale(1, 1, 1);
    std::vector<irr::io::path> textures;
    
    TileEntity tile(1, pos, scale, textures, "cube.b3d", nullptr, "ResourceTile");
    
    Inventory& inventory = tile.getInventory();
    inventory.addItem("linemate", 5);
    inventory.addItem("deraumere", 3);
    inventory.addItem("food", 2);
    
    cr_assert_eq(inventory.getItemQuantity("linemate"), 5, "Tile should store linemate");
    cr_assert_eq(inventory.getItemQuantity("deraumere"), 3, "Tile should store deraumere");
    cr_assert_eq(inventory.getItemQuantity("food"), 2, "Tile should store food");
    
    const Inventory& constInventory = tile.getInventory();
    cr_assert_eq(constInventory.getItemQuantity("linemate"), 5, "Const access should work");
}

Test(entities, stone_entity_construction) {
    irr::core::vector3df pos(1.0f, 1.0f, 0.0f);
    irr::core::vector3df scale(0.5f, 0.5f, 0.5f);
    std::vector<irr::io::path> textures = {"stone.png"};
    
    Stone stone(1, pos, scale, textures, "stone.b3d", "linemate");
    
    cr_assert_eq(stone.getId(), 1, "Stone ID should be set correctly");
    cr_assert_str_eq(stone.getName().c_str(), "Stone", "Stone name should be 'Stone'");
    cr_assert_str_eq(stone.getType().c_str(), "linemate", "Stone type should be 'linemate'");
    
    irr::core::vector3df retrievedPos = stone.getPosition();
    cr_assert_float_eq(retrievedPos.X, 1.0f, 0.001f);
    cr_assert_float_eq(retrievedPos.Y, 1.0f, 0.001f);
    cr_assert_float_eq(retrievedPos.Z, 0.0f, 0.001f);
}

Test(entities, stone_entity_types) {
    irr::core::vector3df pos(0, 0, 0);
    irr::core::vector3df scale(1, 1, 1);
    std::vector<irr::io::path> textures;
    
    std::vector<std::string> stoneTypes = {
        "linemate", "deraumere", "sibur", "mendiane", "phiras", "thystame", "food"
    };
    
    for (size_t i = 0; i < stoneTypes.size(); ++i) {
        Stone stone(i + 1, pos, scale, textures, "stone.b3d", stoneTypes[i]);
        cr_assert_str_eq(stone.getType().c_str(), stoneTypes[i].c_str(), 
                        "Stone type should be set correctly");
    }
}

Test(entities, stone_entity_type_modification) {
    irr::core::vector3df pos(0, 0, 0);
    irr::core::vector3df scale(1, 1, 1);
    std::vector<irr::io::path> textures;
    
    Stone stone(1, pos, scale, textures, "stone.b3d", "linemate");
    cr_assert_str_eq(stone.getType().c_str(), "linemate", "Initial type should be linemate");
    
    stone.setType("deraumere");
    cr_assert_str_eq(stone.getType().c_str(), "deraumere", "Type should be changed to deraumere");
    
    stone.setType("food");
    cr_assert_str_eq(stone.getType().c_str(), "food", "Type should be changed to food");
}

Test(entities, stone_entity_default_type) {
    irr::core::vector3df pos(0, 0, 0);
    irr::core::vector3df scale(1, 1, 1);
    std::vector<irr::io::path> textures;
    
    Stone stone(1, pos, scale, textures, "stone.b3d");  // No type specified
    cr_assert_str_eq(stone.getType().c_str(), "linemate", "Default type should be linemate");
}

Test(entities, egg_entity_construction) {
    irr::core::vector3df pos(3.0f, 4.0f, 0.0f);
    irr::core::vector3df scale(0.8f, 0.8f, 0.8f);
    std::vector<irr::io::path> textures = {"egg.png"};
    
    Egg egg(1, pos, scale, textures, "egg.b3d");
    
    cr_assert_eq(egg.getId(), 1, "Egg ID should be set correctly");
    cr_assert_str_eq(egg.getName().c_str(), "Egg", "Egg name should be 'Egg'");
    cr_assert_str_eq(egg.getType().c_str(), "egg", "Default egg type should be 'egg'");
    
    irr::core::vector3df retrievedPos = egg.getPosition();
    cr_assert_float_eq(retrievedPos.X, 3.0f, 0.001f);
    cr_assert_float_eq(retrievedPos.Y, 4.0f, 0.001f);
    cr_assert_float_eq(retrievedPos.Z, 0.0f, 0.001f);
}

Test(entities, egg_entity_type_states) {
    irr::core::vector3df pos(0, 0, 0);
    irr::core::vector3df scale(1, 1, 1);
    std::vector<irr::io::path> textures;
    
    Egg egg(1, pos, scale, textures, "egg.b3d");
    cr_assert_str_eq(egg.getType().c_str(), "egg", "Initial type should be 'egg'");
    
    egg.setType("hatching");
    cr_assert_str_eq(egg.getType().c_str(), "hatching", "Type should be changed to 'hatching'");
    
    egg.setType("egg");
    cr_assert_str_eq(egg.getType().c_str(), "egg", "Type should be changed back to 'egg'");
    
    egg.setType("ready");
    cr_assert_str_eq(egg.getType().c_str(), "ready", "Custom type should be accepted");
}

Test(entities, multiple_entities_coexistence) {
    irr::core::vector3df pos(0, 0, 0);
    irr::core::vector3df scale(1, 1, 1);
    std::vector<irr::io::path> textures;
    
    PlayerEntity player(1, pos, scale, Direction::NORTH, "Red", textures, 1, "player.b3d");
    TileEntity tile(2, pos, scale, textures, "cube.b3d", nullptr, "Tile");
    Stone stone(3, pos, scale, textures, "stone.b3d", "linemate");
    Egg egg(4, pos, scale, textures, "egg.b3d");
    
    // All entities should coexist without issues
    cr_assert_eq(player.getId(), 1, "Player should maintain its ID");
    cr_assert_eq(tile.getId(), 2, "Tile should maintain its ID");
    cr_assert_eq(stone.getId(), 3, "Stone should maintain its ID");
    cr_assert_eq(egg.getId(), 4, "Egg should maintain its ID");
    
    cr_assert_str_eq(player.getTeam().c_str(), "Red", "Player should maintain its team");
    cr_assert_str_eq(stone.getType().c_str(), "linemate", "Stone should maintain its type");
    cr_assert_str_eq(egg.getType().c_str(), "egg", "Egg should maintain its type");
}

Test(entities, entity_polymorphism) {
    irr::core::vector3df pos(0, 0, 0);
    irr::core::vector3df scale(1, 1, 1);
    std::vector<irr::io::path> textures;
    
    // Test polymorphic behavior through base class pointers
    std::unique_ptr<IEntity> player = std::make_unique<PlayerEntity>(
        1, pos, scale, Direction::NORTH, "Red", textures, 1, "player.b3d");
    std::unique_ptr<IEntity> tile = std::make_unique<TileEntity>(
        2, pos, scale, textures, "cube.b3d", nullptr, "Tile");
    std::unique_ptr<IEntity> stone = std::make_unique<Stone>(
        3, pos, scale, textures, "stone.b3d", "linemate");
    std::unique_ptr<IEntity> egg = std::make_unique<Egg>(
        4, pos, scale, textures, "egg.b3d");
    
    // Test polymorphic method calls
    cr_assert_eq(player->getId(), 1, "Polymorphic ID access should work for player");
    cr_assert_eq(tile->getId(), 2, "Polymorphic ID access should work for tile");
    cr_assert_eq(stone->getId(), 3, "Polymorphic ID access should work for stone");
    cr_assert_eq(egg->getId(), 4, "Polymorphic ID access should work for egg");
    
    player->setLevel(5);
    cr_assert_eq(player->getLevel(), 5, "Polymorphic level setting should work");
}

Test(entities, stress_test_entity_creation) {
    // Create many entities to test memory management and performance
    std::vector<std::unique_ptr<IEntity>> entities;
    
    irr::core::vector3df basePos(0, 0, 0);
    irr::core::vector3df scale(1, 1, 1);
    std::vector<irr::io::path> textures;
    
    for (int i = 0; i < 1000; ++i) {
        irr::core::vector3df pos(i % 100, i / 100, 0);
        
        switch (i % 4) {
            case 0:
                entities.push_back(std::make_unique<PlayerEntity>(
                    i, pos, scale, static_cast<Direction>(i % 4), "Team" + std::to_string(i % 5),
                    textures, (i % 8) + 1, "player.b3d"));
                break;
            case 1:
                entities.push_back(std::make_unique<TileEntity>(
                    i, pos, scale, textures, "cube.b3d", nullptr, irr::core::stringc(("Tile" + std::to_string(i)).c_str())));
                break;
            case 2:
                entities.push_back(std::make_unique<Stone>(
                    i, pos, scale, textures, "stone.b3d", "linemate"));
                break;
            case 3:
                entities.push_back(std::make_unique<Egg>(
                    i, pos, scale, textures, "egg.b3d"));
                break;
        }
        }
    
    cr_assert_eq(entities.size(), 1000, "Should create 1000 entities");
    
    // Verify random entities
    cr_assert_eq(entities[0]->getId(), 0, "First entity should have correct ID");
    cr_assert_eq(entities[999]->getId(), 999, "Last entity should have correct ID");
    cr_assert_eq(entities[500]->getId(), 500, "Middle entity should have correct ID");
}

Test(entities, aentity_basic_operations) {
    MockEntity entity;
    
    // Test default values
    cr_assert_eq(entity.getId(), 0);
    cr_assert_str_eq(entity.getName().c_str(), "unknown");
    cr_assert_eq(entity.getLevel(), 1);
    
    // Test setters and getters
    entity.setId(42);
    cr_assert_eq(entity.getId(), 42);
    
    entity.setName("TestEntity");
    cr_assert_str_eq(entity.getName().c_str(), "TestEntity");
    
    entity.setLevel(5);
    cr_assert_eq(entity.getLevel(), 5);
}

Test(entities, aentity_position_operations) {
    MockEntity entity;
    irr::core::vector3df pos(10.0f, 5.0f, 15.0f);
    
    entity.setPosition(pos);
    irr::core::vector3df retrieved = entity.getPosition();
    
    cr_assert_float_eq(retrieved.X, 10.0f, 0.001f);
    cr_assert_float_eq(retrieved.Y, 5.0f, 0.001f);
    cr_assert_float_eq(retrieved.Z, 15.0f, 0.001f);
}

Test(entities, aentity_scale_operations) {
    MockEntity entity;
    irr::core::vector3df scale(2.0f, 1.5f, 0.5f);
    
    entity.setScale(scale);
    irr::core::vector3df retrieved = entity.getScale();
    
    cr_assert_float_eq(retrieved.X, 2.0f, 0.001f);
    cr_assert_float_eq(retrieved.Y, 1.5f, 0.001f);
        cr_assert_float_eq(retrieved.Z, 0.5f, 0.001f);
}
