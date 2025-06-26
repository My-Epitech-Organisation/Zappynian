/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** Game
*/

#include "Game.hpp"
#include "../Entities/IEntity.hpp"
#include <memory>
#include <iostream>

Game::Game(const std::string &host, int port)
    : host_(host), port_(port), networkManager_(new NetworkManager()) {
  initWindow();
  if (!device_) {
    std::cerr << "Failed to create Irrlicht device\n";
    return;
  }

  if (!initNetwork()) {
    std::cerr << "Failed to initialize network connection\n";
    return;
  }
}

Game::~Game() {
  if (networkManager_) {
    delete networkManager_;
    networkManager_ = nullptr;
  }
}

bool Game::initNetwork() {
  if (!networkManager_) {
    std::cerr << "NetworkManager not initialized\n";
    return false;
  }

  if (!networkManager_->connect(host_, port_)) {
    return false;
  }
  if (!networkManager_->performHandshake()) {
    return false;
  }
  return true;
}

void Game::processNetworkMessages() {
  if (!networkManager_ || !networkManager_->isConnected()) {
    return;
  }

  networkManager_->updateFromServer();

  const auto& gameState = networkManager_->getGameState();

  static bool firstSync = true;
  if (networkManager_->isGameStateSynchronized() && firstSync) {
    firstSync = false;
  }
}

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
  mediaPath_ = "gui/assets/";
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

void Game::updatePlayerMovement(irr::u32 currentTime, WorldScene &scene) {
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

void Game::updateIncantingPlayers(WorldScene &scene) {
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

  WorldScene scene(device_, smgr_, driver_, receiver_, mediaPath_);

  createWorldScene(scene);
  if (networkManager_)
    networkManager_->initializeParser(scene);

  bool mapInitialized = false;
  bool resourcesInitialized = false;

  while (device_->run()) {
    irr::u32 currentTime = device_->getTimer()->getTime();

    processNetworkMessages();

    if (networkManager_->isGameStateSynchronized()) {
      const auto& gameState = networkManager_->getGameState();

      if (!mapInitialized && gameState.getMapSize().X > 0) {
        auto mapSize = gameState.getMapSize();
        scene.createPlane(mapSize.X, mapSize.Y);
        for (const auto& teamName : gameState.getAllTeamNames()) {
          scene.addTeam(teamName);
        }
        mapInitialized = true;
        entity_ = scene.getEntities();
      }

      if (mapInitialized && !resourcesInitialized) {
        auto tiles = gameState.getAllTiles();
        for (const auto* tile : tiles) {
          int totalResources = tile->getInventory().getItemQuantity("food") +
                              tile->getInventory().getItemQuantity("linemate") +
                              tile->getInventory().getItemQuantity("deraumere") +
                              tile->getInventory().getItemQuantity("sibur") +
                              tile->getInventory().getItemQuantity("mendiane") +
                              tile->getInventory().getItemQuantity("phiras") +
                              tile->getInventory().getItemQuantity("thystame");
          if (totalResources > 0) {
            scene.createEntities(tile->getPosition().X, tile->getPosition().Y,
                                tile->getInventory().getItemQuantity("food"),
                                tile->getInventory().getItemQuantity("linemate"),
                                tile->getInventory().getItemQuantity("deraumere"),
                                tile->getInventory().getItemQuantity("sibur"),
                                tile->getInventory().getItemQuantity("mendiane"),
                                tile->getInventory().getItemQuantity("phiras"),
                                tile->getInventory().getItemQuantity("thystame"));
          }
        }
        resourcesInitialized = true;
        entity_ = scene.getEntities();
      }
    }

    entity_ = scene.getEntities();

    if (!entity_.empty()) {
      updatePlayerMovement(currentTime, scene);
      updateIncantingPlayers(scene);
      scene.updatePaperPlaneMovements();
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
