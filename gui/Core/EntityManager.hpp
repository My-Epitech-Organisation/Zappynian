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
  EntityManager(const irr::io::path &mediaPath = "assets/")
      : mediaPath_(mediaPath) {}

  ~EntityManager() = default;

  void createPlayers(irr::scene::ISceneManager *smgr_,
                     irr::video::IVideoDriver *driver, EventReceiver &receiver);

  void createStones(irr::scene::ISceneManager *smgr_,
                    irr::video::IVideoDriver *driver, EventReceiver &receiver);

  void createTiles(irr::scene::ISceneManager *smgr_,
                   irr::video::IVideoDriver *driver, EventReceiver &receiver);

  const std::vector<std::shared_ptr<IEntity>> &getEntities() const {
    return entity_;
  }

protected:
  std::vector<std::shared_ptr<IEntity>> entity_;
  irr::io::path mediaPath_;

private:
};
