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
#include <algorithm>

struct TileInfo {
    int x, y;
    int food, linemate, deraumere, sibur, mendiane, phiras, thystame;
};
int mapWidth = 0;
int mapHeight = 0;
std::vector<std::string> teamNames;
std::vector<TileInfo> tiles;


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
  if (!networkManager_) {
    return;
  }
  bool isConnected = networkManager_->isConnected();
  if (!isConnected) {
    return;
  }

  networkManager_->updateFromServer();

  static bool firstSync = true;
  
  bool isGameStateSynchronized = networkManager_->isGameStateSynchronized();
  
  if (isGameStateSynchronized && firstSync) {
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
  bool isMoving = receiver_.getIsMoving();
  if (!isMoving) {
    return;
  }

  int currentEntityId = receiver_.getCurrentEntityId();
  auto currentEntity = findEntityById(currentEntityId);
  
  if (!currentEntity) {
    receiver_.setIsMoving(false);
    return;
  }

  irr::u32 moveStartTime = receiver_.getMoveStartTime();
  float elapsedTime = (currentTime - moveStartTime) / 1000.0f;
  
  auto *node = currentEntity->getNode();
  
  if (!node) {
    receiver_.setIsMoving(false);
    return;
  }
  
  auto *parent = node->getParent();
  
  if (!parent) {
    receiver_.setIsMoving(false);
    return;
  }

  if (elapsedTime >= 0.00005f) {
    irr::core::vector3df pos;
    float rotationY = receiver_.getCurrentRotationY();
    float angle = rotationY * M_PI / 180.0f;
    float moveStartX = receiver_.getMoveStartX();
    float moveStartZ = receiver_.getMoveStartZ();
    pos.X = moveStartX - 20.0f * sin(angle);
    pos.Z = moveStartZ - 20.0f * cos(angle);
    try {
      irr::core::vector3df currentPos = node->getPosition();
      pos.Y = currentPos.Y;
    } catch (...) {
      receiver_.setIsMoving(false);
      return;
    }
    node->setPosition(pos);
    receiver_.setIsMoving(false);
    if (auto *animatedNode = dynamic_cast<irr::scene::IAnimatedMeshSceneNode *>(node)) {
      animatedNode->setAnimationSpeed(0.0f);
    } else {
      std::cout << "[ERROR] Node is not an animated mesh scene node" << std::endl;
    }
    
    scene.updateMovements();
  } else {
    float progress = elapsedTime / 0.00005f;
    irr::core::vector3df pos;
    float rotationY = receiver_.getCurrentRotationY();
    float angle = rotationY * M_PI / 180.0f;
    float moveStartX = receiver_.getMoveStartX();
    float moveStartZ = receiver_.getMoveStartZ();
    pos.X = moveStartX - (20.0f * progress * sin(angle));
    pos.Z = moveStartZ - (20.0f * progress * cos(angle));
    try {
      irr::core::vector3df currentPos = node->getPosition();
      pos.Y = currentPos.Y;
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

  entity_ = scene.getEntities();
  while (device_->run()) {
    irr::u32 currentTime = device_->getTimer()->getTime();
    processNetworkMessages();
    scene.updateMapText();
    entity_ = scene.getEntities();

    if (!mapInitialized && mapWidth > 0 && mapHeight > 0) {
        scene.createPlane(mapWidth, mapHeight);
        for (const auto& teamName : teamNames) {
            scene.addTeam(teamName);
        }
        mapInitialized = true;
        entity_ = scene.getEntities();
    }

    if (mapInitialized && !resourcesInitialized) {
        for (const auto& tile : tiles) {
            int totalResources = tile.food + tile.linemate + tile.deraumere +
                                 tile.sibur + tile.mendiane + tile.phiras + tile.thystame;
            if (totalResources > 0) {
                scene.createEntities(tile.x, tile.y,
                                    tile.food, tile.linemate, tile.deraumere,
                                    tile.sibur, tile.mendiane, tile.phiras, tile.thystame);
            }
        }
        resourcesInitialized = true;
        entity_ = scene.getEntities();
    }

    entity_ = scene.getEntities();
    if (!entity_.empty()) {
      updatePlayerMovement(currentTime, scene);
      updateIncantingPlayers(scene);
      scene.updatePaperPlaneMovements();
      entity_ = scene.getEntities();
    } else {
      std::cout << "[DEBUG] No entities to update" << std::endl;
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

void Game::createWorldScene(WorldScene &scene) {
    scene.createLights();
    scene.createCamera();
    scene.createText();
}
