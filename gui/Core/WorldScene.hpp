/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** WorldScene
*/
#include "EntityManager.hpp"
#include <iostream>
#include <queue>
#include <unordered_map>
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
  virtual ~WorldScene() = default;

  void createEntities(int id, int x, int y, Direction direction, int level,
                      std::string team);

  void setPlayerInventory(int id, int x, int y, int q0, int q1, int q2, int q3,
                          int q4, int q5, int q6);

  void setPlayerLevel(int id, int level);

  void createEntities(int x, int y, int q0, int q1, int q2, int q3, int q4,
                      int q5, int q6, int nbTiles);

  void createEntities(int x, int y, int q0, int q1, int q2, int q3, int q4,
                      int q5, int q6);

  void createEntities(int id);

  void changePlayerPos(int id, int x, int y, Direction direction);
  EdgePositionResult isEdgePosition(const irr::core::vector3df &position,
                                    Direction direction, int nextX, int nextY);

  void updateMovements();

  void createLights();

  void createCamera();

  void createPlane(int x, int y);

  void createText();

  virtual void createWorld();

  void startIncantation(int x, int y, int level, std::vector<int> ids);
  void stopIncantation(int x, int y, bool result);
  bool isPlayerIncanting(int id) const {
    return isIncanting_.count(id) > 0 && isIncanting_.at(id);
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
  std::unordered_map<int, bool> isIncanting_;
  std::vector<std::tuple<int, int, int>> incantationData_; // x, y, id

private:
};
