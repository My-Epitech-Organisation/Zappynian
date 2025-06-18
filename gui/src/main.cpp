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
  if (argc < 3) {
    std::cerr << "Usage: " << argv[0] << " <host> <port>" << std::endl;
    return 1;
  }
  std::string host = argv[1];
  int port = std::stoi(argv[2]);
  Game game(host, port);

  game.gameLoop();
  return 0;
}
