/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** EntityManager
*/

#include "EntityManager.hpp"
#include <iostream>
#include <sstream>

void EntityManager::createPlayers(int id, int x, int y, Direction direction,
                                  int level, std::string team) {
  std::vector<irr::io::path> texturesArcher = {
      mediaPath_ + "archer_texture/DarkBrown_CrossBow01.png",
      mediaPath_ + "archer_texture/Black_CrossBow01.png",
      mediaPath_ + "archer_texture/Brown_CrossBow01.png",
      mediaPath_ + "archer_texture/ay_head.png",
      mediaPath_ + "archer_texture/RedTeam_Archer_Armor_Red.png",
      mediaPath_ + "archer_texture/BlueTeam_Archer_Helmet_Black.png",
      mediaPath_ + "archer_texture/RedTeam_Archer_Armor_Red.png",
      mediaPath_ + "archer_texture/RedTeam_Archer_Armor_DarkBrown.png",
      mediaPath_ + "archer_texture/RedTeam_Archer_Armor_Brown.png",
      mediaPath_ + "archer_texture/RedTeam_Archer_Gloves_DarkRed.png",
      mediaPath_ + "archer_texture/RedTeam_Archer_Shoulders_Gray.png",
      mediaPath_ + "archer_texture/RedTeam_Archer_Shoulders_Gray.png",
      mediaPath_ + "archer_texture/RedTeam_Archer_Quiver_Brown.png",
      mediaPath_ + "archer_texture/RedTeam_Archer_Belt_Brown.png"};

  entity_.push_back(std::make_shared<PlayerEntity>(
      id, irr::core::vector3df(x, 6, y),
      irr::core::vector3df(12.0f, 12.0f, 12.0f), direction, team,
      texturesArcher, level, mediaPath_ + "archer.b3d"));

  entity_.back()->createNode(smgr_, driver_);
  receiver_.setAnimatedNode(entity_.back()->getNode());
}

void EntityManager::createStones(int x, int y, int q0, int q1, int q2, int q3,
                                 int q4, int q5, int q6) {
  std::vector<std::vector<irr::io::path>> stoneTextures = {
      {mediaPath_ + "stone_texture/food_redbull.png",
       mediaPath_ + "stone_texture/food_redbull.png"},
      {mediaPath_ + "stone_texture/stone_red.png"},
      {mediaPath_ + "stone_texture/stone_orange.png"},
      {mediaPath_ + "stone_texture/stone_yellow.png"},
      {mediaPath_ + "stone_texture/stone_green.png"},
      {mediaPath_ + "stone_texture/stone_blue.png"},
      {mediaPath_ + "stone_texture/stone_purple.png"}};
  std::vector<std::string> stoneNames = {"food",    "linemate", "deraumere",
                                         "sibur",   "mendiane", "phiras",
                                         "thystame"};
  std::vector<irr::io::path> qB3D(7, mediaPath_ + "ruby.b3d");
  qB3D[0] = mediaPath_ + "RedBull.b3d";
  std::vector<irr::core::vector3df> qScale(
      1, irr::core::vector3df(0.4f, 0.4f, 0.4f));
  qScale.resize(7, irr::core::vector3df(0.005f, 0.005f, 0.005f));

  irr::core::vector3df position(0.0f, 0.0f, 0.0f);
  std::ostringstream oss;
  oss << "Cube info: row " << x << " col " << y;
  for (auto &tile : tiles_) {
    if (tile->getName() == oss.str()) {
      position.set(x * 20.0f, 5.0f, y * 20.0f);
      tile->getInventory().addItem("food", q0);
      tile->getInventory().addItem("linemate", q1);
      tile->getInventory().addItem("deraumere", q2);
      tile->getInventory().addItem("sibur", q3);
      tile->getInventory().addItem("mendiane", q4);
      tile->getInventory().addItem("phiras", q5);
      tile->getInventory().addItem("thystame", q6);
      break;
    }
  }
  std::vector<int> quantities = {q0, q1, q2, q3, q4, q5, q6};
  const int gridCols = 8, gridRows = 8;
  const float spacing = 2.1f;
  int objCount = 0;
  float yOffset = 0.0f;
  for (size_t stoneIdx = 0; stoneIdx < quantities.size(); ++stoneIdx)
    for (int i = 0; i < quantities[stoneIdx]; ++i, ++objCount) {
      int gridIndex = objCount % (gridCols * gridRows);
      if (gridIndex == 0 && objCount != 0)
        yOffset += 2.0f;
      int row = gridIndex / gridCols, col = gridIndex % gridCols;
      irr::core::vector3df objPos = position;
      objPos.X += col * spacing - ((gridCols - 1) * spacing) / 2.0f;
      objPos.Z += row * spacing - ((gridRows - 1) * spacing) / 2.0f;
      objPos.Y += yOffset;
      entity_.push_back(std::make_shared<Stone>(
          0, objPos, qScale[stoneIdx], stoneTextures[stoneIdx], qB3D[stoneIdx],
          stoneNames[stoneIdx]));
      entity_.back()->createNode(smgr_, driver_);
    }
}

void EntityManager::createTiles(int x, int y) {
  irr::video::ITexture *image2 = driver_->getTexture(mediaPath_ + "grass.png");
  driver_->makeColorKeyTexture(image2, irr::core::position2d<irr::s32>(0, 0));
  float cubeX = 0.0f;
  float cubeY = 0.0f;
  for (int j = 0; j < x; ++j) {
    for (int i = 0; i < y; ++i) {
      irr::core::stringc name = "Cube info: ";
      name += "row ";
      name += i;
      name += " col ";
      name += j;
      auto tile = std::make_shared<TileEntity>(
          i + j, irr::core::vector3df(cubeX, 0, cubeY),
          irr::core::vector3df(1.0f, 0.5f, 1.0f),
          std::vector<irr::io::path>{mediaPath_ + "grass.png"},
          mediaPath_ + "cube.b3d", image2, name);
      tile->createTileNode(smgr_);
      receiver_.addCube(tile.get());
      tiles_.push_back(tile);
      cubeX += 20.0f;
    }
    cubeX = 0.0f;
    cubeY += 20.0f;
  }
}