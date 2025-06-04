/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** Game
*/
#include "../Entities/PlayerEntity.hpp"
#include "../Entities/Stone.hpp"
#include "../Event/EventReceiver.hpp"
#include "../Core/EntityManager.hpp"
#include <irrlicht/irrlicht.h>
#include <memory>
#include <string>
#include <vector>

#pragma once

class Game {
public:
  Game();
  ~Game();

  void initWindow();

  void gameLoop();

protected:
  irr::IrrlichtDevice *device_;
  irr::video::IVideoDriver *driver_;
  irr::scene::ISceneManager *smgr_;
  irr::gui::IGUIEnvironment *guienv_;
  EventReceiver receiver_;
  std::vector<std::shared_ptr<IEntity>> entity_;

private:
};
