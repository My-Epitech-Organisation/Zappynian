/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** EntityManager
*/
#include "../Entities/Direction.hpp"
#include "../Entities/Egg.hpp"
#include "../Entities/PlayerEntity.hpp"
#include "../Entities/Stone.hpp"
#include "../Entities/TileEntity.hpp"
#include "../Event/EventReceiver.hpp"
#include <iostream>
#include <irrlicht/irrlicht.h>
#include <memory>
#include <string>
#include <vector>

#pragma once

class EntityManager {
public:
  /**
   * @brief Constructs an EntityManager with required dependencies
   * 
   * @param smgr Pointer to the scene manager
   * @param driver Pointer to the video driver
   * @param receiver Reference to the event receiver
   * @param mediaPath Path to media assets
   */
  EntityManager(irr::scene::ISceneManager *smgr,
                irr::video::IVideoDriver *driver, EventReceiver &receiver,
                const irr::io::path &mediaPath)
      : smgr_(smgr), driver_(driver), receiver_(receiver),
        mediaPath_(mediaPath) {};

  /**
   * @brief Default destructor for EntityManager
   */
  ~EntityManager() = default;

  /**
   * @brief Creates player entities
   * 
   * @param id Player identifier
   * @param x X coordinate
   * @param y Y coordinate
   * @param direction Player's facing direction
   * @param level Player's level
   * @param team Player's team name
   */
  void createPlayers(int id, int x, int y, Direction direction, int level,
                     std::string team);

  /**
   * @brief Creates stone resource entities
   * 
   * @param x X coordinate
   * @param y Y coordinate
   * @param q0 Food quantity (default: 0)
   * @param q1 Linemate quantity (default: 0)
   * @param q2 Deraumere quantity (default: 0)
   * @param q3 Sibur quantity (default: 0)
   * @param q4 Mendiane quantity (default: 0)
   * @param q5 Phiras quantity (default: 0)
   * @param q6 Thystame quantity (default: 0)
   */
  void createStones(int x, int y, int q0 = 0, int q1 = 0, int q2 = 0,
                    int q3 = 0, int q4 = 0, int q5 = 0, int q6 = 0);

  /**
   * @brief Creates tile entities
   * 
   * @param x X coordinate
   * @param y Y coordinate
   */
  void createTiles(int x, int y);

  /**
   * @brief Creates a paper plane entity for broadcasts
   * 
   * @param position Starting position for the paper plane
   * @return Shared pointer to the created paper plane entity
   */
  std::shared_ptr<IEntity> createPaperPlane(irr::core::vector3df position);

  /**
   * @brief Creates an egg entity
   * 
   * @param id Egg identifier
   */
  void createEgg(int id);

  /**
   * @brief Gets all entities managed by this manager
   * 
   * @return Const reference to the entities vector
   */
  const std::vector<std::shared_ptr<IEntity>> &getEntities() const {
    return entity_;
  }

  /**
   * @brief Gets a tile entity by name
   * 
   * @param name Name of the tile to find
   * @return Shared pointer to the tile entity, nullptr if not found
   */
  std::shared_ptr<IEntity> getTileByName(const std::string &name) {
    for (const auto &tile : tiles_) {
      if (tile->getName() == name) {
        return tile;
      }
    }
    return nullptr;
  }

  /**
   * @brief Sets the list of teams
   * 
   * @param teams Vector of team names
   */
  void addTeams(std::vector<std::string> teams) { teams_ = teams; }

  /**
   * @brief Places stone entities at specified position
   * 
   * @param position Position to place stones
   * @param quantities Quantities of each stone type
   * @param stoneTextures Textures for each stone type
   * @param qB3D 3D model paths for stones
   * @param stoneNames Names of stone types
   * @param qScale Scale values for each stone type
   */
  void placeStoneEntities(
      const irr::core::vector3df &position, const std::vector<int> &quantities,
      const std::vector<std::vector<irr::io::path>> &stoneTextures,
      const std::vector<irr::io::path> &qB3D,
      const std::vector<std::string> &stoneNames,
      const std::vector<irr::core::vector3df> &qScale);

  /**
   * @brief Creates a dropped resource entity
   * 
   * @param x X coordinate
   * @param y Y coordinate
   * @param item Type of resource being dropped
   */
  void createDroppedResource(int x, int y, const std::string &item);

  /**
   * @brief Updates the map text display
   * 
   * @param textMap Pointer to the map text GUI element
   */
  void updateMapText(irr::gui::IGUIStaticText *textMap);

protected:
  std::vector<std::shared_ptr<IEntity>> entity_;
  irr::scene::ISceneManager *smgr_;
  irr::video::IVideoDriver *driver_;
  EventReceiver &receiver_;
  irr::io::path mediaPath_;
  std::vector<std::shared_ptr<TileEntity>> tiles_;
  std::vector<std::string> teams_;
};
