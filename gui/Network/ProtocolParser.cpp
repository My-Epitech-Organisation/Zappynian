/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** ProtocolParser
*/

#include "ProtocolParser.hpp"
#include <iostream>
#include <sstream>
#include <algorithm>

namespace Zappy {

    ProtocolParser::ProtocolParser(GameState& gameState, WorldScene& worldScene)
        : gameState_(gameState), worldScene_(worldScene) {}

    bool ProtocolParser::parseMessage(const std::string& message) {
        if (!isValidMessage(message)) {
            return false;
        }

        std::vector<std::string> tokens = tokenize(message);
        if (tokens.empty()) {
            return false;
        }

        const std::string& command = tokens[0];
        std::vector<std::string> args(tokens.begin() + 1, tokens.end());

        if (command == "msz") return parseMapSize(args);
        else if (command == "mct") return parseMapContent(args);
        else if (command == "bct") return parseTileContent(args);
        else if (command == "tna") return parseTeamName(args);
        else if (command == "pnw") return parsePlayerNew(args);
        else if (command == "ppo") return parsePlayerPosition(args);
        else if (command == "plv") return parsePlayerLevel(args);
        else if (command == "pin") return parsePlayerInventory(args);
        else if (command == "pex") return parsePlayerExpulsion(args);
        else if (command == "pbc") return parsePlayerBroadcast(args);
        else if (command == "pic") return parsePlayerIncantStart(args);
        else if (command == "pie") return parsePlayerIncantEnd(args);
        else if (command == "pfk") return parsePlayerFork(args);
        else if (command == "pdr") return parsePlayerResourceDrop(args);
        else if (command == "pgt") return parsePlayerResourceTake(args);
        else if (command == "pdi") return parsePlayerDeath(args);
        else if (command == "enw") return parseEggLaid(args);
        else if (command == "eht") return parseEggConnection(args);
        else if (command == "edi") return parseEggDeath(args);
        else if (command == "sgt") return parseServerTime(args);
        else if (command == "sst") return parseServerTimeSet(args);
        else if (command == "smg") return parseServerMessage(args);
        else if (command == "seg") return parseServerEnd(args);
        else {
            std::cout << "WARNING: Unknown command '" << command << "'" << std::endl;
            return false;
        }
    }

    void ProtocolParser::parseMessages(const std::vector<std::string>& messages) {
        for (const auto& message : messages) {
            parseMessage(message);
        }
    }

    bool ProtocolParser::parseMapSize(const std::vector<std::string>& args) {
        if (args.size() != 2) {
            std::cerr << "ERROR: Invalid msz arguments count: " << args.size() << std::endl;
            return false;
        }

        try {
            int width = std::stoi(args[0]);
            int height = std::stoi(args[1]);
            worldScene_.createPlane(width, height);
            return true;
        } catch (const std::exception& e) {
            std::cerr << "ERROR: Failed to parse msz: " << e.what() << std::endl;
            return false;
        }
    }

    bool ProtocolParser::parseMapContent(const std::vector<std::string>& args) {
        return true;
    }

    bool ProtocolParser::parseTileContent(const std::vector<std::string>& args) {
        if (args.size() != 9) {
            return false;
        }

        try {
            int x = std::stoi(args[0]);
            int y = std::stoi(args[1]);

            Inventory resources;
            resources.addItem("food", std::stoi(args[2]));
            resources.addItem("linemate", std::stoi(args[3]));
            resources.addItem("deraumere", std::stoi(args[4]));
            resources.addItem("sibur", std::stoi(args[5]));
            resources.addItem("mendiane", std::stoi(args[6]));
            resources.addItem("phiras", std::stoi(args[7]));
            resources.addItem("thystame", std::stoi(args[8]));
            worldScene_.createEntities(x, y, resources.getItemQuantity("food"),
                                       resources.getItemQuantity("linemate"),
                                       resources.getItemQuantity("deraumere"),
                                       resources.getItemQuantity("sibur"),
                                       resources.getItemQuantity("mendiane"),
                                       resources.getItemQuantity("phiras"),
                                       resources.getItemQuantity("thystame"));
            return true;
        } catch (const std::exception& e) {
            std::cerr << "ERROR: Failed to parse bct: " << e.what() << std::endl;
            return false;
        }
    }

    bool ProtocolParser::parseTeamName(const std::vector<std::string>& args) {
        if (args.size() != 1) {
            std::cerr << "ERROR: Invalid tna arguments count: " << args.size() << std::endl;
            return false;
        }

        worldScene_.addTeam(args[0]);
        return true;
    }

    bool ProtocolParser::parsePlayerNew(const std::vector<std::string>& args) {
        if (args.size() != 6) {
            std::cerr << "ERROR: Invalid pnw arguments count: " << args.size() << std::endl;
            return false;
        }

        try {
            int id = parseId(args[0]);
            int x = std::stoi(args[1]);
            int y = std::stoi(args[2]);
            Direction dir = parseDirection(args[3]);
            int level = std::stoi(args[4]);
            std::string team = args[5];

            worldScene_.createEntities(id, x, y, dir, level, team);

            if (onPlayerConnected_) {
                onPlayerConnected_("Player " + std::to_string(id) + " connected");
            }

            return true;
        } catch (const std::exception& e) {
            std::cerr << "ERROR: Failed to parse pnw: " << e.what() << " (args: ";
            for (const auto& a : args) std::cerr << a << ", ";
            std::cerr << ")" << std::endl;
            return false;
        }
    }

    bool ProtocolParser::parsePlayerPosition(const std::vector<std::string>& args) {
        if (args.size() != 4) {
            std::cerr << "ERROR: Invalid ppo arguments count: " << args.size() << std::endl;
            return false;
        }

        try {
            int id = parseId(args[0]);
            int x = std::stoi(args[1]);
            int y = std::stoi(args[2]);
            Direction dir = parseDirection(args[3]);

            std::cout << "DEBUG: Updating player " << id << " position to (" << x << ", " << y << ") with direction " << static_cast<int>(dir) << std::endl;
            worldScene_.changePlayerPos(id, x, y, dir, dir);
            return true;
        } catch (const std::exception& e) {
            std::cerr << "ERROR: Failed to parse ppo: " << e.what() << std::endl;
            return false;
        }
    }

    bool ProtocolParser::parsePlayerLevel(const std::vector<std::string>& args) {
        if (args.size() != 2) {
            std::cerr << "ERROR: Invalid plv arguments count: " << args.size() << std::endl;
            return false;
        }

        try {
            int id = parseId(args[0]);
            int level = std::stoi(args[1]);

            worldScene_.setPlayerLevel(id, level);
            return true;
        } catch (const std::exception& e) {
            std::cerr << "ERROR: Failed to parse plv: " << e.what() << std::endl;
            return false;
        }
    }

    bool ProtocolParser::parsePlayerInventory(const std::vector<std::string>& args) {
        if (args.size() != 10) {
            std::cerr << "ERROR: Invalid pin arguments count: " << args.size() << std::endl;
            return false;
        }

        try {
            int id = parseId(args[0]);
            int x = std::stoi(args[1]);
            int y = std::stoi(args[2]);

            Inventory inventory = parseInventoryArgs(args, 3);
            worldScene_.setPlayerInventory(id, x, y, inventory.getItemQuantity("food"),
                                           inventory.getItemQuantity("linemate"),
                                           inventory.getItemQuantity("deraumere"),
                                           inventory.getItemQuantity("sibur"),
                                           inventory.getItemQuantity("mendiane"),
                                           inventory.getItemQuantity("phiras"),
                                           inventory.getItemQuantity("thystame"));
            return true;
        } catch (const std::exception& e) {
            std::cerr << "ERROR: Failed to parse pin: " << e.what() << std::endl;
            return false;
        }
    }

    bool ProtocolParser::parsePlayerExpulsion(const std::vector<std::string>& args) {
        if (args.size() != 1) {
            std::cerr << "ERROR: Invalid pex arguments count: " << args.size() << std::endl;
            return false;
        }

        try {
            int id = parseId(args[0]);
            worldScene_.expulsion(id);
            return true;
        } catch (const std::exception& e) {
            std::cerr << "ERROR: Failed to parse pex: " << e.what() << std::endl;
            return false;
        }
    }

    bool ProtocolParser::parsePlayerBroadcast(const std::vector<std::string>& args) {
        if (args.size() < 2) {
            std::cerr << "ERROR: Invalid pbc arguments count: " << args.size() << std::endl;
            return false;
        }

        try {
            int id = parseId(args[0]);
            std::string message = args[1];
            for (size_t i = 2; i < args.size(); ++i) {
                message += " " + args[i];
            }

            // if (onBroadcast_) {
            //     onBroadcast_("Player " + std::to_string(id) + ": " + message);
            // }
            worldScene_.broadcast(id, message);
            return true;
        } catch (const std::exception& e) {
            std::cerr << "ERROR: Failed to parse pbc: " << e.what() << std::endl;
            return false;
        }
    }

    bool ProtocolParser::parsePlayerIncantStart(const std::vector<std::string>& args) {
        if (args.size() < 4) {
            std::cerr << "ERROR: Invalid pic arguments count: " << args.size() << std::endl;
            return false;
        }

        try {
            int x = std::stoi(args[0]);
            int y = std::stoi(args[1]);
            int level = std::stoi(args[2]);

            std::vector<int> playerIds;
            for (size_t i = 3; i < args.size(); ++i) {
                playerIds.push_back(parseId(args[i]));
            }
            worldScene_.startIncantation(x, y, level, playerIds);
            return true;
        } catch (const std::exception& e) {
            std::cerr << "ERROR: Failed to parse pic: " << e.what() << std::endl;
            return false;
        }
    }

    bool ProtocolParser::parsePlayerIncantEnd(const std::vector<std::string>& args) {
        if (args.size() < 3) {
            std::cerr << "ERROR: Invalid pie arguments count: " << args.size() << std::endl;
            return false;
        }

        try {
            int x = std::stoi(args[0]);
            int y = std::stoi(args[1]);
            bool success = (args[2] == "1");

            worldScene_.stopIncantation(x, y, success);
            return true;
        } catch (const std::exception& e) {
            std::cerr << "ERROR: Failed to parse pie: " << e.what() << std::endl;
            return false;
        }
    }

    bool ProtocolParser::parsePlayerFork(const std::vector<std::string>& args) {
        if (args.size() != 1) {
            std::cerr << "ERROR: Invalid pfk arguments count: " << args.size() << std::endl;
            return false;
        }

        try {
            int id = parseId(args[0]);
            // worldScene_.createEntities(id);
            return true;
        } catch (const std::exception& e) {
            std::cerr << "ERROR: Failed to parse pfk: " << e.what() << std::endl;
            return false;
        }
    }

    bool ProtocolParser::parsePlayerResourceDrop(const std::vector<std::string>& args) {
        if (args.size() != 2) {
            std::cerr << "ERROR: Invalid pdr arguments count: " << args.size() << std::endl;
            return false;
        }

        try {
            int id = parseId(args[0]);
            std::string resource = args[1];
            worldScene_.resourceDropping(id, resource);
            return true;
        } catch (const std::exception& e) {
            std::cerr << "ERROR: Failed to parse pdr: " << e.what() << std::endl;
            return false;
        }
    }

    bool ProtocolParser::parsePlayerResourceTake(const std::vector<std::string>& args) {
        if (args.size() != 2) {
            std::cerr << "ERROR: Invalid pgt arguments count: " << args.size() << std::endl;
            return false;
        }

        try {
            int id = parseId(args[0]);
            std::string resource = args[1];
            worldScene_.resourceCollect(id, resource);
            return true;
        } catch (const std::exception& e) {
            std::cerr << "ERROR: Failed to parse pgt: " << e.what() << std::endl;
            return false;
        }
    }

    bool ProtocolParser::parsePlayerDeath(const std::vector<std::string>& args) {
        if (args.size() != 1) {
            std::cerr << "ERROR: Invalid pdi arguments count: " << args.size() << std::endl;
            return false;
        }

        try {
            int id = parseId(args[0]);
            worldScene_.killPlayer(id);

            // if (onPlayerDisconnected_) {
            //     onPlayerDisconnected_("Player " + std::to_string(id) + " died");
            // }

            return true;
        } catch (const std::exception& e) {
            std::cerr << "ERROR: Failed to parse pdi: " << e.what() << std::endl;
            return false;
        }
    }

    bool ProtocolParser::parseEggLaid(const std::vector<std::string>& args) {
        if (args.size() != 4) {
            std::cerr << "ERROR: Invalid enw arguments count: " << args.size() << std::endl;
            return false;
        }

        try {
            std::string eggIdStr = args[0];
            int eggId;
            if (eggIdStr.front() == '#') {
                eggId = std::stoi(eggIdStr.substr(1));
            } else {
                eggId = std::stoi(eggIdStr);
            }

            std::string playerIdStr = args[1];
            // int playerId = -1;
            // if (playerIdStr.front() == '#') {
            //     std::string idStr = playerIdStr.substr(1);
            //     if (idStr != "-1") {
            //         playerId = std::stoi(idStr);
            //     }
            // } else {
            //     playerId = std::stoi(playerIdStr);
            // }

            // int x = std::stoi(args[2]);
            // int y = std::stoi(args[3]);

            // std::string team = "unknown";
            // if (playerId >= 0) {
            //     const PlayerEntity* player = worldScene_.getPlayer(playerId);
            //     if (player) {
            //         team = player->getTeam();
            //     }
            // }

            // worldScene_.addEgg(eggId, irr::core::vector3df(x, y, 0), team);
            return true;
        } catch (const std::exception& e) {
            std::cerr << "ERROR: Failed to parse enw: " << e.what() << std::endl;
            return false;
        }
    }

    bool ProtocolParser::parseEggConnection(const std::vector<std::string>& args) {
        if (args.size() != 1) {
            std::cerr << "ERROR: Invalid eht arguments count: " << args.size() << std::endl;
            return false;
        }

        try {
            std::string eggIdStr = args[0];
            int eggId;
            if (eggIdStr.front() == '#') {
                eggId = std::stoi(eggIdStr.substr(1));
            } else {
                eggId = std::stoi(eggIdStr);
            }

            // gameState_.setEggHatching(eggId, true);
            return true;
        } catch (const std::exception& e) {
            std::cerr << "ERROR: Failed to parse eht: " << e.what() << std::endl;
            return false;
        }
    }

    bool ProtocolParser::parseEggDeath(const std::vector<std::string>& args) {
        if (args.size() != 1) {
            std::cerr << "ERROR: Invalid edi arguments count: " << args.size() << std::endl;
            return false;
        }

        try {
            std::string eggIdStr = args[0];
            int eggId;
            if (eggIdStr.front() == '#') {
                eggId = std::stoi(eggIdStr.substr(1));
            } else {
                eggId = std::stoi(eggIdStr);
            }

            worldScene_.killEgg(eggId);
            return true;
        } catch (const std::exception& e) {
            std::cerr << "ERROR: Failed to parse edi: " << e.what() << std::endl;
            return false;
        }
    }

    bool ProtocolParser::parseServerTime(const std::vector<std::string>& args) {
        if (args.size() != 1) {
            std::cerr << "ERROR: Invalid sgt arguments count: " << args.size() << std::endl;
            return false;
        }

        try {
            int timeUnit = std::stoi(args[0]);
            // gameState_.setTimeUnit(timeUnit);
            return true;
        } catch (const std::exception& e) {
            std::cerr << "ERROR: Failed to parse sgt: " << e.what() << std::endl;
            return false;
        }
    }

    bool ProtocolParser::parseServerTimeSet(const std::vector<std::string>& args) {
        if (args.size() != 1) {
            std::cerr << "ERROR: Invalid sst arguments count: " << args.size() << std::endl;
            return false;
        }

        try {
            int timeUnit = std::stoi(args[0]);
            // gameState_.setTimeUnit(timeUnit);
            return true;
        } catch (const std::exception& e) {
            std::cerr << "ERROR: Failed to parse sst: " << e.what() << std::endl;
            return false;
        }
    }

    bool ProtocolParser::parseServerMessage(const std::vector<std::string>& args) {
        std::string message;
        for (size_t i = 0; i < args.size(); ++i) {
            if (i > 0) message += " ";
            message += args[i];
        }

        std::cout << "SERVER MESSAGE: " << message << std::endl;
        return true;
    }

    bool ProtocolParser::parseServerEnd(const std::vector<std::string>& args) {
        if (args.size() != 1) {
            std::cerr << "ERROR: Invalid seg arguments count: " << args.size() << std::endl;
            return false;
        }

        std::string winningTeam = args[0];
        std::cout << "GAME ENDED: Team '" << winningTeam << "' won!" << std::endl;

        // if (onGameEnd_) {
        //     onGameEnd_("Game ended - Team " + winningTeam + " won!");
        // }

        return true;
    }

    std::vector<std::string> ProtocolParser::tokenize(const std::string& message) const {
        std::vector<std::string> tokens;
        std::istringstream iss(message);
        std::string token;

        while (iss >> token) {
            tokens.push_back(token);
        }

        return tokens;
    }

    Direction ProtocolParser::parseDirection(const std::string& dirStr) const {
        if (dirStr == "NORTH" || dirStr == "1") return Direction::NORTH;
        if (dirStr == "EAST" || dirStr == "2") return Direction::EAST;
        if (dirStr == "SOUTH" || dirStr == "3") return Direction::SOUTH;
        if (dirStr == "WEST" || dirStr == "4") return Direction::WEST;

        std::cerr << "WARNING: Unknown direction '" << dirStr << "', defaulting to NORTH" << std::endl;
        return Direction::NORTH;
    }

    Inventory ProtocolParser::parseInventoryArgs(const std::vector<std::string>& args, size_t startIndex) const {
        Inventory inventory;
        if (args.size() >= startIndex + 7) {
            inventory.addItem("food", std::stoi(args[startIndex]));
            inventory.addItem("linemate", std::stoi(args[startIndex + 1]));
            inventory.addItem("deraumere", std::stoi(args[startIndex + 2]));
            inventory.addItem("sibur", std::stoi(args[startIndex + 3]));
            inventory.addItem("mendiane", std::stoi(args[startIndex + 4]));
            inventory.addItem("phiras", std::stoi(args[startIndex + 5]));
            inventory.addItem("thystame", std::stoi(args[startIndex + 6]));
        }
        return inventory;
    }

    std::vector<int> ProtocolParser::parsePlayerIds(const std::vector<std::string>& args, size_t startIndex) const {
        std::vector<int> playerIds;

        for (size_t i = startIndex; i < args.size(); ++i) {
            try {
                playerIds.push_back(std::stoi(args[i]));
            } catch (const std::exception& e) {
                std::cerr << "WARNING: Failed to parse player ID '" << args[i] << "'" << std::endl;
            }
        }

        return playerIds;
    }

    bool ProtocolParser::isValidMessage(const std::string& message) const {
        return !message.empty() && !std::all_of(message.begin(), message.end(), ::isspace);
    }

    int ProtocolParser::parseId(const std::string& idStr) const {
        if (!idStr.empty() && idStr.front() == '#')
            return std::stoi(idStr.substr(1));
        return std::stoi(idStr);
    }
}
