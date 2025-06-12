/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** NetworkClient
*/

#pragma once
#include "../Core/WorldScene.hpp"
#include <chrono>
#include <iostream>
#include <thread>

class NetworkClient : public WorldScene {
public:
  NetworkClient(irr::IrrlichtDevice *device, irr::scene::ISceneManager *smgr,
                irr::video::IVideoDriver *driver, EventReceiver &receiver,
                const irr::io::path &mediaPath)
      : WorldScene(device, smgr, driver, receiver, mediaPath) {}

  void parseMessage(const std::string &message);

  void createPlayer(int id, int x, int y, Direction direction, int level,
                    std::string team) {
    WorldScene::createEntities(id, x, y, direction, level, team);
  }

  void contentMap(int x, int y, int q0, int q1, int q2, int q3, int q4, int q5,
                  int q6, int nbTiles) {
    WorldScene::createEntities(x, y, q0, q1, q2, q3, q4, q5, q6, nbTiles);
  }

  void contentTiles(int x, int y, int q0, int q1, int q2, int q3, int q4,
                    int q5, int q6) {
    WorldScene::createEntities(x, y, q0, q1, q2, q3, q4, q5, q6);
  }

  void createPlane(int x, int y) { WorldScene::createPlane(x, y); }

  void createLights() { WorldScene::createLights(); }

  void createCamera() { WorldScene::createCamera(); }

  void createText() { WorldScene::createText(); }

  void movePlayer(int id, int x, int y, Direction direction) {
    WorldScene::changePlayerPos(id, x, y, direction);
  }

  void PlayerInventory(int id, int x, int y, int q0, int q1, int q2, int q3,
                       int q4, int q5, int q6) {
    WorldScene::setPlayerInventory(id, x, y, q0, q1, q2, q3, q4, q5, q6);
  }

  void createWorld() override {
    createPlane(5, 5);
    createLights();
    createCamera();
    createText();
    try {
      createPlayer(1, 2, 2, Direction::NORTH, 0, "Red");
    } catch (const std::exception &e) {
    }
    try {
      createPlayer(2, 4, 2, Direction::NORTH, 0, "Blue");
    } catch (const std::exception &e) {
    }
    try {
      createPlayer(3, 0, 0, Direction::SOUTH, 0, "Red");
    } catch (const std::exception &e) {
    }
    contentTiles(2, 4, 0, 32, 32, 32, 32, 32, 32);
    contentTiles(4, 4, 32, 32, 32, 0, 32, 32, 32);
    contentTiles(3, 3, 0, 0, 32, 0, 32, 0, 32);
    movePlayer(1, 2, 2, Direction::NORTH);
    movePlayer(1, 2, 1, Direction::NORTH);
    movePlayer(1, 2, 0, Direction::NORTH);
    movePlayer(1, 2, 4, Direction::NORTH);
    movePlayer(1, 1, 4, Direction::EAST);
    movePlayer(1, 0, 4, Direction::EAST);
    movePlayer(1, 4, 4, Direction::EAST);
    movePlayer(1, 4, 0, Direction::SOUTH);
    movePlayer(1, 4, 1, Direction::SOUTH);
    movePlayer(1, 4, 2, Direction::SOUTH);
    movePlayer(1, 0, 2, Direction::WEST);
    // movePlayer(2, 4, 2, Direction::NORTH);
    PlayerInventory(1, 0, 2, 1, 1, 1, 14, 1, 1, 1);
    PlayerInventory(3, 0, 0, 10, 10, 10, 10, 10, 10, 10);
    startIncantation(4, 2, 2, {2});
    // stopIncantation(4, 2, true);
    resourceDroping(3, "mendiane");
    resourceDroping(3, "mendiane");
    resourceDroping(3, "mendiane");
  }

  std::vector<std::shared_ptr<IEntity>> getEntities() {
    return WorldScene::getEntities();
  }
};
