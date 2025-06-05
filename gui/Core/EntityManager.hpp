/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** EntityManager
*/
#include "../Entities/PlayerEntity.hpp"
#include "../Entities/Stone.hpp"
#include "../Entities/TileEntity.hpp"
#include "../Event/EventReceiver.hpp"
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

  void createStones();

  void createTiles(int x, int y);

  const std::vector<std::shared_ptr<IEntity>> &getEntities() const {
    return entity_;
  }

protected:
  std::vector<std::shared_ptr<IEntity>> entity_;
  irr::scene::ISceneManager *smgr_;
  irr::video::IVideoDriver *driver_;
  EventReceiver &receiver_;
  irr::io::path mediaPath_;

private:
};
