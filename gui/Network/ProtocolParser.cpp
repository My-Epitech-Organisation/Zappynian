/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** ProtocolParser - Implémentation du parser de protocole
*/

#include "ProtocolParser.hpp"
#include <iostream>
#include <sstream>
#include <algorithm>

namespace Zappy {

    ProtocolParser::ProtocolParser(GameState& gameState) : gameState_(gameState) {}

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

        logParsedMessage(command, args);

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
            gameState_.setMapSize(width, height);
            return true;
        } catch (const std::exception& e) {
            std::cerr << "ERROR: Failed to parse msz: " << e.what() << std::endl;
            return false;
        }
    }

    bool ProtocolParser::parseMapContent(const std::vector<std::string>& args) {
        std::cout << "DEBUG: Map content request received" << std::endl;
        return true;
    }

    bool ProtocolParser::parseTileContent(const std::vector<std::string>& args) {
        if (args.size() != 9) {
            std::cerr << "ERROR: Invalid bct arguments count: " << args.size() << std::endl;
            return false;
        }

        try {
            int x = std::stoi(args[0]);
            int y = std::stoi(args[1]);

            Inventory resources;
            resources.food = std::stoi(args[2]);
            resources.linemate = std::stoi(args[3]);
            resources.deraumere = std::stoi(args[4]);
            resources.sibur = std::stoi(args[5]);
            resources.mendiane = std::stoi(args[6]);
            resources.phiras = std::stoi(args[7]);
            resources.thystame = std::stoi(args[8]);

            gameState_.updateTile(Position(x, y), resources);
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

        gameState_.addTeam(args[0]);
        return true;
    }

    bool ProtocolParser::parsePlayerNew(const std::vector<std::string>& args) {
        if (args.size() != 6) {
            std::cerr << "ERROR: Invalid pnw arguments count: " << args.size() << std::endl;
            return false;
        }

        try {
            int id = std::stoi(args[0]);
            int x = std::stoi(args[1]);
            int y = std::stoi(args[2]);
            Direction dir = parseDirection(args[3]);
            int level = std::stoi(args[4]);
            std::string team = args[5];

            gameState_.addPlayer(id, Position(x, y), dir, level, team);

            if (onPlayerConnected_) {
                onPlayerConnected_("Player " + std::to_string(id) + " connected");
            }

            return true;
        } catch (const std::exception& e) {
            std::cerr << "ERROR: Failed to parse pnw: " << e.what() << std::endl;
            return false;
        }
    }

    bool ProtocolParser::parsePlayerPosition(const std::vector<std::string>& args) {
        if (args.size() != 4) {
            std::cerr << "ERROR: Invalid ppo arguments count: " << args.size() << std::endl;
            return false;
        }

        try {
            int id = std::stoi(args[0]);
            int x = std::stoi(args[1]);
            int y = std::stoi(args[2]);
            Direction dir = parseDirection(args[3]);

            gameState_.updatePlayerPosition(id, Position(x, y), dir);
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
            int id = std::stoi(args[0]);
            int level = std::stoi(args[1]);

            gameState_.updatePlayerLevel(id, level);
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
            int id = std::stoi(args[0]);
            int x = std::stoi(args[1]);
            int y = std::stoi(args[2]);

            Inventory inventory = parseInventoryArgs(args, 3);
            gameState_.updatePlayerInventory(id, Position(x, y), inventory);
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
            int id = std::stoi(args[0]);
            std::cout << "DEBUG: Player " << id << " was expelled" << std::endl;
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
            int id = std::stoi(args[0]);
            std::string message = args[1];
            for (size_t i = 2; i < args.size(); ++i) {
                message += " " + args[i];
            }

            if (onBroadcast_) {
                onBroadcast_("Player " + std::to_string(id) + ": " + message);
            }

            std::cout << "DEBUG: Player " << id << " broadcast: '" << message << "'" << std::endl;
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

            std::vector<int> playerIds = parsePlayerIds(args, 3);
            gameState_.startIncantation(Position(x, y), level, playerIds);
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

            gameState_.endIncantation(Position(x, y), success);
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
            int id = std::stoi(args[0]);
            std::cout << "DEBUG: Player " << id << " is forking" << std::endl;
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
            int id = std::stoi(args[0]);
            std::string resource = args[1];
            std::cout << "DEBUG: Player " << id << " dropped " << resource << std::endl;
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
            int id = std::stoi(args[0]);
            std::string resource = args[1];
            std::cout << "DEBUG: Player " << id << " took " << resource << std::endl;
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
            int id = std::stoi(args[0]);
            gameState_.removePlayer(id);

            if (onPlayerDisconnected_) {
                onPlayerDisconnected_("Player " + std::to_string(id) + " died");
            }

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
            int playerId = -1;
            if (playerIdStr.front() == '#') {
                std::string idStr = playerIdStr.substr(1);
                if (idStr != "-1") {
                    playerId = std::stoi(idStr);
                }
            } else {
                playerId = std::stoi(playerIdStr);
            }

            int x = std::stoi(args[2]);
            int y = std::stoi(args[3]);

            std::string team = "unknown";
            if (playerId >= 0) {
                const Player* player = gameState_.getPlayer(playerId);
                if (player) {
                    team = player->team;
                }
            }

            gameState_.addEgg(eggId, Position(x, y), team);
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

            gameState_.setEggHatching(eggId, true);
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

            gameState_.removeEgg(eggId);
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
            gameState_.setTimeUnit(timeUnit);
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
            gameState_.setTimeUnit(timeUnit);
            std::cout << "DEBUG: Server time unit set to " << timeUnit << std::endl;
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

        if (onGameEnd_) {
            onGameEnd_("Game ended - Team " + winningTeam + " won!");
        }

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

    ResourceType ProtocolParser::parseResource(const std::string& resourceStr) const {
        if (resourceStr == "food") return ResourceType::FOOD;
        if (resourceStr == "linemate") return ResourceType::LINEMATE;
        if (resourceStr == "deraumere") return ResourceType::DERAUMERE;
        if (resourceStr == "sibur") return ResourceType::SIBUR;
        if (resourceStr == "mendiane") return ResourceType::MENDIANE;
        if (resourceStr == "phiras") return ResourceType::PHIRAS;
        if (resourceStr == "thystame") return ResourceType::THYSTAME;

        std::cerr << "WARNING: Unknown resource '" << resourceStr << "'" << std::endl;
        return ResourceType::FOOD;
    }

    Inventory ProtocolParser::parseInventoryArgs(const std::vector<std::string>& args, size_t startIndex) const {
        Inventory inventory;

        if (args.size() >= startIndex + 7) {
            inventory.food = std::stoi(args[startIndex]);
            inventory.linemate = std::stoi(args[startIndex + 1]);
            inventory.deraumere = std::stoi(args[startIndex + 2]);
            inventory.sibur = std::stoi(args[startIndex + 3]);
            inventory.mendiane = std::stoi(args[startIndex + 4]);
            inventory.phiras = std::stoi(args[startIndex + 5]);
            inventory.thystame = std::stoi(args[startIndex + 6]);
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

    void ProtocolParser::logParsedMessage(const std::string& command, const std::vector<std::string>& args) const {
        std::cout << "DEBUG: Parsing '" << command << "'";
        for (const auto& arg : args) {
            std::cout << " '" << arg << "'";
        }
        std::cout << std::endl;
    }

}
