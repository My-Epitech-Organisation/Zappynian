/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** WorldScene
*/
#include "../Entities/Direction.hpp"
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
  Direction directionBefore;
};

struct PaperPlaneMovement {
  std::shared_ptr<IEntity> paperPlane;
  int targetPlayerId;
  irr::core::vector3df startPosition;
  irr::core::vector3df targetPosition;
  irr::u32 startTime;
  float duration; // sec
  bool isActive;
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

  void changeHeadLevel(std::shared_ptr<IEntity> &entity, int level);

  void createEntities(int x, int y, int q0, int q1, int q2, int q3, int q4,
                      int q5, int q6, int nbTiles);

  void createEntities(int x, int y, int q0, int q1, int q2, int q3, int q4,
                      int q5, int q6);

  void createEntities(int id);

  void changePlayerPos(int id, int x, int y, Direction direction,
                       Direction directionBefore);

  EdgePositionResult isEdgePosition(const irr::core::vector3df &position,
                                    Direction direction, int nextX, int nextY);

  void updateMovements();

  void updatePaperPlaneMovements();

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

  void killPlayer(int id);

  void addChatMessage(const std::string &message);

  void broadcast(int id, const std::string &message);

  void addTeam(const std::string &team) { teams_.push_back(team); }

  void clearElements();

  void endGame(std::string winner);

  std::vector<std::shared_ptr<IEntity>>
  getPlayersEggAtPosition(irr::core::vector3df position);

  void expulsionMove(int id, irr::f32 direction, Direction directionEnum,
                     irr::core::vector3df pos);

  void expulsion(int id);

  void resourceDroping(int id, const std::string &item);

  void resourceCollect(int id, const std::string &item);

protected:
  irr::IrrlichtDevice *device_;
  irr::scene::ISceneManager *smgr_;
  irr::video::IVideoDriver *driver_;
  EventReceiver &receiver_;
  irr::io::path mediaPath_;
  EntityManager entityManager_;
  std::vector<std::shared_ptr<IEntity>> entity_;
  std::queue<Movement> movementQueue_;
  std::vector<PaperPlaneMovement> paperPlaneMovements_;
  irr::core::vector3df actualPos_;
  std::pair<int, int> planeSize_;
  std::unordered_map<int, bool> isIncanting_;
  std::vector<std::tuple<int, int, int>> incantationData_; // x, y, id
  irr::gui::IGUIStaticText *textChat_;
  std::vector<std::string> chatMessages_;
  static const size_t MAX_CHAT_MESSAGES = 20;
  std::vector<std::string> teams_;

private:
  void updateChatDisplay();
};
