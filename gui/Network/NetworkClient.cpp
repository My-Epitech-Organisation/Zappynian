/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** NetworkClient
*/

#include "NetworkClient.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

void NetworkClient::parseMessage() {
  int id = 0, x = 0, y = 0, level = 0, q0 = 0, q1 = 0, q2 = 0, q3 = 0, q4 = 0,
      q5 = 0, q6 = 0, nbTiles = 0, nbArgs = 0;
  std::string team;
  Direction direction;
  initialiseSocket();
  if (sock_ == nullptr) {
    std::cerr << "Socket not initialized\n";
    return;
  }
  std::string message = readLine(sock_);
  std::istringstream iss(message);
  std::string command;
  std::string lastToken, token;
  iss >> command;
  std::istringstream iss_args(message);
  std::string tmp;
  iss_args >> tmp;

  while (iss_args >> tmp) {
    nbArgs++;
  }
  if (command == "pnw") {
    std::string dirStr;
    std::istringstream iss2(message);
    iss2 >> command;
    if (!(iss2 >> id >> x >> y >> dirStr >> level >> team)) {
      std::cerr << "Invalid arguments for pnw command\n";
      return;
    }
    if (dirStr == "NORTH")
      direction = Direction::NORTH;
    else if (dirStr == "EAST")
      direction = Direction::EAST;
    else if (dirStr == "SOUTH")
      direction = Direction::SOUTH;
    else if (dirStr == "WEST")
      direction = Direction::WEST;
    else
      direction = Direction::NORTH;
    createPlayer(id, x, y, direction, level, team);
  } else if (command == "bct" && nbArgs == 10) {
    std::istringstream iss2(message);
    iss2 >> command;
    if (!(iss2 >> x >> y >> q0 >> q1 >> q2 >> q3 >> q4 >> q5 >> q6 >>
          nbTiles)) {
      std::cerr << "Invalid arguments for bct (map) command\n";
      return;
    }
    contentMap(x, y, q0, q1, q2, q3, q4, q5, q6, nbTiles);
  } else if (command == "bct" && nbArgs == 9) {
    std::istringstream iss2(message);
    iss2 >> command;
    if (!(iss2 >> x >> y >> q0 >> q1 >> q2 >> q3 >> q4 >> q5 >> q6)) {
      std::cerr << "Invalid arguments for bct (tile) command\n";
      return;
    }
    contentTiles(x, y, q0, q1, q2, q3, q4, q5, q6);
  } else if (command == "msz") {
    std::istringstream iss2(message);
    iss2 >> command;
    if (!(iss2 >> x >> y)) {
      std::cerr << "Invalid arguments for msz command\n";
      return;
    }
    createPlane(x, y);
  } else if (command == "tna") {
    std::istringstream iss2(message);
    iss2 >> command;
    while (iss2 >> token) {
      addTeam(token);
    }
  } else if (command == "ppo") {
    std::string dirStr;
    std::istringstream iss2(message);
    iss2 >> command;
    if (!(iss2 >> id >> x >> y >> dirStr)) {
      std::cerr << "Invalid arguments for ppo command\n";
      return;
    }
    if (dirStr == "NORTH")
      direction = Direction::NORTH;
    else if (dirStr == "EAST")
      direction = Direction::EAST;
    else if (dirStr == "SOUTH")
      direction = Direction::SOUTH;
    else if (dirStr == "WEST")
      direction = Direction::WEST;
    else
      direction = Direction::NORTH;
    changePlayerPos(id, x, y, direction);
  } else if (command == "pin") {
    std::istringstream iss2(message);
    iss2 >> command;
    if (!(iss2 >> id >> x >> y >> q0 >> q1 >> q2 >> q3 >> q4 >> q5 >> q6)) {
      std::cerr << "Invalid arguments for pin command\n";
      return;
    }
    setPlayerInventory(id, x, y, q0, q1, q2, q3, q4, q5, q6);
  } else if (command == "sgt") {
    std::istringstream iss2(message);
    iss2 >> command;
    if (!(iss2 >> level)) {
      std::cerr << "Invalid arguments for sgt command\n";
      return;
    }
  } else if (command == "sst") {
    std::istringstream iss2(message);
    iss2 >> command;
  } else if (command == "smg") {
    std::istringstream iss2(message);
    iss2 >> command;
  } else if (command == "enw") {
    std::istringstream iss2(message);
  }
}


const std::string NetworkClient::readLine(zn_socket_t sock) {
  char buffer[1024];
  ssize_t bytesRead = zn_readln(sock, buffer, sizeof(buffer) - 1);
  if (bytesRead < 0) {
    std::cerr << "Error reading from socket\n";
    return "";
  }
  buffer[bytesRead] = '\0';
  return std::string(buffer);
}
