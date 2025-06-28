/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** Game
*/
#include "../Core/EntityManager.hpp"
#include "../Core/WorldScene.hpp"
#include "../Entities/PlayerEntity.hpp"
#include "../Entities/Stone.hpp"
#include "../Event/EventReceiver.hpp"
#include "../Network/NetworkManager.hpp"
#include <irrlicht/irrlicht.h>
#include <memory>
#include <string>
#include <vector>

#pragma once

class Game {
public:
  /**
   * @brief Constructs a Game instance with network connection parameters
   * 
   * @param host Server host address
   * @param port Server port number
   */
  Game(const std::string &host, int port);
  
  /**
   * @brief Destructor for Game
   */
  ~Game();

  /**
   * @brief Initializes the Irrlicht window and graphics system
   */
  void initWindow();
  
  /**
   * @brief Main game loop that handles rendering and updates
   */
  void gameLoop();
  
  /**
   * @brief Finds an entity by its unique identifier
   * 
   * @param id The entity ID to search for
   * @return Shared pointer to the entity if found, nullptr otherwise
   */
  std::shared_ptr<IEntity> findEntityById(int id);
  
  /**
   * @brief Updates player movement animations and positions
   * 
   * @param currentTime Current time in milliseconds
   * @param scene Reference to the world scene
   */
  void updatePlayerMovement(irr::u32 currentTime, WorldScene &scene);
  
  /**
   * @brief Updates players who are currently performing incantations
   * 
   * @param scene Reference to the world scene
   */
  void updateIncantingPlayers(WorldScene &scene);

  /**
   * @brief Creates and initializes the world scene with test data
   * 
   * @param scene Reference to the world scene to initialize
   */
  void createWorldScene(WorldScene &scene);

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

  NetworkManager *networkManager_;

  /**
   * @brief Initialize the network connection
   * @return true if the connection was successful
   */
  bool initNetwork();

  /**
   * @brief Process network messages
   */
  void processNetworkMessages();
};
