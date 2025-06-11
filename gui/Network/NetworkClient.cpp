/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** NetworkClient
*/

#include "NetworkClient.hpp"
#include <sstream>
#include <iostream>
#include <string>
#include <vector>

void NetworkClient::parseMessage(const std::string &message) {
    int id = 0, x = 0, y = 0, level = 0, q0 = 0, q1 = 0, q2 = 0, q3 = 0, q4 = 0, q5 = 0, q6 = 0, nbTiles = 0, nbArgs = 0;
    std::string team;
    Direction direction;
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
        if (!(iss2 >> x >> y >> q0 >> q1 >> q2 >> q3 >> q4 >> q5 >> q6 >> nbTiles)) {
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
    }
}
