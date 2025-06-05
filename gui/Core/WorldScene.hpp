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
  WorldScene(irr::scene::ISceneManager *smgr, irr::video::IVideoDriver *driver,
             EventReceiver &receiver, const irr::io::path &mediaPath)
      : smgr_(smgr), driver_(driver), receiver_(receiver),
        mediaPath_(mediaPath),
        entityManager_(smgr, driver, receiver, mediaPath) {};
  ~WorldScene();

  void createEntities(int id, int x, int y, Direction direction, int level,
                      std::string team);

  void createEntities(int x, int y, int q0, int q1, int q2, int q3, int q4,
                      int q5, int q6, int nbTiles);

  void createLights();

  void createCamera();

  void createPlane(int x, int y);

  void createText();

  void createWorld() {
    createEntities(1, 0, 0, Direction::NORTH, 0, "Red");
    createPlane(10, 10);
    createEntities(2, 4, 16, 16, 16, 16, 16, 16, 16, 1);
    createLights();
    createCamera();
    createText();
  }

  std::vector<std::shared_ptr<IEntity>> getEntities() const { return entity_; }

protected:
  irr::scene::ISceneManager *smgr_;
  irr::video::IVideoDriver *driver_;
  EventReceiver &receiver_;
  irr::io::path mediaPath_;
  EntityManager entityManager_;
  std::vector<std::shared_ptr<IEntity>> entity_;

private:
};
