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
    if (entity->getId() == id) {
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

void Game::gameLoop() {
  irr::u32 frames = 0;

  NetworkClient scene(device_, smgr_, driver_, receiver_, mediaPath_);
  scene.createWorld();
  entity_ = scene.getEntities();
  if (entity_.empty() || !entity_[0] || !entity_[0]->getNode()) {
    return;
  }

  while (device_->run()) {
    irr::u32 currentTime = device_->getTimer()->getTime();

    if (receiver_.getIsMoving()) {
      auto currentEntity = findEntityById(receiver_.getCurrentEntityId());
      if (!currentEntity) {
        receiver_.setIsMoving(false);
        continue;
      }

      float elapsedTime =
          (currentTime - receiver_.getMoveStartTime()) / 1000.0f;
      if (elapsedTime >= 1.0f) {
        irr::core::vector3df pos;
        float angle = receiver_.getCurrentRotationY() * M_PI / 180.0f;
        pos.X = receiver_.getMoveStartX() - 20.0f * sin(angle);
        pos.Z = receiver_.getMoveStartZ() - 20.0f * cos(angle);
        pos.Y = currentEntity->getNode()->getPosition().Y;
        currentEntity->getNode()->setPosition(pos);
        receiver_.setIsMoving(false);
        currentEntity->getNode()->setAnimationSpeed(0.0f);
        scene.updateMovements();
      } else {
        float progress = elapsedTime / 1.0f;
        irr::core::vector3df pos;
        float angle = receiver_.getCurrentRotationY() * M_PI / 180.0f;
        pos.X = receiver_.getMoveStartX() - (20.0f * progress * sin(angle));
        pos.Z = receiver_.getMoveStartZ() - (20.0f * progress * cos(angle));
        pos.Y = currentEntity->getNode()->getPosition().Y;
        currentEntity->getNode()->setPosition(pos);
      }
    }

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
