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
  // Constructeur qui accepte un NetworkManager existant
  NetworkClient(irr::IrrlichtDevice *device, irr::scene::ISceneManager *smgr,
                irr::video::IVideoDriver *driver, EventReceiver &receiver,
                const irr::io::path &mediaPath,
                NetworkManager& networkManager)
      : WorldScene(device, smgr, driver, receiver, mediaPath),
        networkManager_(networkManager) {}

  // Ancien constructeur pour compatibilité (deprecated)
  NetworkClient(irr::IrrlichtDevice *device, irr::scene::ISceneManager *smgr,
                irr::video::IVideoDriver *driver, EventReceiver &receiver,
                const irr::io::path &mediaPath,
                const std::string &host, int port)
      : WorldScene(device, smgr, driver, receiver, mediaPath),
        networkManager_(ownNetworkManager_), host_(host), port_(port) {
    // Pour la compatibilité avec l'ancien code, mais pas recommandé
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
    // Pour le nouveau constructeur, le NetworkManager est déjà connecté
    // Pas besoin de refaire la connexion
    if (&networkManager_ == &ownNetworkManager_) {
      // Cas de l'ancien constructeur - faire la connexion
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

  void createWorld() override {
    // createPlane(5, 5);
    parseMessage();
    createLights();
    createCamera();
    createText();
    // addTeam("Red");
    // addTeam("Blue");
    // addTeam("Yellow");
    // addTeam("Purple");
    // addTeam("Orange");
    // addTeam("Green");
    // try {
    //   createPlayer(1, 0, 0, Direction::NORTH, 0, "Red");
    // } catch (const std::exception &e) {
    // }
    // setPlayerLevel(1, 1);
    // try {
    //   createPlayer(2, 1, 0, Direction::NORTH, 0, "Blue");
    // } catch (const std::exception &e) {
    // }
    // setPlayerLevel(2, 2);
    // try {
    //   createPlayer(3, 2, 0, Direction::NORTH, 0, "Red");
    // } catch (const std::exception &e) {
    // }
    // setPlayerLevel(3, 3);

    // try {
    //   createPlayer(4, 3, 0, Direction::NORTH, 0, "Yellow");
    // } catch (const std::exception &e) {
    // }
    // setPlayerLevel(4, 4);
    // try {
    //   createPlayer(5, 4, 0, Direction::NORTH, 0, "Purple");
    // } catch (const std::exception &e) {
    // }
    // setPlayerLevel(5, 5);
    // try {
    //   createPlayer(6, 0, 1, Direction::NORTH, 0, "Orange");
    // } catch (const std::exception &e) {
    // }
    // setPlayerLevel(6, 6);
    // try {
    //   createPlayer(7, 0, 2, Direction::NORTH, 0, "Green");
    // } catch (const std::exception &e) {
    // }
    // setPlayerLevel(7, 7);
    // try {
    //   createPlayer(8, 0, 3, Direction::NORTH, 0, "Green");
    // } catch (const std::exception &e) {
    // }
    // setPlayerLevel(8, 8);

    // contentTiles(2, 4, 32, 32, 32, 32, 32, 32, 32);
    // contentTiles(4, 4, 32, 32, 32, 32, 32, 32, 32);
    // movePlayer(1, 2, 2, Direction::NORTH);
    // movePlayer(1, 2, 1, Direction::NORTH);
    // movePlayer(1, 2, 0, Direction::NORTH);
    // movePlayer(1, 2, 4, Direction::NORTH);
    // movePlayer(1, 1, 4, Direction::EAST);
    // movePlayer(1, 0, 4, Direction::EAST);
    // movePlayer(1, 4, 4, Direction::EAST);
    // movePlayer(1, 4, 0, Direction::SOUTH);
    // movePlayer(1, 4, 1, Direction::SOUTH);
    // movePlayer(1, 4, 2, Direction::SOUTH);
    // movePlayer(1, 0, 2, Direction::WEST);
    // movePlayer(2, 4, 2, Direction::NORTH);
    // PlayerInventory(1, 0, 2, 1, 1, 1, 14, 1, 1, 1);
    // startIncantation(4, 2, 2, {2});
    // // stopIncantation(4, 2, true);
    // // killPlayer(3);
    // broadcast(1, "Hello from player 1!");
    // broadcast(2, "Hello from player 2!");
  }

  std::vector<std::shared_ptr<IEntity>> getEntities() {
    return WorldScene::getEntities();
  }

  private:
   NetworkManager& networkManager_;  // Référence au NetworkManager à utiliser
   NetworkManager ownNetworkManager_; // Instance propre pour la compatibilité
   std::string host_;
   int port_;
};
