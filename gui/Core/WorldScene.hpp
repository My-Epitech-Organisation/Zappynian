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

struct EdgePositionResult {
  bool isEdge;
  float offsetX;
  float offsetZ;
  irr::core::vector3df nextPosition;
};

class WorldScene {
public:
  WorldScene(irr::IrrlichtDevice *device, irr::scene::ISceneManager *smgr,
             irr::video::IVideoDriver *driver, EventReceiver &receiver,
             const irr::io::path &mediaPath)
      : device_(device), smgr_(smgr), driver_(driver), receiver_(receiver),
        mediaPath_(mediaPath),
        entityManager_(smgr, driver, receiver, mediaPath) {};
  ~WorldScene();

  void createEntities(int id, int x, int y, Direction direction, int level,
                      std::string team);

  void setPlayerLevel(int id, int level);

  void createEntities(int x, int y, int q0, int q1, int q2, int q3, int q4,
                      int q5, int q6, int nbTiles);

  void createEntities(int x, int y, int q0, int q1, int q2, int q3, int q4,
                      int q5, int q6);

  void changePlayerPos(int id, int x, int y, Direction direction);
  EdgePositionResult isEdgePosition(const irr::core::vector3df &position,
                                    Direction direction, int nextX, int nextY);

  void updateMovements();

  void createLights();

  void createCamera();

  void createPlane(int x, int y);

  void createText();

  void updatePlayersInfos();

  void createWorld() {
    createText();
    createPlane(5, 5);
    createEntities(1, 2, 2, Direction::NORTH, 0, "Red");
    createEntities(2, 4, 2, Direction::NORTH, 0, "Blue");
    createEntities(2, 4, 32, 32, 32, 32, 32, 32, 32);
    createEntities(4, 4, 32, 32, 32, 32, 32, 32, 32);
    // createEntities(9, 9, 32, 32, 32, 32, 32, 32, 32, 30);
    createLights();
    createCamera();
    changePlayerPos(1, 2, 2, Direction::NORTH);
    changePlayerPos(1, 2, 1, Direction::NORTH);
    changePlayerPos(1, 2, 0, Direction::NORTH);
    changePlayerPos(1, 2, 4, Direction::NORTH);
    changePlayerPos(1, 1, 4, Direction::EAST);
    changePlayerPos(1, 0, 4, Direction::EAST);
    changePlayerPos(1, 4, 4, Direction::EAST);
    changePlayerPos(1, 4, 0, Direction::SOUTH);
    changePlayerPos(1, 4, 1, Direction::SOUTH);
    changePlayerPos(1, 4, 2, Direction::SOUTH);
    changePlayerPos(1, 0, 2, Direction::WEST);
    updatePlayersInfos();
  }

  std::vector<std::shared_ptr<IEntity>> getEntities() const { return entity_; }

protected:
  irr::IrrlichtDevice *device_;
  irr::scene::ISceneManager *smgr_;
  irr::video::IVideoDriver *driver_;
  EventReceiver &receiver_;
  irr::io::path mediaPath_;
  EntityManager entityManager_;
  std::vector<std::shared_ptr<IEntity>> entity_;
  std::queue<Movement> movementQueue_;
  irr::core::vector3df actualPos_;
  std::pair<int, int> planeSize_;
  irr::gui::IGUIStaticText *playersInfos_;

private:
};
