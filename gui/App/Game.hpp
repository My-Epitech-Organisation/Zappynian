/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** Game
*/
#include "../Core/EntityManager.hpp"
#include "../Core/WorldScene.hpp"
#include "../Core/GameState.hpp"
#include "../Entities/PlayerEntity.hpp"
#include "../Entities/Stone.hpp"
#include "../Event/EventReceiver.hpp"
#include "../Network/NetworkClient.hpp"
#include "../Network/NetworkManager.hpp"
#include <irrlicht/irrlicht.h>
#include <memory>
#include <string>
#include <vector>

#pragma once

class Game {
public:
  Game(const std::string &host, int port);
  ~Game();

  void initWindow();
  void gameLoop();
  std::shared_ptr<IEntity> findEntityById(int id);
  void updatePlayerMovement(irr::u32 currentTime, WorldScene &scene);
  void updateIncantingPlayers(WorldScene &scene);

private:
  irr::IrrlichtDevice *device_;
  irr::video::IVideoDriver *driver_;
  irr::scene::ISceneManager *smgr_;
  irr::gui::IGUIEnvironment *guienv_;
  EventReceiver receiver_;
  irr::io::path mediaPath_;
  std::vector<std::shared_ptr<IEntity>> entity_;
  std::string host_;
  int port_;
  
  // Nouveau: gestionnaire réseau
  NetworkManager* networkManager_;
  
  /**
   * @brief Initialiser la connexion réseau
   * @return true si connexion réussie
   */
  bool initNetwork();
  
  /**
   * @brief Traiter les messages réseau
   */
  void processNetworkMessages();
};
