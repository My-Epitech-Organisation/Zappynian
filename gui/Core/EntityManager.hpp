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
  EntityManager(irr::scene::ISceneManager *smgr,
                irr::video::IVideoDriver *driver, EventReceiver &receiver,
                const irr::io::path &mediaPath)
      : smgr_(smgr), driver_(driver), receiver_(receiver),
        mediaPath_(mediaPath) {};

  ~EntityManager() = default;

  void createPlayers(int id, int x, int y, Direction direction, int level,
                     std::string team);

  void createStones(int x, int y, int q0 = 0, int q1 = 0, int q2 = 0,
                    int q3 = 0, int q4 = 0, int q5 = 0, int q6 = 0);

  void createTiles(int x, int y);

  std::shared_ptr<IEntity> createPaperPlane(irr::core::vector3df position);

  void createEgg(int id);

  const std::vector<std::shared_ptr<IEntity>> &getEntities() const {
    return entity_;
  }

  std::shared_ptr<IEntity> getTileByName(const std::string &name) {
    for (const auto &tile : tiles_) {
      if (tile->getName() == name) {
        return tile;
      }
    }
    return nullptr;
  }

  void addTeams(std::vector<std::string> teams) { teams_ = teams; }

  void placeStoneEntities(
      const irr::core::vector3df &position, const std::vector<int> &quantities,
      const std::vector<std::vector<irr::io::path>> &stoneTextures,
      const std::vector<irr::io::path> &qB3D,
      const std::vector<std::string> &stoneNames,
      const std::vector<irr::core::vector3df> &qScale);

protected:
  std::vector<std::shared_ptr<IEntity>> entity_;
  irr::scene::ISceneManager *smgr_;
  irr::video::IVideoDriver *driver_;
  EventReceiver &receiver_;
  irr::io::path mediaPath_;
  std::vector<std::shared_ptr<TileEntity>> tiles_;
  std::vector<std::string> teams_;
};
