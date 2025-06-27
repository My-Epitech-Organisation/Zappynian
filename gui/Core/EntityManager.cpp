/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** EntityManager
*/

#include "EntityManager.hpp"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <sstream>

void EntityManager::createPlayers(int id, int x, int y, Direction direction,
                                  int level, std::string team) {
  std::vector<irr::io::path> texturesTeams = {
      mediaPath_ + "archer_texture/archer_red.png",
      mediaPath_ + "archer_texture/archer_blue.png",
      mediaPath_ + "archer_texture/archer_green.png",
      mediaPath_ + "archer_texture/archer_yellow.png",
      mediaPath_ + "archer_texture/archer_orange.png",
      mediaPath_ + "archer_texture/archer_purple.png"};

  size_t teamIndex = 0;
  for (size_t i = 0; i < teams_.size(); ++i) {
    if (teams_[i] == team) {
      team = teams_[i];
      teamIndex = i;
      break;
    }
  }

  std::vector<irr::io::path> texturesArcher = {
      mediaPath_ + "archer_texture/DarkBrown_CrossBow01.png",
      mediaPath_ + "archer_texture/Black_CrossBow01.png",
      mediaPath_ + "archer_texture/Brown_CrossBow01.png",
      mediaPath_ + "archer_texture/ay_head.png",
      texturesTeams[teamIndex],
      mediaPath_ + "archer_texture/BlueTeam_Archer_Helmet_Black.png",
      texturesTeams[teamIndex],
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
  entity_.back()->getNode()->setRotation(
      irr::core::vector3df(0.0f, static_cast<float>(direction) * 90.0f, 0.0f));
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
      1, irr::core::vector3df(0.8f, 0.8f, 0.8f));
  qScale.resize(7, irr::core::vector3df(0.009f, 0.009f, 0.009f));

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
  placeStoneEntities(position, quantities, stoneTextures, qB3D, stoneNames,
                     qScale);
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
          -4, irr::core::vector3df(cubeX, 0, cubeY),
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

std::shared_ptr<IEntity>
EntityManager::createPaperPlane(irr::core::vector3df position) {
  auto paperPlane = std::make_shared<TileEntity>(
      -5, position, irr::core::vector3df(1.0f, 1.0f, 1.0f),
      std::vector<irr::io::path>{mediaPath_ +
                                 "plane_texture/plane_texture.jpg"},
      mediaPath_ + "plane.b3d", nullptr, "Paper Plane");
  paperPlane->createNode(smgr_, driver_);
  entity_.push_back(paperPlane);
  return paperPlane;
}

void EntityManager::createEgg(int id) {
  for (const auto &entities : entity_) {
    auto player = std::dynamic_pointer_cast<PlayerEntity>(entities);
    if (player && player->getId() == id) {
      irr::core::vector3df pos = player->getNode()->getPosition();
      std::vector<irr::io::path> eggTextures = {mediaPath_ +
                                                "egg_texture/egg_texture.png"};
      auto egg =
          std::make_shared<Egg>(-7, pos, irr::core::vector3df(20.f, 20.f, 20.f),
                                eggTextures, mediaPath_ + "Egg.b3d");
      entity_.push_back(egg);
      egg->createNode(smgr_, driver_);
      auto tile = getTileByName(
          "Cube info: row " + std::to_string(static_cast<int>(pos.X / 20)) +
          " col " + std::to_string(static_cast<int>(pos.Z / 20)));
      if (tile)
        tile->getInventory().addItem("egg", 1);
      break;
    }
  }
}

void EntityManager::placeStoneEntities(
    const irr::core::vector3df &position, const std::vector<int> &quantities,
    const std::vector<std::vector<irr::io::path>> &stoneTextures,
    const std::vector<irr::io::path> &qB3D,
    const std::vector<std::string> &stoneNames,
    const std::vector<irr::core::vector3df> &qScale) {
  int numStones = 0;
  for (size_t i = 0; i < quantities.size(); ++i)
    if (quantities[i] > 0)
      ++numStones;
  if (numStones == 0)
    return;
  float radius = 6.0f;
  float angleStep = 2.0f * M_PI / numStones;
  int placed = 0;
  for (size_t i = 0; i < quantities.size(); ++i) {
    if (quantities[i] > 0) {
      float angle = placed * angleStep;
      irr::core::vector3df objPos = position;
      objPos.X += std::cos(angle) * radius;
      objPos.Z += std::sin(angle) * radius;
      entity_.push_back(std::make_shared<Stone>(
          -2, objPos, qScale[i], stoneTextures[i], qB3D[i], stoneNames[i]));
      entity_.back()->createNode(smgr_, driver_);
      ++placed;
    }
  }
}

void EntityManager::createDroppedResource(int x, int y,
                                          const std::string &item) {
  std::map<std::string, std::vector<irr::io::path>> resourceTextures = {
      {"food",
       {mediaPath_ + "stone_texture/food_redbull.png",
        mediaPath_ + "stone_texture/food_redbull.png"}},
      {"linemate", {mediaPath_ + "stone_texture/stone_red.png"}},
      {"deraumere", {mediaPath_ + "stone_texture/stone_orange.png"}},
      {"sibur", {mediaPath_ + "stone_texture/stone_yellow.png"}},
      {"mendiane", {mediaPath_ + "stone_texture/stone_green.png"}},
      {"phiras", {mediaPath_ + "stone_texture/stone_blue.png"}},
      {"thystame", {mediaPath_ + "stone_texture/stone_purple.png"}}};
  std::map<std::string, irr::io::path> resourceB3D = {
      {"food", mediaPath_ + "RedBull.b3d"},
      {"linemate", mediaPath_ + "ruby.b3d"},
      {"deraumere", mediaPath_ + "ruby.b3d"},
      {"sibur", mediaPath_ + "ruby.b3d"},
      {"mendiane", mediaPath_ + "ruby.b3d"},
      {"phiras", mediaPath_ + "ruby.b3d"},
      {"thystame", mediaPath_ + "ruby.b3d"}};
  std::map<std::string, irr::core::vector3df> resourceScale = {
      {"food", irr::core::vector3df(0.8f, 0.8f, 0.8f)},
      {"linemate", irr::core::vector3df(0.009f, 0.009f, 0.009f)},
      {"deraumere", irr::core::vector3df(0.009f, 0.009f, 0.009f)},
      {"sibur", irr::core::vector3df(0.009f, 0.009f, 0.009f)},
      {"mendiane", irr::core::vector3df(0.009f, 0.009f, 0.009f)},
      {"phiras", irr::core::vector3df(0.009f, 0.009f, 0.009f)},
      {"thystame", irr::core::vector3df(0.009f, 0.009f, 0.009f)}};

  if (resourceTextures.find(item) == resourceTextures.end())
    return;

  auto tile = getTileByName("Cube info: row " + std::to_string(x) + " col " +
                            std::to_string(y));
  std::vector<std::string> stoneNames = {"food",    "linemate", "deraumere",
                                         "sibur",   "mendiane", "phiras",
                                         "thystame"};
  std::vector<int> quantities;
  for (const auto &name : stoneNames) {
    int qty = static_cast<int>(tile->getInventory().getItemQuantity(name));
    quantities.push_back(qty);
  }
  irr::core::vector3df position(x * 20.0f, 5.0f, y * 20.0f);
  std::vector<std::vector<irr::io::path>> stoneTextures;
  std::vector<irr::io::path> qB3D;
  std::vector<irr::core::vector3df> qScale;
  for (const auto &name : stoneNames) {
    if (!resourceTextures[name].empty())
      stoneTextures.push_back(resourceTextures[name]);
    else
      stoneTextures.push_back({});
    qB3D.push_back(resourceB3D[name]);
    qScale.push_back(resourceScale[name]);
  }
  float radius = 6.0f;

  auto it = entity_.begin();
  while (it != entity_.end()) {
    if ((*it)->getName() != "Stone") {
      ++it;
      continue;
    }
    float distance =
        std::sqrt(std::pow((*it)->getPosition().X - position.X, 2) +
                  std::pow((*it)->getPosition().Z - position.Z, 2));
    if (distance <= radius + 0.01f) {
      if ((*it)->getNode())
        (*it)->getNode()->remove();
      it = entity_.erase(it);
    } else {
      ++it;
    }
  }

  placeStoneEntities(position, quantities, stoneTextures, qB3D, stoneNames,
                     qScale);
}

void EntityManager::updateMapText(irr::gui::IGUIStaticText *textMap) {
  if (!textMap)
    return;
  irr::core::stringw mapText = L"Map Info:\n";
  std::map<std::string, int> resourceCount;
  std::vector<std::string> resources = {
      "food", "linemate", "deraumere", "sibur", "mendiane", "phiras", "thystame", "egg"};
  for (const auto &entity : tiles_) {
    for (const auto &resource : resources) {
      resourceCount[resource] += entity->getInventory().getItemQuantity(resource);
    }
  }
  for (const auto &entity : entity_) {
    if (entity->getName() == "Egg") {
      resourceCount["egg"] += 1;
    }
  }
  std::vector<std::pair<std::string, int>> sortedResources;
  for (const auto &resource : resources) {
    sortedResources.emplace_back(resource, resourceCount[resource]);
  }
  for (const auto &pair : sortedResources) {
    mapText += irr::core::stringw(pair.first.c_str());
    mapText += L": ";
    mapText += irr::core::stringw(std::to_wstring(pair.second).c_str());
    mapText += L"\n";
  }
  textMap->setText(mapText.c_str());
}