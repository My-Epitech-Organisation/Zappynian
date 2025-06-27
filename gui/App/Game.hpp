/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** Game
*/
#include "../Core/EntityManager.hpp"
#include "../Core/GameState.hpp"
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
  void createWorldScene(WorldScene &scene) {
    scene.createPlane(5, 5);
    scene.createLights();
    scene.createCamera();
    scene.createText();
    scene.addTeam("Red");
    scene.addTeam("Blue");
    scene.addTeam("Yellow");
    scene.addTeam("Purple");
    scene.addTeam("Orange");
    scene.addTeam("Green");
    try {
      scene.createEntities(1, 2, 2, Direction::WEST, 0, "Red");
    } catch (const std::exception &e) {
    }
    scene.setPlayerLevel(1, 1);
    try {
      scene.createEntities(2, 2, 2, Direction::EAST, 0, "Blue");
    } catch (const std::exception &e) {
    }
    scene.setPlayerLevel(2, 2);
    try {
      scene.createEntities(3, 2, 2, Direction::EAST, 0, "Red");
    } catch (const std::exception &e) {
    }
    scene.setPlayerLevel(3, 3);
    try {
      scene.createEntities(4, 0, 0, Direction::WEST, 0, "Yellow");
    } catch (const std::exception &e) {
    }
    scene.setPlayerLevel(4, 4);
    try {
      scene.createEntities(5, 0, 1, Direction::WEST, 0, "Purple");
    } catch (const std::exception &e) {
    }
    scene.setPlayerLevel(5, 5);
    try {
      scene.createEntities(6, 0, 2, Direction::WEST, 0, "Orange");
    } catch (const std::exception &e) {
    }
    scene.setPlayerLevel(6, 6);
    try {
      scene.createEntities(7, 0, 3, Direction::WEST, 0, "Green");
    } catch (const std::exception &e) {
    }
    scene.setPlayerLevel(7, 7);
    try {
      scene.createEntities(8, 0, 4, Direction::WEST, 0, "Green");
    } catch (const std::exception &e) {
    }
    scene.setPlayerLevel(8, 8);
    try {
      scene.createEntities(9, 4, 4, Direction::WEST, 0, "Red");
    } catch (const std::exception &e) {
    }
    scene.setPlayerLevel(9, 5);

    try {
      scene.createEntities(10, 3, 0, Direction::WEST, 0, "Red");
    } catch (const std::exception &e) {
    }
    scene.setPlayerLevel(10, 5);
    scene.setPlayerInventory(10, 1, 1, 1, 1, 1, 1, 1);
    scene.createEntities(3, 0, 10, 0, 10, 10, 10, 10, 10);
    scene.resourceDropping(10, "linemate");
    scene.changePlayerPos(10, 4, 0, Direction::WEST, Direction::WEST);

    scene.createEntities(2, 4, 32, 32, 32, 32, 32, 32, 32);
    scene.createEntities(4, 4, 32, 32, 32, 32, 32, 32, 32);
    scene.changePlayerPos(4, 1, 0, Direction::WEST, Direction::WEST);
    scene.changePlayerPos(5, 1, 1, Direction::WEST, Direction::WEST);
    scene.changePlayerPos(6, 1, 2, Direction::WEST, Direction::WEST);
    scene.changePlayerPos(7, 1, 3, Direction::WEST, Direction::WEST);
    scene.changePlayerPos(8, 1, 4, Direction::WEST, Direction::WEST);
    scene.changePlayerPos(1, 2, 3, Direction::WEST, Direction::WEST);
    // changePlayerPos(1, 2, 2, Direction::NORTH);
    // changePlayerPos(1, 2, 1, Direction::NORTH);
    // changePlayerPos(1, 2, 0, Direction::NORTH);
    // changePlayerPos(1, 2, 4, Direction::NORTH);
    // changePlayerPos(1, 1, 4, Direction::EAST);
    // changePlayerPos(1, 0, 4, Direction::EAST);
    // changePlayerPos(1, 4, 4, Direction::EAST);
    // changePlayerPos(1, 4, 0, Direction::SOUTH);
    // changePlayerPos(1, 4, 1, Direction::SOUTH);
    // changePlayerPos(1, 4, 2, Direction::SOUTH);
    // changePlayerPos(1, 0, 2, Direction::WEST);
    // changePlayerPos(2, 4, 2, Direction::NORTH);
    scene.setPlayerInventory(1, 1, 1, 1, 14, 1, 1, 1);
    scene.setPlayerInventory(3, 10, 10, 10, 10, 10, 10, 10);
    scene.startIncantation(4, 4, 6, {9});
    // stopIncantation(4, 4, true);
    // killPlayer(3);
    // resourceDroping(3, "linemate");
    // resourceDroping(3, "sibur");
    // resourceDroping(3, "phiras");
    // resourceDroping(3, "thystame");
    scene.broadcast(1, "Hello from player 1!");
    scene.broadcast(2, "Hello from player 2!");
    scene.createEntities(4);
    scene.createEntities(5);
    scene.createEntities(1);
    scene.createEntities(6);
    scene.createEntities(7);
    scene.createEntities(8);
    scene.expulsion(1);

    // resourceDroping(3, "mendiane");
    // resourceDroping(3, "deraumere");
    // endGame("Red");
  }

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
