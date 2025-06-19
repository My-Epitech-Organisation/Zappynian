/*
** EPITECH PROJECT, 2025
** testparshme
** File description:
** main
*/

#include "../App/Game.hpp"
#include <cmath>
#include <irrlicht/irrlicht.h>
#include <vector>
#include <string>
#include <iostream>

int main(int argc, char **argv) {
  if (argc < 5) {
    std::cerr << "Usage: " << argv[0] << "-p <port> -h <host>\n";
    return -1;
  }
  if (std::string(argv[3]) != "-h" || std::string(argv[1]) != "-p") {
    std::cerr << "Usage: " << argv[0] << "-p <port> -h <host>\n";
    return -1;
  }
  if (std::string(argv[2]).empty() || std::string(argv[4]).empty()) {
    std::cerr << "Host and port must not be empty.\n";
    return -1;
  }
  if (std::stoi(argv[2]) <= 0 || std::stoi(argv[2]) > 65535) {
    std::cerr << "Port must be a valid number between 1 and 65535.\n";
    return -1;
  }
  if (argc > 5) {
    std::cerr << "Too many arguments provided.\n";
    return -1;
  }
  if (argc == 3 && std::string(argv[1]) == "-help") {
    std::cout << "Usage: " << argv[0] << "-p port -h machine\n";
  }
  std::string host = argv[4];
  int port = std::stoi(argv[2]);
  Game game(host, port);

  game.gameLoop();
  return 0;
}
