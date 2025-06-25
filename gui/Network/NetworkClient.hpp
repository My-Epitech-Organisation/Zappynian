/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** NetworkClient
*/

#pragma once
#include "../Core/WorldScene.hpp"
#include "NetworkManager.hpp"
#include <chrono>
#include <iostream>
#include <thread>

class NetworkClient : public WorldScene {
public:
  NetworkClient(irr::IrrlichtDevice *device, irr::scene::ISceneManager *smgr,
                irr::video::IVideoDriver *driver, EventReceiver &receiver,
                const irr::io::path &mediaPath,
                NetworkManager& networkManager)
      : WorldScene(device, smgr, driver, receiver, mediaPath),
        networkManager_(networkManager) {}

  NetworkClient(irr::IrrlichtDevice *device, irr::scene::ISceneManager *smgr,
                irr::video::IVideoDriver *driver, EventReceiver &receiver,
                const irr::io::path &mediaPath,
                const std::string &host, int port)
      : WorldScene(device, smgr, driver, receiver, mediaPath),
        networkManager_(ownNetworkManager_), host_(host), port_(port) {
  }

  void parseMessage(void);

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

  void movePlayer(int id, int x, int y, Direction direction,
                  Direction directionBefore) {
    WorldScene::changePlayerPos(id, x, y, direction, directionBefore);
  }

  void PlayerInventory(int id, int x, int y, int q0, int q1, int q2, int q3,
                       int q4, int q5, int q6) {
    WorldScene::setPlayerInventory(id, x, y, q0, q1, q2, q3, q4, q5, q6);
  }

  void startIncantation(int x, int y, int level, std::vector<int> ids) {
    WorldScene::startIncantation(x, y, level, ids);
  }

  void stopIncantation(int x, int y, bool result) {
    WorldScene::stopIncantation(x, y, result);
  }

  bool isPlayerIncanting(int id) const {
    return WorldScene::isPlayerIncanting(id);
  }

  void createEgg(int id) { WorldScene::createEntities(id); }

  bool initialiseSocket() {

    if (&networkManager_ == &ownNetworkManager_) {
      if (!networkManager_.connect(host_, port_)) {
        std::cerr << "Failed to connect to server at " << host_ << ":" << port_
                  << " - " << networkManager_.getLastError() << "\n";
        return false;
      }

      if (!networkManager_.performHandshake()) {
        std::cerr << "Failed to perform handshake - " << networkManager_.getLastError() << "\n";
        return false;
      }
    }

    return true;
  }

  const std::string readLine();

  std::vector<std::shared_ptr<IEntity>> getEntities() {
    return WorldScene::getEntities();
  }

  private:
   NetworkManager& networkManager_;
   NetworkManager ownNetworkManager_;
   std::string host_;
   int port_;
};
