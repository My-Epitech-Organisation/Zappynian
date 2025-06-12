/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** WorldScene
*/

#include "WorldScene.hpp"

void WorldScene::createEntities(int id, int x, int y, Direction direction,
                                int level, std::string team) {
  auto tile = entityManager_.getTileByName(
      "Cube info: row " + std::to_string(x) + " col " + std::to_string(y));
  if (!tile) {
    return;
  }
  irr::core::vector3df pos = tile->getPosition();
  entityManager_.createPlayers(id, pos.X, pos.Z, direction, level, team);
  entity_ = entityManager_.getEntities();
  receiver_.addEntity(entity_.back());
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

  irr::gui::IGUIFont *font = smgr_->getGUIEnvironment()->getFont(
      mediaPath_ + "fonthaettenschweiler.bmp");
  if (font) {
    text->setOverrideFont(font);
    playerText->setOverrideFont(font);
  }

  receiver_.setText(text);
  receiver_.setPlayerText(playerText);
}

void WorldScene::setPlayerLevel(int id, int level) {
  for (auto &entity : entity_) {
    if (entity->getId() == id) {
      entity->setLevel(level);
      return;
    }
  }
}

void WorldScene::startIncantation(int x, int y, int level,
                                  std::vector<int> ids) {
  for (int id : ids) {
    for (auto &entity : entity_) {
      if (entity->getId() == id) {
        incantationData_.push_back(std::make_tuple(x, y, id));
        isIncanting_[id] = true;
        break;
      }
    }
  }
}

void WorldScene::stopIncantation(int x, int y, bool result) {
  for (auto it = incantationData_.begin(); it != incantationData_.end();) {
    if (std::get<0>(*it) == x && std::get<1>(*it) == y) {
      isIncanting_[std::get<2>(*it)] = false;
      it = incantationData_.erase(it);
      if (result) {
        for (auto &entity : entity_) {
          if (entity->getId() == std::get<2>(*it)) {
            entity->setLevel(entity->getLevel() + 1);
            break;
          }
        }
      }
    } else {
      ++it;
    }
  }
}

void WorldScene::createWorld() {}
