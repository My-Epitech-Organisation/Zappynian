/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** WorldScene
*/
#include "EntityManager.hpp"
#include <iostream>
#include <queue>
#pragma once

struct Movement {
  int id;
  int x;
  int y;
  Direction direction;
};

class WorldScene {
public:
  WorldScene(irr::IrrlichtDevice* device, irr::scene::ISceneManager *smgr, irr::video::IVideoDriver *driver,
             EventReceiver &receiver, const irr::io::path &mediaPath)
      : device_(device), smgr_(smgr), driver_(driver), receiver_(receiver),
        mediaPath_(mediaPath),
        entityManager_(smgr, driver, receiver, mediaPath) {};
  ~WorldScene();

  void createEntities(int id, int x, int y, Direction direction, int level,
                      std::string team);

  void createEntities(int x, int y, int q0, int q1, int q2, int q3, int q4,
                      int q5, int q6, int nbTiles);

  void createEntities(int x, int y, int q0, int q1, int q2, int q3, int q4,
                      int q5, int q6);

  void changePlayerPos(int id, int x, int y, Direction direction);

  void createLights();

  void createCamera();

  void createPlane(int x, int y);

  void createText();

  void createWorld() {
    createPlane(10, 10);
    createEntities(1, 5, 5, Direction::NORTH, 0, "Red");
    createEntities(2, 4, 32, 32, 32, 32, 32, 32, 32);
    createEntities(9, 9, 32, 32, 32, 32, 32, 32, 32);
    // createEntities(9, 9, 32, 32, 32, 32, 32, 32, 32, 30);
    createLights();
    createCamera();
    createText();
    changePlayerPos(1, 5, 4, Direction::NORTH);
    changePlayerPos(1, 5, 3, Direction::NORTH);
    changePlayerPos(1, 5, 2, Direction::NORTH);
    changePlayerPos(1, 5, 1, Direction::NORTH);
    changePlayerPos(1, 5, 0, Direction::NORTH);
    changePlayerPos(1, 5, 0, Direction::EAST);
  }

  std::vector<std::shared_ptr<IEntity>> getEntities() const { return entity_; }

  void updateMovements();

protected:
  irr::IrrlichtDevice* device_;
  irr::scene::ISceneManager *smgr_;
  irr::video::IVideoDriver *driver_;
  EventReceiver &receiver_;
  irr::io::path mediaPath_;
  EntityManager entityManager_;
  std::vector<std::shared_ptr<IEntity>> entity_;
  std::queue<Movement> movementQueue_;

private:
};
