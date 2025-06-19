// Unitary tests for Entities (PlayerEntity, Stone, TileEntity, Inventory)
#include "../../gui/Entities/PlayerEntity.hpp"
#include "../../gui/Entities/Stone.hpp"
#include "../../gui/Entities/TileEntity.hpp"
#include "../../gui/Entities/Inventory.hpp"
#include <gtest/gtest.h>
#include <memory>

TEST(EntitiesTest, PlayerEntityConstruction) {
    irr::core::vector3df pos(0,0,0);
    irr::core::vector3df scale(1,1,1);
    std::vector<irr::io::path> textures;
    irr::io::path entityB3D = "player.b3d";
    PlayerEntity player(1, pos, scale, Direction::NORTH, "Red", textures, 2, entityB3D);
    EXPECT_EQ(player.getId(), 1);
    EXPECT_EQ(player.getTeam(), "Red");
    EXPECT_EQ(player.getDirection(), Direction::NORTH);
}

TEST(EntitiesTest, StoneType) {
    irr::core::vector3df pos(0,0,0);
    irr::core::vector3df scale(1,1,1);
    std::vector<irr::io::path> textures;
    irr::io::path entityB3D = "stone.b3d";
    Stone stone(42, pos, scale, textures, entityB3D, "sibur");
    EXPECT_EQ(stone.getType(), "sibur");
    stone.setType("linemate");
    EXPECT_EQ(stone.getType(), "linemate");
}

TEST(EntitiesTest, TileEntityPosition) {
    irr::core::vector3df pos(2,0,3);
    irr::core::vector3df scale(1,1,1);
    std::vector<irr::io::path> textures;
    irr::io::path entityB3D = "tile.b3d";
    irr::video::ITexture* textureTile = nullptr;
    irr::core::stringc name = "tile";
    TileEntity tile(5, pos, scale, textures, entityB3D, textureTile, name);
    EXPECT_EQ(tile.getId(), 5);
    EXPECT_FLOAT_EQ(tile.getPosition().X, 2);
    EXPECT_FLOAT_EQ(tile.getPosition().Z, 3);
}

TEST(EntitiesTest, InventoryAddGet) {
    Inventory inv;
    inv.addItem("food", 3);
    inv.addItem("linemate", 2);
    EXPECT_EQ(inv.getItemQuantity("food"), 3u);
    EXPECT_EQ(inv.getItemQuantity("linemate"), 2u);
    EXPECT_EQ(inv.getItemQuantity("sibur"), 0u);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
