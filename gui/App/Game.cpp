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

  // Initialiser la connexion réseau
  if (!initNetwork()) {
    std::cerr << "Failed to initialize network connection\n";
    return;
  }

  std::cout << "Game initialized successfully - Connected to " << host_ << ":" << port_ << std::endl;
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

  // Étape 1: Se connecter au serveur
  std::cout << "Connecting to " << host_ << ":" << port_ << "..." << std::endl;
  if (!networkManager_->connect(host_, port_)) {
    std::cerr << "Failed to connect: " << networkManager_->getLastError() << std::endl;
    return false;
  }

  std::cout << "Connected successfully!" << std::endl;

  // Étape 2: Effectuer le handshake GUI
  std::cout << "Performing handshake..." << std::endl;
  if (!networkManager_->performHandshake()) {
    std::cerr << "Handshake failed: " << networkManager_->getLastError() << std::endl;
    return false;
  }

  std::cout << "Handshake completed successfully!" << std::endl;
  return true;
}

void Game::processNetworkMessages() {
  if (!networkManager_ || !networkManager_->isConnected()) {
    return;
  }

  // Phase 3: Utiliser la synchronisation automatique
  networkManager_->updateFromServer();
  
  // Obtenir l'état du jeu synchronisé
  const auto& gameState = networkManager_->getGameState();
  
  // Afficher des informations de debug si l'état a changé
  static bool firstSync = true;
  if (networkManager_->isGameStateSynchronized() && firstSync) {
    firstSync = false;
    std::cout << "=== ÉTAT SYNCHRONISÉ ===" << std::endl;
    std::cout << "Map: " << gameState.getMapSize().x << "x" << gameState.getMapSize().y << std::endl;
    std::cout << "Teams: " << gameState.getAllTeamNames().size() << std::endl;
    std::cout << "Players: " << gameState.getAllPlayers().size() << std::endl;
    std::cout << "========================" << std::endl;
  }
}

std::shared_ptr<IEntity> Game::findEntityById(int id) {
  for (auto &entity : entity_) {
    if (entity && entity->getId() == id) {
      return entity;
    }
  }
  return {}; // Retourner un shared_ptr vide
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

  // Phase 3: Créer une scène WorldScene directement (pas NetworkClient)
  WorldScene scene(device_, smgr_, driver_, receiver_, mediaPath_);
  
  // Créer la scène initiale basique
  scene.createLights();
  scene.createCamera();
  scene.createText();
  
  // Flags pour la synchronisation
  bool mapInitialized = false;
  bool resourcesInitialized = false;
  
  std::cout << "Game loop started, waiting for synchronization..." << std::endl;

  while (device_->run()) {
    irr::u32 currentTime = device_->getTimer()->getTime();

    // Phase 3: Traiter les messages réseau et synchroniser
    processNetworkMessages();
    
    // Si l'état du jeu est synchronisé, mettre à jour la scène
    if (networkManager_->isGameStateSynchronized()) {
      const auto& gameState = networkManager_->getGameState();
      
      // Initialiser la map si nécessaire
      if (!mapInitialized && gameState.getMapSize().x > 0) {
        auto mapSize = gameState.getMapSize();
        std::cout << "Creating plane with size: " << mapSize.x << "x" << mapSize.y << std::endl;
        scene.createPlane(mapSize.x, mapSize.y);
        
        // Ajouter les équipes
        for (const auto& teamName : gameState.getAllTeamNames()) {
          scene.addTeam(teamName);
          std::cout << "Added team: " << teamName << std::endl;
        }
        
        mapInitialized = true;
        
        // Mettre à jour la liste des entités après création de la map
        entity_ = scene.getEntities();
      }
      
      // Synchroniser les joueurs
      auto players = gameState.getAllPlayers();
      for (const auto* player : players) {
        // Vérifier si le joueur existe déjà dans la scène
        bool playerExists = false;
        for (auto& entity : entity_) {
          if (entity && entity->getId() == player->id) {
            playerExists = true;
            break;
          }
        }
        
        if (!playerExists) {
          // Créer le nouveau joueur
          scene.createEntities(player->id, player->pos.x, player->pos.y, 
                             static_cast<Direction>(player->direction), 
                             player->level, player->team);
          std::cout << "Created player " << player->id << " at (" 
                    << player->pos.x << "," << player->pos.y << ")" << std::endl;
          
          // Mettre à jour la liste des entités
          entity_ = scene.getEntities();
        }
      }
      
      // Synchroniser les ressources sur les tiles (une seule fois après l'initialisation de la map)
      if (mapInitialized && !resourcesInitialized) {
        std::cout << "=== SYNCHRONIZING RESOURCES ===" << std::endl;
        auto tiles = gameState.getAllTiles();
        std::cout << "Found " << tiles.size() << " tiles with resources to synchronize" << std::endl;
        
        for (const auto* tile : tiles) {
          // Vérifier si cette tile a des ressources
          int totalResources = tile->resources.food + tile->resources.linemate + 
                             tile->resources.deraumere + tile->resources.sibur + 
                             tile->resources.mendiane + tile->resources.phiras + 
                             tile->resources.thystame;
          
          if (totalResources > 0) {
            std::cout << "Creating resources at tile (" << tile->pos.x << "," << tile->pos.y 
                      << ") - food:" << tile->resources.food 
                      << " linemate:" << tile->resources.linemate
                      << " deraumere:" << tile->resources.deraumere
                      << " sibur:" << tile->resources.sibur
                      << " mendiane:" << tile->resources.mendiane
                      << " phiras:" << tile->resources.phiras
                      << " thystame:" << tile->resources.thystame << std::endl;
            
            // Utiliser createEntities pour créer les ressources sur cette tile
            scene.createEntities(tile->pos.x, tile->pos.y,
                               tile->resources.food,
                               tile->resources.linemate,
                               tile->resources.deraumere,
                               tile->resources.sibur,
                               tile->resources.mendiane,
                               tile->resources.phiras,
                               tile->resources.thystame);
          }
        }
        resourcesInitialized = true;
        std::cout << "=== RESOURCES SYNCHRONIZED ===" << std::endl;
        
        // Mettre à jour la liste des entités après création des ressources
        entity_ = scene.getEntities();
      }
    }

    // Mise à jour des mouvements et animations si on a des entités
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
