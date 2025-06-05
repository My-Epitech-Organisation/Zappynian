/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** WorldScene
*/
#include "EntityManager.hpp"
#pragma once

class WorldScene {
public:
  WorldScene();
  ~WorldScene();

  void createEntities(irr::scene::ISceneManager *smgr,
                      irr::video::IVideoDriver *driver,
                      EventReceiver &receiver);

  void createLights(irr::scene::ISceneManager *smgr);

  void createCamera(irr::scene::ISceneManager *smgr);

  void createPlane(irr::scene::ISceneManager *smgr,
                   irr::video::IVideoDriver *driver, EventReceiver &receiver,
                   const irr::io::path &mediaPath);

  void createText(irr::gui::IGUIEnvironment *guienv,
                  const irr::io::path &mediaPath, EventReceiver &receiver);

  void createWorld(irr::scene::ISceneManager *smgr,
                   irr::video::IVideoDriver *driver, EventReceiver &receiver,
                   const irr::io::path &mediaPath) {
    createEntities(smgr, driver, receiver);
    createLights(smgr);
    createCamera(smgr);
    createPlane(smgr, driver, receiver, mediaPath);
    createText(smgr->getGUIEnvironment(), mediaPath, receiver);
  }

  std::vector<std::shared_ptr<IEntity>> getEntities() const { return entity_; }

protected:
  std::vector<std::shared_ptr<IEntity>> entity_;
  EntityManager entityManager_;

private:
};
