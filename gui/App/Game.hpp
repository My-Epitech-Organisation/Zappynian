/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** Game
*/
#include "../Event/EventReceiver.hpp"
#include <irrlicht/irrlicht.h>

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

private:
};
