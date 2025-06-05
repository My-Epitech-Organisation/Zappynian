/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** EntityManager
*/

#include "EntityManager.hpp"

void EntityManager::createPlayers(int id, int x, int y, Direction direction, int level,
                      std::string team) {
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
      id, irr::core::vector3df(x, 6, y), irr::core::vector3df(8.0f, 8.0f, 8.0f),
      direction, team, texturesArcher, level, mediaPath_ + "archer.b3d"));

  entity_[0]->createNode(smgr_, driver_, receiver_);
  receiver_.setAnimatedNode(entity_[0]->getNode());
}

void EntityManager::createStones() {
  std::vector<irr::io::path> texturesStone = {
      mediaPath_ + "archer_texture/RedTeam_Archer_Armor_Red.png"};

  entity_.push_back(std::make_shared<Stone>(
      1, irr::core::vector3df(0, 5, -60),
      irr::core::vector3df(0.01f, 0.01f, 0.01f), texturesStone,
      mediaPath_ + "ruby.b3d", "linemate"));

  entity_[1]->createNode(smgr_, driver_, receiver_);
}

void EntityManager::createTiles() {
  irr::video::ITexture *image2 = driver_->getTexture(mediaPath_ + "grass.png");
  driver_->makeColorKeyTexture(image2, irr::core::position2d<irr::s32>(0, 0));
  std::vector<irr::scene::IMeshSceneNode *> cubes;
  float cubeX = -100.0f;
  float cubeY = -100.0f;
  for (int j = 0; j < 10; ++j) {
    for (int i = 0; i < 10; ++i) {
      irr::core::stringc name = "Cube info: ";
      name += "row ";
      name += i;
      name += " col ";
      name += j;
      TileEntity *tile = new TileEntity(
          i + j, irr::core::vector3df(cubeX, 0, cubeY),
          irr::core::vector3df(1.0f, 0.5f, 1.0f), {mediaPath_ + "grass.png"},
          mediaPath_ + "cube.b3d", image2, name);
      tile->createTileNode(smgr_, driver_, receiver_);
      cubes.push_back(tile->getTileNode());
      receiver_.addCube(tile->getTileNode());
      cubeX += 20.0f;
    }
    cubeX = -100.0f;
    cubeY += 20.0f;
  }
}