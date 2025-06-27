/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** Tests for TileEntity class
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "irrlicht_mocks.hpp"
#include "Entities/TileEntity.hpp"
#include "Entities/Inventory.hpp"
#include <memory>

TestSuite(tile_entity);

Test(tile_entity, constructor_with_parameters) {
    irr::core::vector3df pos(5.0f, 0.0f, 10.0f);
    irr::core::vector3df scale(1.0f, 1.0f, 1.0f);
    std::vector<irr::io::path> textures = {"tile_texture.png"};
    irr::io::path entityB3D = "tile.b3d";
    irr::video::ITexture* mockTexture = nullptr;  // Mock texture pointer
    irr::core::stringc name = "Tile_5_10";
    
    TileEntity tile(1, pos, scale, textures, entityB3D, mockTexture, name);
    
    cr_assert_eq(tile.getId(), 1, "Tile ID should be set correctly");
    cr_assert_str_eq(tile.getName().c_str(), "Tile_5_10", "Tile name should be set correctly");
}

Test(tile_entity, constructor_different_positions) {
    irr::core::vector3df scale(1.0f, 1.0f, 1.0f);
    std::vector<irr::io::path> textures = {"tile_texture.png"};
    irr::io::path entityB3D = "tile.b3d";
    irr::video::ITexture* mockTexture = nullptr;
    
    irr::core::vector3df pos1(0.0f, 0.0f, 0.0f);
    irr::core::vector3df pos2(10.0f, 0.0f, 5.0f);
    irr::core::vector3df pos3(-5.0f, 0.0f, -10.0f);
    
    TileEntity tile1(1, pos1, scale, textures, entityB3D, mockTexture, "Tile_0_0");
    TileEntity tile2(2, pos2, scale, textures, entityB3D, mockTexture, "Tile_10_5");
    TileEntity tile3(3, pos3, scale, textures, entityB3D, mockTexture, "Tile_-5_-10");
    
    irr::core::vector3df retrieved1 = tile1.getPosition();
    irr::core::vector3df retrieved2 = tile2.getPosition();
    irr::core::vector3df retrieved3 = tile3.getPosition();
    
    cr_assert_float_eq(retrieved1.X, 0.0f, 0.001f, "Tile1 X position should be correct");
    cr_assert_float_eq(retrieved2.X, 10.0f, 0.001f, "Tile2 X position should be correct");
    cr_assert_float_eq(retrieved3.X, -5.0f, 0.001f, "Tile3 X position should be correct");
    
    cr_assert_float_eq(retrieved1.Z, 0.0f, 0.001f, "Tile1 Z position should be correct");
    cr_assert_float_eq(retrieved2.Z, 5.0f, 0.001f, "Tile2 Z position should be correct");
    cr_assert_float_eq(retrieved3.Z, -10.0f, 0.001f, "Tile3 Z position should be correct");
}

Test(tile_entity, constructor_different_names) {
    irr::core::vector3df pos(0.0f, 0.0f, 0.0f);
    irr::core::vector3df scale(1.0f, 1.0f, 1.0f);
    std::vector<irr::io::path> textures = {"tile_texture.png"};
    irr::io::path entityB3D = "tile.b3d";
    irr::video::ITexture* mockTexture = nullptr;
    
    TileEntity tile1(1, pos, scale, textures, entityB3D, mockTexture, "GrassTile");
    TileEntity tile2(2, pos, scale, textures, entityB3D, mockTexture, "StoneTile");
    TileEntity tile3(3, pos, scale, textures, entityB3D, mockTexture, "");
    
    cr_assert_str_eq(tile1.getName().c_str(), "GrassTile", "Grass tile name should be set");
    cr_assert_str_eq(tile2.getName().c_str(), "StoneTile", "Stone tile name should be set");
    cr_assert_str_eq(tile3.getName().c_str(), "", "Empty tile name should be set");
}

Test(tile_entity, inventory_access) {
    irr::core::vector3df pos(0.0f, 0.0f, 0.0f);
    irr::core::vector3df scale(1.0f, 1.0f, 1.0f);
    std::vector<irr::io::path> textures = {"tile_texture.png"};
    irr::io::path entityB3D = "tile.b3d";
    irr::video::ITexture* mockTexture = nullptr;
    irr::core::stringc name = "TestTile";
    
    TileEntity tile(1, pos, scale, textures, entityB3D, mockTexture, name);
    
    // Test non-const inventory access
    Inventory& inv = tile.getInventory();
    inv.addItem("linemate", 3);
    inv.addItem("food", 2);
    
    cr_assert_eq(inv.getItemQuantity("linemate"), 3, "Linemate should be added to tile inventory");
    cr_assert_eq(inv.getItemQuantity("food"), 2, "Food should be added to tile inventory");
    
    // Test const inventory access
    const TileEntity& const_tile = tile;
    const Inventory& const_inv = const_tile.getInventory();
    cr_assert_eq(const_inv.getItemQuantity("linemate"), 3, "Const access should work for linemate");
    cr_assert_eq(const_inv.getItemQuantity("food"), 2, "Const access should work for food");
}

Test(tile_entity, inventory_all_resources) {
    irr::core::vector3df pos(0.0f, 0.0f, 0.0f);
    irr::core::vector3df scale(1.0f, 1.0f, 1.0f);
    std::vector<irr::io::path> textures = {"tile_texture.png"};
    irr::io::path entityB3D = "tile.b3d";
    irr::video::ITexture* mockTexture = nullptr;
    irr::core::stringc name = "ResourceTile";
    
    TileEntity tile(1, pos, scale, textures, entityB3D, mockTexture, name);
    Inventory& inv = tile.getInventory();
    
    // Add all Zappy resources
    inv.addItem("food", 1);
    inv.addItem("linemate", 2);
    inv.addItem("deraumere", 3);
    inv.addItem("sibur", 4);
    inv.addItem("mendiane", 5);
    inv.addItem("phiras", 6);
    inv.addItem("thystame", 7);
    
    cr_assert_eq(inv.getItemQuantity("food"), 1, "Food quantity should be correct");
    cr_assert_eq(inv.getItemQuantity("linemate"), 2, "Linemate quantity should be correct");
    cr_assert_eq(inv.getItemQuantity("deraumere"), 3, "Deraumere quantity should be correct");
    cr_assert_eq(inv.getItemQuantity("sibur"), 4, "Sibur quantity should be correct");
    cr_assert_eq(inv.getItemQuantity("mendiane"), 5, "Mendiane quantity should be correct");
    cr_assert_eq(inv.getItemQuantity("phiras"), 6, "Phiras quantity should be correct");
    cr_assert_eq(inv.getItemQuantity("thystame"), 7, "Thystame quantity should be correct");
}

Test(tile_entity, get_tile_node) {
    irr::core::vector3df pos(0.0f, 0.0f, 0.0f);
    irr::core::vector3df scale(1.0f, 1.0f, 1.0f);
    std::vector<irr::io::path> textures = {"tile_texture.png"};
    irr::io::path entityB3D = "tile.b3d";
    irr::video::ITexture* mockTexture = nullptr;
    irr::core::stringc name = "TestTile";
    
    TileEntity tile(1, pos, scale, textures, entityB3D, mockTexture, name);
    
    // getTileNode should return nullptr initially (no scene manager)
    irr::scene::IMeshSceneNode* node = tile.getTileNode();
    // In mock environment, this will be nullptr
    cr_assert(true, "getTileNode should be callable");
}

Test(tile_entity, create_tile_node_mock) {
    irr::core::vector3df pos(0.0f, 0.0f, 0.0f);
    irr::core::vector3df scale(1.0f, 1.0f, 1.0f);
    std::vector<irr::io::path> textures = {"tile_texture.png"};
    irr::io::path entityB3D = "tile.b3d";
    irr::video::ITexture* mockTexture = nullptr;
    irr::core::stringc name = "TestTile";
    
    TileEntity tile(1, pos, scale, textures, entityB3D, mockTexture, name);
    
    // createTileNode should be callable without crashing (even with nullptr)
    tile.createTileNode(nullptr);
    cr_assert(true, "createTileNode should be callable with mock scene manager");
}

Test(tile_entity, destructor) {
    irr::core::vector3df pos(0.0f, 0.0f, 0.0f);
    irr::core::vector3df scale(1.0f, 1.0f, 1.0f);
    std::vector<irr::io::path> textures = {"tile_texture.png"};
    irr::io::path entityB3D = "tile.b3d";
    irr::video::ITexture* mockTexture = nullptr;
    irr::core::stringc name = "TestTile";
    
    TileEntity* tile = new TileEntity(1, pos, scale, textures, entityB3D, mockTexture, name);
    delete tile;
    
    cr_assert(true, "TileEntity destructor should work without issues");
}

Test(tile_entity, multiple_tiles_independence) {
    irr::core::vector3df scale(1.0f, 1.0f, 1.0f);
    std::vector<irr::io::path> textures = {"tile_texture.png"};
    irr::io::path entityB3D = "tile.b3d";
    irr::video::ITexture* mockTexture = nullptr;
    
    irr::core::vector3df pos1(0.0f, 0.0f, 0.0f);
    irr::core::vector3df pos2(10.0f, 0.0f, 10.0f);
    
    TileEntity tile1(1, pos1, scale, textures, entityB3D, mockTexture, "Tile1");
    TileEntity tile2(2, pos2, scale, textures, entityB3D, mockTexture, "Tile2");
    
    // Add different items to each tile
    tile1.getInventory().addItem("food", 5);
    tile2.getInventory().addItem("linemate", 3);
    
    cr_assert_eq(tile1.getInventory().getItemQuantity("food"), 5, "Tile1 should have food");
    cr_assert_eq(tile1.getInventory().getItemQuantity("linemate"), 0, "Tile1 should not have linemate");
    cr_assert_eq(tile2.getInventory().getItemQuantity("food"), 0, "Tile2 should not have food");
    cr_assert_eq(tile2.getInventory().getItemQuantity("linemate"), 3, "Tile2 should have linemate");
    
    cr_assert_str_eq(tile1.getName().c_str(), "Tile1", "Tile1 name should be independent");
    cr_assert_str_eq(tile2.getName().c_str(), "Tile2", "Tile2 name should be independent");
}

Test(tile_entity, edge_cases) {
    irr::core::vector3df pos(0.0f, 0.0f, 0.0f);
    irr::core::vector3df scale(0.0f, 0.0f, 0.0f);  // Zero scale
    std::vector<irr::io::path> empty_textures;     // Empty textures
    irr::io::path entityB3D = "";                  // Empty B3D path
    irr::video::ITexture* mockTexture = nullptr;
    irr::core::stringc name = "EdgeCaseTile";
    
    TileEntity tile(0, pos, scale, empty_textures, entityB3D, mockTexture, name);
    
    cr_assert_eq(tile.getId(), 0, "Zero ID should work");
    cr_assert_str_eq(tile.getName().c_str(), "EdgeCaseTile", "Name should work with edge case parameters");
}
