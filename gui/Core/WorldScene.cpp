/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** WorldScene
*/

#include "WorldScene.hpp"
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <map>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void WorldScene::createEntities(int id, int x, int y, Direction direction,
                                int level, std::string team) {
  if (!smgr_ || !driver_ || !device_) {
    std::cerr << "Error: Uninitialized dependencies detected. "
              << "smgr_: " << (smgr_ ? "initialized" : "uninitialized") << ", "
              << "driver_: " << (driver_ ? "initialized" : "uninitialized")
              << ", "
              << "device_: " << (device_ ? "initialized" : "uninitialized")
              << std::endl;
    return;
  }
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
  teamStates_[team].first++;
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
  if (!smgr_ || !driver_ || !device_)
    return;
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
  if (!smgr_ || !driver_ || !device_)
    return;
  entityManager_.createStones(x, y, q0, q1, q2, q3, q4, q5, q6);
  entity_ = entityManager_.getEntities();
}

void WorldScene::createEntities(int id) {
  if (!smgr_ || !driver_ || !device_)
    return;
  entityManager_.createEgg(id);
  entity_ = entityManager_.getEntities();
}

void WorldScene::changePlayerPos(int id, int x, int y, Direction direction,
                                 Direction directionBefore) {
  Movement movement = {id, x, y, direction, directionBefore};
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
        animatedNode->setAnimationSpeed(30.0f);
        receiver_.setMoveStartX(node->getPosition().X);
        receiver_.setMoveStartZ(node->getPosition().Z);
        receiver_.setCurrentRotationY(static_cast<float>(movement.direction) *
                                      90.0f);
        receiver_.setIsMoving(true);
        receiver_.setMoveStartTime(device_->getTimer()->getTime());
      }
      node->setRotation(irr::core::vector3df(
          0, static_cast<float>(movement.directionBefore) * 90.0f, 0));
      return;
    }
  }
}

void WorldScene::createLights() {
  if (!smgr_)
    return;
  smgr_->addLightSceneNode(0, irr::core::vector3df(0, 100, -100),
                           irr::video::SColorf(1.0f, 1.0f, 1.0f), 800.0f);
  smgr_->addLightSceneNode(0, irr::core::vector3df(-100, 100, -100),
                           irr::video::SColorf(0.6f, 0.6f, 0.6f), 800.0f);
  smgr_->addLightSceneNode(0, irr::core::vector3df(100, 100, -100),
                           irr::video::SColorf(0.6f, 0.6f, 0.6f), 800.0f);
}

void WorldScene::createCamera() {
  if (!smgr_)
    return;
  smgr_->addCameraSceneNode(0, irr::core::vector3df(0, 50, -100),
                            irr::core::vector3df(0, 30, 0));
}

void WorldScene::createPlane(int x, int y) {
  if (!smgr_ || !driver_ || !device_)
    return;
  planeSize_ = {x, y};
  entityManager_.createTiles(x, y);
  entity_ = entityManager_.getEntities();
}

void WorldScene::createText() {
  if (!smgr_ || !driver_)
    return;
  irr::gui::IGUIStaticText *text = smgr_->getGUIEnvironment()->addStaticText(
      L"Tile Info:", irr::core::rect<irr::s32>(10, 10, 220, 150), false);
  text->setDrawBorder(true);

  irr::gui::IGUIStaticText *playerText =
      smgr_->getGUIEnvironment()->addStaticText(
          L"Player Info:",
          irr::core::rect<irr::s32>(
              smgr_->getVideoDriver()->getScreenSize().Width - 120, 10,
              smgr_->getVideoDriver()->getScreenSize().Width - 10, 180),
          false);
  playerText->setDrawBorder(true);

  textMap_ = smgr_->getGUIEnvironment()->addStaticText(
      L"Map Info:\n", irr::core::rect<irr::s32>(10, 220, 220, 640), false);
  textMap_->setDrawBorder(true);

  irr::core::dimension2du screenSize = smgr_->getVideoDriver()->getScreenSize();
  textChat_ = smgr_->getGUIEnvironment()->addStaticText(
      L"Chat:\n",
      irr::core::rect<irr::s32>(10, 700, 400, screenSize.Height - 10), false);
  textChat_->setDrawBorder(true);

  irr::gui::IGUIFont *font = smgr_->getGUIEnvironment()->getFont(
      mediaPath_ + "fonthaettenschweiler.bmp");
  if (font) {
    text->setOverrideColor(irr::video::SColor(255, 0, 0, 0));
    playerText->setOverrideColor(irr::video::SColor(255, 0, 0, 0));
    textChat_->setOverrideColor(irr::video::SColor(255, 0, 0, 0));
    textMap_->setOverrideColor(irr::video::SColor(255, 0, 0, 0));

    text->setBackgroundColor(irr::video::SColor(120, 211, 211, 211));
    playerText->setBackgroundColor(irr::video::SColor(120, 211, 211, 211));
    textChat_->setBackgroundColor(irr::video::SColor(120, 211, 211, 211));
    textMap_->setBackgroundColor(irr::video::SColor(120, 211, 211, 211));

    text->setOverrideFont(font);
    playerText->setOverrideFont(font);
    textChat_->setOverrideFont(font);
    textMap_->setOverrideFont(font);
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
  if (!driver_)
    return;
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
            setPlayerLevel(entityId, entity->getLevel());
            break;
          }
        }
      } else {
        addChatMessage("Incantation at (" + std::to_string(x) + ", " +
                       std::to_string(y) + ") failed for player " +
                       std::to_string(entityId));
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
      teamStates_[player->getTeam()].first--;
      teamStates_[player->getTeam()].second++;
    } else {
      ++it;
    }
  }
}

void WorldScene::killEgg(int id) {
  for (auto it = entity_.begin(); it != entity_.end();) {
    if ((*it)->getId() == -7) {
      auto egg = std::dynamic_pointer_cast<Egg>(*it);
      if (egg)
        egg->getNode()->remove();
      it = entity_.erase(it);
      receiver_.removeEntity(id);
      addChatMessage("Egg " + std::to_string(id) + " has been killed.");
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
  if (!device_)
    return;
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
      movement.duration = 1.7f;
      movement.isActive = true;
      paperPlaneMovements_.push_back(movement);
    }
  }
  entity_ = entityManager_.getEntities();
}

void WorldScene::resourceDropping(int id, const std::string &item) {
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
      entityManager_.createDroppedResource(tileX, tileZ, item);
      entity_ = entityManager_.getEntities();
      addChatMessage("Player " + std::to_string(id) + " dropped " + item +
                     " at (" + std::to_string(tileX) + ", " +
                     std::to_string(tileZ) + ")");
      return;
    }
  }
}

void WorldScene::resourceCollect(int id, const std::string &item) {
  for (auto &entity : entity_) {
    if (entity->getId() == id) {
      int tileX = static_cast<int>(entity->getPosition().X / 20);
      int tileZ = static_cast<int>(entity->getPosition().Z / 20);
      auto tile = entityManager_.getTileByName("Cube info: row " +
                                               std::to_string(tileX) + " col " +
                                               std::to_string(tileZ));
      if (!tile)
        return;
      if (tile->getInventory().getItemQuantity(item) == 0)
        return;
      tile->getInventory().removeItem(item, 1);
      entity->getInventory().addItem(item, 1);
      entityManager_.createDroppedResource(tileX, tileZ, item);
      entity_ = entityManager_.getEntities();
      addChatMessage("Player " + std::to_string(id) + " collected " + item +
                     " at (" + std::to_string(tileX) + ", " +
                     std::to_string(tileZ) + ")");
      return;
    }
  }
}

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
  if (!device_)
    return;
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
      float arcHeight = 30.0f;
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

void WorldScene::clearElements() {
  if (!device_)
    return;
  std::queue<Movement> emptyQueue;
  std::swap(movementQueue_, emptyQueue);
  for (auto &plane : paperPlaneMovements_)
    if (plane.paperPlane && plane.paperPlane->getNode())
      plane.paperPlane->getNode()->remove();
  paperPlaneMovements_.clear();
  actualPos_ = irr::core::vector3df();
  planeSize_ = {0, 0};
  isIncanting_.clear();
  incantationData_.clear();
  teams_.clear();
  receiver_.clearAllEntities();
  for (auto &entity : entity_) {
    if (!entity || !entity->getNode())
      continue;
    if (entity->getId() == -5 || entity->getId() == -6)
      continue;
    entity->getNode()->setVisible(false);
  }
}

void WorldScene::endGame(std::string winner) {
  if (!smgr_ || !driver_)
    return;
  clearElements();
  addChatMessage("Game Over! Winner: " + winner);
  irr::core::dimension2du screenSize = smgr_->getVideoDriver()->getScreenSize();
  int buttonWidth = 300;
  int buttonHeight = 100;
  int centerX = (screenSize.Width - buttonWidth) / 2;
  int centerY = (screenSize.Height - buttonHeight) / 2;
  std::wstring winnerW =
      L"Winner: " + std::wstring(winner.begin(), winner.end()) + L"\nQuit";
  irr::gui::IGUIButton *quitButton = smgr_->getGUIEnvironment()->addButton(
      irr::core::rect<irr::s32>(centerX, centerY, centerX + buttonWidth,
                                centerY + buttonHeight),
      0, -1, winnerW.c_str());

  quitButton->setOverrideFont(smgr_->getGUIEnvironment()->getFont(
      mediaPath_ + "fonthaettenschweiler.bmp"));
  quitButton->setUseAlphaChannel(true);
  quitButton->setDrawBorder(true);
  quitButton->setIsPushButton(true);
  quitButton->setPressed(false);
  quitButton->setID(9999);
}

std::vector<std::shared_ptr<IEntity>>
WorldScene::getPlayersEggAtPosition(irr::core::vector3df position) {
  std::vector<std::shared_ptr<IEntity>> entitiesAtPosition;
  for (const auto &entity : entity_) {
    if (entity->getId() < 0 && entity->getId() != -7)
      continue;
    if (entity->getPosition() == position) {
      entitiesAtPosition.push_back(entity);
    }
  }
  return entitiesAtPosition;
}

void WorldScene::expulsionMove(int id, irr::f32 direction,
                               Direction directionEnum,
                               irr::core::vector3df pos) {
  auto expulsion = getPlayersEggAtPosition(pos);
  for (auto &entity : expulsion) {
    if (entity->getId() != id) {
      if (entity->getId() == -7) {
        irr::core::vector3df eggPos = entity->getPosition();
        auto tile = entityManager_.getTileByName(
            "Cube info: row " +
            std::to_string(static_cast<int>(eggPos.X / 20)) + " col " +
            std::to_string(static_cast<int>(eggPos.Z / 20)));
        if (tile)
          tile->getInventory().removeItem("egg", 1);
        if (entity->getNode())
          entity->getNode()->remove();
        receiver_.removeEntity(-7);
        continue;
      }
      auto actualDir = entity->getDirection();
      changePlayerPos(
          entity->getId(),
          static_cast<int>(pos.X - 20 * sin(direction * M_PI / 180)),
          static_cast<int>(pos.Z - 20 * cos(direction * M_PI / 180)),
          directionEnum, actualDir);
    }
  }
}

void WorldScene::expulsion(int id) {
  for (auto it = entity_.begin(); it != entity_.end();) {
    if ((*it)->getId() < 0) {
      ++it;
      continue;
    }
    if ((*it)->getId() == id) {
      auto direction = (*it)->getNode()->getRotation().Y;
      auto directionEnum =
          static_cast<Direction>(static_cast<int>(direction / 90) % 4);
      irr::core::vector3df pos = (*it)->getNode()->getPosition();
      expulsionMove(id, direction, directionEnum, pos);
      break;
    } else {
      ++it;
    }
  }
}

void WorldScene::updateMapText() {
  entityManager_.updateMapText(textMap_);
  irr::core::stringw mapText = textMap_->getText();
  int playerCount = 0;
  for (const auto &entity : entity_) {
    if (entity->getId() < 0)
      continue;
    playerCount++;
  }
  mapText += irr::core::stringw(L"\nTotal Players: ") + irr::core::stringw(playerCount) + L"\n\n";
  std::vector<std::string> levelTeams = {"0", "0", "0", "0", "0", "0", "0", "0", "0"};
  for (const auto &teamState : teamStates_) {
    mapText += irr::core::stringw(teamState.first.c_str()) + L": " +
               irr::core::stringw(teamState.second.first) + L" alive, " +
               irr::core::stringw(teamState.second.second) + L" dead\n";
    mapText += L"Levels :  ";
    for (const auto &entity : entity_) {
      if (entity->getId() < 0)
        continue;
      auto player = std::dynamic_pointer_cast<PlayerEntity>(entity);
      if (player && player->getTeam() == teamState.first) {
        levelTeams[player->getLevel()] =
            std::to_string(std::stoi(levelTeams[player->getLevel()]) + 1);
      }
    }
    for (size_t i = 0; i < levelTeams.size(); ++i) {
      mapText += irr::core::stringw(levelTeams[i].c_str()) + L" ";
      if (i < levelTeams.size() - 1)
        mapText += L"- ";
    }
    mapText += L"\n\n";
    levelTeams = {"0", "0", "0", "0", "0", "0", "0", "0", "0"};
  }
  textMap_->setText(mapText.c_str());
}
