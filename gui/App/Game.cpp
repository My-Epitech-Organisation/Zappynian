/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** Game
*/

#include "Game.hpp"

Game::Game() { initWindow(); }

Game::~Game() {}

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
}

void Game::gameLoop() {
  irr::u32 frames = 0;

  WorldScene worldScene(smgr_, driver_, receiver_, mediaPath_);

  worldScene.createWorld();
  entity_ = worldScene.getEntities();

  while (device_->run()) {
    irr::u32 currentTime = device_->getTimer()->getTime();

    if (receiver_.isMoving) {
      float elapsedTime = (currentTime - receiver_.moveStartTime) / 1000.0f;
      if (elapsedTime >= 1.0f) {
        irr::core::vector3df pos;
        float angle = receiver_.currentRotationY * M_PI / 180.0f;
        pos.X = receiver_.moveStartX - 20.0f * sin(angle);
        pos.Z = receiver_.moveStartZ - 20.0f * cos(angle);
        pos.Y = entity_[0]->getNode()->getPosition().Y;
        entity_[0]->getNode()->setPosition(pos);
        receiver_.isMoving = false;
        entity_[0]->getNode()->setAnimationSpeed(0.0f);
      } else {
        float progress = elapsedTime / 1.0f;
        irr::core::vector3df pos;
        float angle = receiver_.currentRotationY * M_PI / 180.0f;
        pos.X = receiver_.moveStartX - (20.0f * progress * sin(angle));
        pos.Z = receiver_.moveStartZ - (20.0f * progress * cos(angle));
        pos.Y = entity_[0]->getNode()->getPosition().Y;
        entity_[0]->getNode()->setPosition(pos);
      }
    }

    driver_->beginScene(true, true, irr::video::SColor(255, 255, 128, 0));

    guienv_->drawAll();
    smgr_->drawAll();

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