/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** WorldScene
*/

#include "WorldScene.hpp"
#include <cmath>
#include <cstdlib>
#include <map>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void WorldScene::createEntities(int id, int x, int y, Direction direction,
                                int level, std::string team) {
  entityManager_.addTeams(teams_);
  auto tile = entityManager_.getTileByName(
      "Cube info: row " + std::to_string(x) + " col " + std::to_string(y));
  if (!tile) {
    return;
  }
  irr::core::vector3df pos = tile->getPosition();
  entityManager_.createPlayers(id, pos.X, pos.Z, direction, level, team);
  entity_ = entityManager_.getEntities();
  receiver_.addEntity(entity_.back());
  addChatMessage(
      "Player " + std::to_string(id) + " created at (" + std::to_string(x) +
      ", " + std::to_string(y) + ") with direction " +
      std::to_string(static_cast<int>(direction)) + " and team " + team);
}

void WorldScene::setPlayerInventory(int id, int x, int y, int q0, int q1,
                                    int q2, int q3, int q4, int q5, int q6) {
  for (auto &entity : entity_) {
    if (entity->getId() == id) {
      std::vector<std::pair<std::string, int>> items = {
          {"food", q0},     {"linemate", q1}, {"deraumere", q2}, {"sibur", q3},
          {"mendiane", q4}, {"phiras", q5},   {"thystame", q6}};
      for (const auto &item : items) {
        entity->getInventory().addItem(item.first, item.second);
      }
      return;
    }
  }
}

void WorldScene::createEntities(int x, int y, int q0, int q1, int q2, int q3,
                                int q4, int q5, int q6, int nbTiles) {
  int countTiles = 0;
  for (int i = 0; i < x; i++) {
    for (int j = 0; j < y; j++) {
      entityManager_.createStones(i, j, q0, q1, q2, q3, q4, q5, q6);
      countTiles++;
      if (countTiles >= nbTiles) {
        return;
      }
    }
  }
  entity_ = entityManager_.getEntities();
}

void WorldScene::createEntities(int x, int y, int q0, int q1, int q2, int q3,
                                int q4, int q5, int q6) {
  entityManager_.createStones(x, y, q0, q1, q2, q3, q4, q5, q6);
  entity_ = entityManager_.getEntities();
}

void WorldScene::changePlayerPos(int id, int x, int y, Direction direction) {
  Movement movement = {id, x, y, direction};
  movementQueue_.push(movement);
  if (!receiver_.getIsMoving()) {
    updateMovements();
  }
}

EdgePositionResult
WorldScene::isEdgePosition(const irr::core::vector3df &position,
                           Direction direction, int nextX, int nextY) {
  EdgePositionResult result = {false, 0.0f, 0.0f, irr::core::vector3df()};

  result.isEdge =
      (position.Z == 0 && direction == Direction::NORTH) ||
      (position.X == 0 && direction == Direction::EAST) ||
      (position.Z == planeSize_.second - 1 && direction == Direction::SOUTH) ||
      (position.X == planeSize_.first - 1 && direction == Direction::WEST);

  if (result.isEdge) {
    auto nextTile =
        entityManager_.getTileByName("Cube info: row " + std::to_string(nextX) +
                                     " col " + std::to_string(nextY));
    if (nextTile) {
      result.nextPosition = nextTile->getPosition();
      switch (direction) {
      case Direction::NORTH:
        result.offsetZ = 20.0f;
        break;
      case Direction::EAST:
        result.offsetX = 20.0f;
        break;
      case Direction::SOUTH:
        result.offsetZ = -20.0f;
        break;
      case Direction::WEST:
        result.offsetX = -20.0f;
        break;
      default:
        break;
      }
    }
  }
  return result;
}

void WorldScene::updateMovements() {
  if (movementQueue_.empty() || receiver_.getIsMoving())
    return;

  Movement movement = movementQueue_.front();
  movementQueue_.pop();

  for (auto &entity : entity_) {
    if (entity->getId() == movement.id) {
      auto *node = entity->getNode();
      receiver_.setCurrentEntityId(movement.id);

      auto edgeResult = isEdgePosition(actualPos_, movement.direction,
                                       movement.x, movement.y);
      if (edgeResult.isEdge) {
        node->setPosition(irr::core::vector3df(
            edgeResult.nextPosition.X + edgeResult.offsetX,
            node->getPosition().Y,
            edgeResult.nextPosition.Z + edgeResult.offsetZ));
      }

      actualPos_ = irr::core::vector3df(movement.x, 0, movement.y);
      node->setRotation(irr::core::vector3df(
          0, static_cast<float>(movement.direction) * 90.0f, 0));
      if (auto *animatedNode =
              dynamic_cast<irr::scene::IAnimatedMeshSceneNode *>(node)) {
        animatedNode->setFrameLoop(1, 13);
        animatedNode->setAnimationSpeed(15.0f);
        receiver_.setMoveStartX(node->getPosition().X);
        receiver_.setMoveStartZ(node->getPosition().Z);
        receiver_.setCurrentRotationY(static_cast<float>(movement.direction) *
                                      90.0f);
        receiver_.setIsMoving(true);
        receiver_.setMoveStartTime(device_->getTimer()->getTime());
      }
      return;
    }
  }
}

void WorldScene::createLights() {
  smgr_->addLightSceneNode(0, irr::core::vector3df(0, 100, -100),
                           irr::video::SColorf(1.0f, 1.0f, 1.0f), 800.0f);
  smgr_->addLightSceneNode(0, irr::core::vector3df(-100, 100, -100),
                           irr::video::SColorf(0.6f, 0.6f, 0.6f), 800.0f);
  smgr_->addLightSceneNode(0, irr::core::vector3df(100, 100, -100),
                           irr::video::SColorf(0.6f, 0.6f, 0.6f), 800.0f);
}

void WorldScene::createCamera() {
  smgr_->addCameraSceneNode(0, irr::core::vector3df(0, 50, -100),
                            irr::core::vector3df(0, 30, 0));
}

void WorldScene::createPlane(int x, int y) {
  planeSize_ = {x, y};
  entityManager_.createTiles(x, y);
  entity_ = entityManager_.getEntities();
}

void WorldScene::createText() {
  irr::gui::IGUIStaticText *text = smgr_->getGUIEnvironment()->addStaticText(
      L"Tile Info:", irr::core::rect<irr::s32>(10, 10, 220, 200), false);

  irr::gui::IGUIStaticText *playerText =
      smgr_->getGUIEnvironment()->addStaticText(
          L"Player Info:",
          irr::core::rect<irr::s32>(
              smgr_->getVideoDriver()->getScreenSize().Width - 120, 10,
              smgr_->getVideoDriver()->getScreenSize().Width - 10, 200),
          false);

  irr::core::dimension2du screenSize = smgr_->getVideoDriver()->getScreenSize();
  textChat_ = smgr_->getGUIEnvironment()->addStaticText(
      L"Chat:\n",
      irr::core::rect<irr::s32>(10, 700, 400, screenSize.Height - 10), false);

  irr::gui::IGUIFont *font = smgr_->getGUIEnvironment()->getFont(
      mediaPath_ + "fonthaettenschweiler.bmp");
  if (font) {
    text->setOverrideFont(font);
    playerText->setOverrideFont(font);
    textChat_->setOverrideFont(font);
  }

  receiver_.setText(text);
  receiver_.setPlayerText(playerText);
}

void WorldScene::setPlayerLevel(int id, int level) {
  for (auto &entity : entity_) {
    if (entity->getId() == id) {
      entity->setLevel(level);
      changeHeadLevel(entity, level);
      return;
    }
  }
}

void WorldScene::changeHeadLevel(std::shared_ptr<IEntity> &entity, int level) {

  std::vector<irr::io::path> texturesHead = {
      mediaPath_ + "archer_texture/santi_head.png",
      mediaPath_ + "archer_texture/santi_head.png",
      mediaPath_ + "archer_texture/eliott_head.png",
      mediaPath_ + "archer_texture/pierrick_head.png",
      mediaPath_ + "archer_texture/matheo_head.png",
      mediaPath_ + "archer_texture/tom_head.png",
      mediaPath_ + "archer_texture/hugo_head.png",
      mediaPath_ + "archer_texture/napoli_head.png",
      mediaPath_ + "archer_texture/ay_head.png"};

  entity->getNode()->getMaterial(3).setTexture(
      0, driver_->getTexture(texturesHead[level]));
}

void WorldScene::startIncantation(int x, int y, int level,
                                  std::vector<int> ids) {
  for (int id : ids) {
    for (auto &entity : entity_) {
      if (entity->getId() == id) {
        incantationData_.push_back(std::make_tuple(x, y, id));
        isIncanting_[id] = true;
        addChatMessage("Player " + std::to_string(id) + " is incanting at (" +
                       std::to_string(x) + ", " + std::to_string(y) + ")");
        break;
      }
    }
  }
}

void WorldScene::stopIncantation(int x, int y, bool result) {
  for (auto it = incantationData_.begin(); it != incantationData_.end();) {
    if (std::get<0>(*it) == x && std::get<1>(*it) == y) {
      int entityId = std::get<2>(*it);
      isIncanting_[entityId] = false;
      it = incantationData_.erase(it);
      if (result) {
        for (auto &entity : entity_) {
          if (entity->getId() == entityId) {
            entity->setLevel(entity->getLevel() + 1);
            addChatMessage(
                "Player " + std::to_string(entityId) + " has leveled up to " +
                std::to_string(entity->getLevel()) +
                " (result: " + std::string(result ? "true" : "false") + ")");
            break;
          }
        }
      }
    } else {
      ++it;
    }
  }
}

void WorldScene::killPlayer(int id) {
  for (auto it = entity_.begin(); it != entity_.end();) {
    if ((*it)->getId() == id) {
      auto player = std::dynamic_pointer_cast<PlayerEntity>(*it);
      if (player)
        player->getNode()->remove();
      it = entity_.erase(it);
      receiver_.removeEntity(id);
      addChatMessage("Player " + std::to_string(id) + " has been killed.");
    } else {
      ++it;
    }
  }
}

void WorldScene::addChatMessage(const std::string &message) {
  std::string fullMessage = "Action: " + message;
  chatMessages_.push_back(fullMessage);

  if (chatMessages_.size() > MAX_CHAT_MESSAGES) {
    chatMessages_.erase(chatMessages_.begin());
  }

  updateChatDisplay();
}

void WorldScene::broadcast(int id, const std::string &message) {
  std::shared_ptr<IEntity> broadcaster = nullptr;
  for (const auto &entity : entity_) {
    if (entity->getId() == id) {
      broadcaster = entity;
      break;
    }
  }
  if (!broadcaster)
    return;

  std::string fullMessage = "Player " + std::to_string(id) + ": " + message;
  addChatMessage(fullMessage);

  irr::core::vector3df broadcasterPos = broadcaster->getNode()->getPosition();
  irr::u32 currentTime = device_->getTimer()->getTime();

  for (const auto &entity : entity_) {
    if (entity->getId() == id || entity->getId() < 0)
      continue;

    auto paperPlane = entityManager_.createPaperPlane(broadcasterPos);
    if (paperPlane) {
      irr::core::vector3df targetPos = entity->getNode()->getPosition();
      PaperPlaneMovement movement;
      movement.paperPlane = paperPlane;
      movement.targetPlayerId = entity->getId();
      movement.startPosition = broadcasterPos;
      movement.targetPosition = targetPos;
      movement.startTime = currentTime;
      movement.duration = 1.5f;
      movement.isActive = true;
      paperPlaneMovements_.push_back(movement);
    }
  }
  entity_ = entityManager_.getEntities();
}

void WorldScene::resourceDroping(int id, const std::string &item) {
  for (auto &entity : entity_) {
    if (entity->getId() == id) {
      entity->getInventory().removeItem(item, 1);
      int tileX = static_cast<int>(entity->getPosition().X / 20);
      int tileZ = static_cast<int>(entity->getPosition().Z / 20);
      auto tile = entityManager_.getTileByName("Cube info: row " +
                                               std::to_string(tileX) + " col " +
                                               std::to_string(tileZ));
      if (!tile)
        return;
      tile->getInventory().addItem(item, 1);
      createDroppedResource(tileX, tileZ, item);
      return;
    }
  }
}

void WorldScene::createDroppedResource(int x, int y, const std::string &item) {
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

  irr::core::vector3df position(x * 20.0f, 5.0f, y * 20.0f);
  float randomOffsetX = (rand() % 100 - 50) / 50.0f;
  float randomOffsetZ = (rand() % 100 - 50) / 50.0f;
  position.X += randomOffsetX * 3.0f;
  position.Z += randomOffsetZ * 3.0f;
  entity_.push_back(std::make_shared<Stone>(-1, position, resourceScale[item],
                                            resourceTextures[item],
                                            resourceB3D[item], item));
  entity_.back()->createNode(smgr_, driver_);
}

void WorldScene::createWorld() {}

void WorldScene::updateChatDisplay() {
  if (!textChat_)
    return;

  irr::core::stringw chatText = L"Chat:\n";
  for (const auto &message : chatMessages_) {
    chatText += irr::core::stringw(message.c_str());
    chatText += L"\n";
  }
  textChat_->setText(chatText.c_str());
}

void WorldScene::updatePaperPlaneMovements() {
  if (paperPlaneMovements_.empty())
    return;
  irr::u32 currentTime = device_->getTimer()->getTime();
  for (auto it = paperPlaneMovements_.begin();
       it != paperPlaneMovements_.end();) {
    if (!it->isActive) {
      ++it;
      continue;
    }

    float elapsedTime = (currentTime - it->startTime) / 1000.0f;
    if (elapsedTime >= it->duration) {
      auto paperPlane = it->paperPlane;
      if (paperPlane && paperPlane->getNode())
        paperPlane->getNode()->remove();
      for (auto entityIt = entity_.begin(); entityIt != entity_.end();
           ++entityIt) {
        if (*entityIt == paperPlane) {
          entity_.erase(entityIt);
          break;
        }
      }
      it = paperPlaneMovements_.erase(it);
    } else {
      float progress = elapsedTime / it->duration;
      progress = std::min(1.0f, std::max(0.0f, progress));
      irr::core::vector3df currentPos = it->startPosition.getInterpolated(
          it->targetPosition, 1.0f - progress);
      float arcHeight = 15.0f;
      float arcProgress = sin(progress * M_PI);
      currentPos.Y += arcHeight * arcProgress;
      if (it->paperPlane && it->paperPlane->getNode()) {
        it->paperPlane->getNode()->setPosition(currentPos);
        irr::core::vector3df direction =
            (it->targetPosition - it->startPosition).normalize();
        float yaw = atan2(direction.X, direction.Z) * 180.0f / M_PI;
        it->paperPlane->getNode()->setRotation(irr::core::vector3df(0, yaw, 0));
      }
      ++it;
    }
  }
}
