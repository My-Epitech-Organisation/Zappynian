/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** Game
*/

#include "Game.hpp"

Game::Game() { initWindow(); }

Game::~Game() {}

std::shared_ptr<IEntity> Game::findEntityById(int id) {
  for (auto &entity : entity_) {
    if (entity && entity->getId() == id) {
      return entity;
    }
  }
  return nullptr;
}

void Game::initWindow() {
  device_ = irr::createDevice(irr::video::EDT_OPENGL,
                              irr::core::dimension2d<irr::u32>(1920, 1080), 16,
                              false, true, false, &receiver_);
  if (!device_)
    return;
  driver_ = device_->getVideoDriver();
  smgr_ = device_->getSceneManager();
  guienv_ = device_->getGUIEnvironment();
  receiver_.setDevice(device_);
  mediaPath_ = "assets/";
  device_->setWindowCaption(L"Zappy FPS: 0");

  driver_->setTextureCreationFlag(irr::video::ETCF_CREATE_MIP_MAPS, false);

  smgr_->addSkyBoxSceneNode(
      driver_->getTexture(mediaPath_ + "sky_texture/skyup.png"),
      driver_->getTexture(mediaPath_ + "sky_texture/skydown.png"),
      driver_->getTexture(mediaPath_ + "sky_texture/skymid.png"),
      driver_->getTexture(mediaPath_ + "sky_texture/skymidright.png"),
      driver_->getTexture(mediaPath_ + "sky_texture/skymidleft.png"),
      driver_->getTexture(mediaPath_ + "sky_texture/skyleft.png"));

  driver_->setTextureCreationFlag(irr::video::ETCF_CREATE_MIP_MAPS, true);
}

void Game::updatePlayerMovement(irr::u32 currentTime, NetworkClient &scene) {
  if (!receiver_.getIsMoving())
    return;

  auto currentEntity = findEntityById(receiver_.getCurrentEntityId());
  if (!currentEntity) {
    receiver_.setIsMoving(false);
    return;
  }

  float elapsedTime = (currentTime - receiver_.getMoveStartTime()) / 1000.0f;
  auto *node = currentEntity->getNode();
  if (!node || !node->getParent()) {
    receiver_.setIsMoving(false);
    return;
  }

  if (elapsedTime >= 0.4f) {
    irr::core::vector3df pos;
    float angle = receiver_.getCurrentRotationY() * M_PI / 180.0f;
    pos.X = receiver_.getMoveStartX() - 20.0f * sin(angle);
    pos.Z = receiver_.getMoveStartZ() - 20.0f * cos(angle);
    try {
      pos.Y = node->getPosition().Y;
    } catch (...) {
      receiver_.setIsMoving(false);
      return;
    }
    node->setPosition(pos);
    receiver_.setIsMoving(false);
    if (auto *animatedNode =
            dynamic_cast<irr::scene::IAnimatedMeshSceneNode *>(node))
      animatedNode->setAnimationSpeed(0.0f);
    scene.updateMovements();
  } else {
    float progress = elapsedTime / 0.4f;
    irr::core::vector3df pos;
    float angle = receiver_.getCurrentRotationY() * M_PI / 180.0f;
    pos.X = receiver_.getMoveStartX() - (20.0f * progress * sin(angle));
    pos.Z = receiver_.getMoveStartZ() - (20.0f * progress * cos(angle));
    try {
      pos.Y = node->getPosition().Y;
      node->setPosition(pos);
    } catch (...) {
      receiver_.setIsMoving(false);
      return;
    }
  }
}

void Game::updateIncantingPlayers(NetworkClient &scene) {
  for (auto &entity : entity_) {
    if (!entity || !entity->getNode()) {
      continue;
    }
    if (scene.isPlayerIncanting(entity->getId())) {
      if (auto *node = dynamic_cast<irr::scene::IAnimatedMeshSceneNode *>(
              entity->getNode())) {
        irr::core::vector3df rotation = node->getRotation();
        rotation.Y += 15.0f;
        if (rotation.Y >= 360.0f)
          rotation.Y = 0.0f;
        node->setRotation(rotation);
      }
    }
  }
}

void Game::gameLoop() {
  irr::u32 frames = 0;

  NetworkClient scene(device_, smgr_, driver_, receiver_, mediaPath_);
  scene.createWorld();
  entity_ = scene.getEntities();
  if (entity_.empty() || !entity_[0] || !entity_[0]->getNode())
    return;

  while (device_->run()) {
    irr::u32 currentTime = device_->getTimer()->getTime();

    updatePlayerMovement(currentTime, scene);
    updateIncantingPlayers(scene);
    scene.updatePaperPlaneMovements();

    driver_->beginScene(true, true, irr::video::SColor(255, 255, 128, 0));

    smgr_->drawAll();
    guienv_->drawAll();

    driver_->endScene();

    if (++frames == 100) {
      irr::core::stringw str = L"Zappy ";
      str += L" FPS: ";
      str += (irr::s32)driver_->getFPS();

      device_->setWindowCaption(str.c_str());
      frames = 0;
    }
  }

  device_->drop();
}
