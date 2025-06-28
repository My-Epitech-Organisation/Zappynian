/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** WorldScene
*/
#include "../Entities/Direction.hpp"
#include "EntityManager.hpp"
#include <iostream>
#include <queue>
#include <unordered_map>
#pragma once

struct Movement {
  int id;
  int x;
  int y;
  Direction direction;
  Direction directionBefore;
};

struct PaperPlaneMovement {
  std::shared_ptr<IEntity> paperPlane;
  int targetPlayerId;
  irr::core::vector3df startPosition;
  irr::core::vector3df targetPosition;
  irr::u32 startTime;
  float duration; // sec
  bool isActive;
};

struct EdgePositionResult {
  bool isEdge;
  float offsetX;
  float offsetZ;
  irr::core::vector3df nextPosition;
};

class WorldScene {
public:
  /**
   * @brief Constructs a WorldScene with required dependencies
   * 
   * @param device Pointer to the Irrlicht device
   * @param smgr Pointer to the scene manager
   * @param driver Pointer to the video driver
   * @param receiver Reference to the event receiver
   * @param mediaPath Path to media assets
   */
  WorldScene(irr::IrrlichtDevice *device, irr::scene::ISceneManager *smgr,
             irr::video::IVideoDriver *driver, EventReceiver &receiver,
             const irr::io::path &mediaPath)
      : device_(device), smgr_(smgr), driver_(driver), receiver_(receiver),
        mediaPath_(mediaPath),
        entityManager_(smgr, driver, receiver, mediaPath) {};
        
  /**
   * @brief Virtual destructor for WorldScene
   */
  virtual ~WorldScene() = default;

  /**
   * @brief Creates a player entity at specified position
   * 
   * @param id Player unique identifier
   * @param x X coordinate
   * @param y Y coordinate
   * @param direction Player's facing direction
   * @param level Player's level
   * @param team Player's team name
   */
  void createEntities(int id, int x, int y, Direction direction, int level,
                      std::string team);

  /**
   * @brief Sets a player's inventory contents
   * 
   * @param id Player identifier
   * @param q0 Food quantity
   * @param q1 Linemate quantity
   * @param q2 Deraumere quantity
   * @param q3 Sibur quantity
   * @param q4 Mendiane quantity
   * @param q5 Phiras quantity
   * @param q6 Thystame quantity
   */
  void setPlayerInventory(int id, int q0, int q1, int q2, int q3,
                          int q4, int q5, int q6);

  /**
   * @brief Sets a player's level
   * 
   * @param id Player identifier
   * @param level Level to set
   */
  void setPlayerLevel(int id, int level);

  /**
   * @brief Changes the visual representation of a player's level
   * 
   * @param entity Reference to the player entity
   * @param level New level to display
   */
  void changeHeadLevel(std::shared_ptr<IEntity> &entity, int level);

  /**
   * @brief Creates resource entities on a tile
   * 
   * @param x Tile X coordinate
   * @param y Tile Y coordinate
   * @param q0 Food quantity
   * @param q1 Linemate quantity
   * @param q2 Deraumere quantity
   * @param q3 Sibur quantity
   * @param q4 Mendiane quantity
   * @param q5 Phiras quantity
   * @param q6 Thystame quantity
   * @param nbTiles Number of tiles
   */
  void createEntities(int x, int y, int q0, int q1, int q2, int q3, int q4,
                      int q5, int q6, int nbTiles);

  /**
   * @brief Creates resource entities on a tile (overloaded version)
   * 
   * @param x Tile X coordinate
   * @param y Tile Y coordinate
   * @param q0 Food quantity
   * @param q1 Linemate quantity
   * @param q2 Deraumere quantity
   * @param q3 Sibur quantity
   * @param q4 Mendiane quantity
   * @param q5 Phiras quantity
   * @param q6 Thystame quantity
   */
  void createEntities(int x, int y, int q0, int q1, int q2, int q3, int q4,
                      int q5, int q6);

  /**
   * @brief Creates an egg entity
   * 
   * @param id Egg identifier
   */
  void createEntities(int eggid, int id, int x, int y);

  /**
   * @brief Changes a player's position and direction
   * 
   * @param id Player identifier
   * @param x New X coordinate
   * @param y New Y coordinate
   * @param direction New facing direction
   * @param directionBefore Previous direction for animation
   */
  void changePlayerPos(int id, int x, int y, Direction direction,
                       Direction directionBefore);

  /**
   * @brief Checks if a position is at the edge of the map
   * 
   * @param position Current position
   * @param direction Movement direction
   * @param nextX Next X coordinate
   * @param nextY Next Y coordinate
   * @return EdgePositionResult containing edge information
   */
  EdgePositionResult isEdgePosition(const irr::core::vector3df &position,
                                    Direction direction, int nextX, int nextY);

  /**
   * @brief Updates all pending movement animations
   */
  void updateMovements();

  /**
   * @brief Updates paper plane movement animations for broadcasts
   */
  void updatePaperPlaneMovements();

  /**
   * @brief Creates lighting for the scene
   */
  void createLights();

  /**
   * @brief Creates and configures the camera
   */
  void createCamera();

  /**
   * @brief Creates the ground plane
   * 
   * @param x Width of the plane
   * @param y Height of the plane
   */
  void createPlane(int x, int y);

  /**
   * @brief Creates UI text elements
   */
  void createText();

  /**
   * @brief Starts an incantation ritual
   * 
   * @param x X coordinate of incantation
   * @param y Y coordinate of incantation
   * @param level Required level for incantation
   * @param ids Vector of player IDs participating
   */
  void startIncantation(int x, int y, std::vector<int> ids);
  
  /**
   * @brief Stops an ongoing incantation
   * 
   * @param x X coordinate of incantation
   * @param y Y coordinate of incantation
   * @param result Success or failure of incantation
   */
  void stopIncantation(int x, int y, bool result);
  
  /**
   * @brief Checks if a player is currently incanting
   * 
   * @param id Player identifier
   * @return true if player is incanting, false otherwise
   */
  bool isPlayerIncanting(int id) const {
    return isIncanting_.count(id) > 0 && isIncanting_.at(id);
  }

  /**
   * @brief Gets all entities in the scene
   * 
   * @return Vector of shared pointers to entities
   */
  std::vector<std::shared_ptr<IEntity>> getEntities() const { return entity_; }

  /**
   * @brief Removes a player from the game
   * 
   * @param id Player identifier to remove
   */
  void killPlayer(int id);

  /**
   * @brief Removes an egg from the game
   * 
   * @param id Egg identifier to remove
   */
  void killEgg(int id);

  /**
   * @brief Adds a message to the chat system
   * 
   * @param message Message text to add
   */
  void addChatMessage(const std::string &message);

  /**
   * @brief Handles a broadcast message from a player
   * 
   * @param id Player identifier who sent the broadcast
   * @param message Broadcast message content
   */
  void broadcast(int id, const std::string &message);

  /**
   * @brief Adds a team to the game
   * 
   * @param team Team name to add
   */
  void addTeam(const std::string &team) { teams_.push_back(team); }

  /**
   * @brief Clears all elements from the scene
   */
  void clearElements();

  /**
   * @brief Ends the game and displays winner
   * 
   * @param winner Name of the winning team
   */
  void endGame(std::string winner);

  /**
   * @brief Gets all players and eggs at a specific position
   * 
   * @param position 3D position to check
   * @return Vector of entities at the position
   */
  std::vector<std::shared_ptr<IEntity>>
  getPlayersEggAtPosition(irr::core::vector3df position);

  /**
   * @brief Moves a player due to expulsion
   * 
   * @param id Player identifier
   * @param direction Direction of expulsion force
   * @param directionEnum Direction enum value
   * @param pos Current position
   */
  void expulsionMove(int id, irr::f32 direction, Direction directionEnum,
                     irr::core::vector3df pos);

  /**
   * @brief Handles player expulsion event
   * 
   * @param id Player identifier who caused expulsion
   */
  void expulsion(int id);

  /**
   * @brief Handles a player dropping a resource
   * 
   * @param id Player identifier
   * @param item Resource type being dropped
   */
  void resourceDropping(int id, const std::string &item);

  /**
   * @brief Handles a player collecting a resource
   * 
   * @param id Player identifier
   * @param item Resource type being collected
   */
  void resourceCollect(int id, const std::string &item);

  /**
   * @brief Updates the map display text
   */
  void updateMapText();

protected:
  irr::IrrlichtDevice *device_;
  irr::scene::ISceneManager *smgr_;
  irr::video::IVideoDriver *driver_;
  EventReceiver &receiver_;
  irr::io::path mediaPath_;
  EntityManager entityManager_;
  std::vector<std::shared_ptr<IEntity>> entity_;
  std::queue<Movement> movementQueue_;
  std::vector<PaperPlaneMovement> paperPlaneMovements_;
  irr::core::vector3df actualPos_;
  std::pair<int, int> planeSize_;
  std::unordered_map<int, bool> isIncanting_;
  std::vector<std::tuple<int, int, int>> incantationData_; // x, y, id
  irr::gui::IGUIStaticText *textChat_;
  irr::gui::IGUIStaticText *textMap_;
  std::vector<std::string> chatMessages_;
  static const size_t MAX_CHAT_MESSAGES = 20;
  std::vector<std::string> teams_;
  std::map<std::string, std::pair<int, int>> teamStates_; // team name, (alive, death)

private:
  /**
   * @brief Updates the chat display with recent messages
   */
  void updateChatDisplay();
};
