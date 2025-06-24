/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** GameState - Modèle local synchronisé avec l'état du serveur
*/

#pragma once

#include <string>
#include <vector>
#include <map>
#include <memory>

namespace Zappy {

    enum class Direction {
        NORTH = 1,
        EAST = 2,
        SOUTH = 3,
        WEST = 4
    };

    enum class ResourceType {
        FOOD = 0,
        LINEMATE = 1,
        DERAUMERE = 2,
        SIBUR = 3,
        MENDIANE = 4,
        PHIRAS = 5,
        THYSTAME = 6
    };

    struct Position {
        int x, y;
        Position(int x = 0, int y = 0) : x(x), y(y) {}
        bool operator==(const Position& other) const {
            return x == other.x && y == other.y;
        }
    };

    struct Inventory {
        int food = 0;
        int linemate = 0;
        int deraumere = 0;
        int sibur = 0;
        int mendiane = 0;
        int phiras = 0;
        int thystame = 0;

        void setResource(ResourceType type, int value) {
            switch (type) {
                case ResourceType::FOOD: food = value; break;
                case ResourceType::LINEMATE: linemate = value; break;
                case ResourceType::DERAUMERE: deraumere = value; break;
                case ResourceType::SIBUR: sibur = value; break;
                case ResourceType::MENDIANE: mendiane = value; break;
                case ResourceType::PHIRAS: phiras = value; break;
                case ResourceType::THYSTAME: thystame = value; break;
            }
        }

        int getResource(ResourceType type) const {
            switch (type) {
                case ResourceType::FOOD: return food;
                case ResourceType::LINEMATE: return linemate;
                case ResourceType::DERAUMERE: return deraumere;
                case ResourceType::SIBUR: return sibur;
                case ResourceType::MENDIANE: return mendiane;
                case ResourceType::PHIRAS: return phiras;
                case ResourceType::THYSTAME: return thystame;
                default: return 0;
            }
        }
    };

    struct Player {
        int id;
        Position pos;
        Direction direction;
        int level;
        std::string team;
        Inventory inventory;
        bool isAlive = true;
        bool isIncanting = false;

        Player(int id, Position pos, Direction dir, int level, const std::string& team)
            : id(id), pos(pos), direction(dir), level(level), team(team) {}
    };

    struct Tile {
        Position pos;
        Inventory resources;
        std::vector<int> playerIds; // Players standing on this tile
        int eggCount = 0;

        Tile(Position pos) : pos(pos) {}
    };

    struct Egg {
        int id;
        Position pos;
        std::string team;
        bool isHatching = false;

        Egg(int id, Position pos, const std::string& team)
            : id(id), pos(pos), team(team) {}
    };

    struct Team {
        std::string name;
        std::vector<int> playerIds;
        std::vector<int> eggIds;
        int slotsAvailable = 0;

        Team(const std::string& name) : name(name) {}
    };

    struct Incantation {
        Position pos;
        int level;
        std::vector<int> playerIds;
        bool isActive = true;

        Incantation(Position pos, int level, const std::vector<int>& playerIds)
            : pos(pos), level(level), playerIds(playerIds) {}
    };

    /**
     * @brief Modèle complet de l'état du jeu Zappy
     *
     * Cette classe maintient une synchronisation en temps réel avec le serveur.
     * Elle est mise à jour par les messages du protocole reçus du serveur.
     */
    class GameState {
    public:
        GameState() = default;
        ~GameState() = default;

        // Map management
        void setMapSize(int width, int height);
        Position getMapSize() const { return mapSize_; }
        bool isValidPosition(const Position& pos) const;

        // Tile management
        void updateTile(const Position& pos, const Inventory& resources);
        const Tile* getTile(const Position& pos) const;
        Tile* getTile(const Position& pos);
        std::vector<const Tile*> getAllTiles() const;

        // Player management
        void addPlayer(int id, const Position& pos, Direction dir, int level, const std::string& team);
        void updatePlayerPosition(int id, const Position& pos, Direction dir);
        void updatePlayerLevel(int id, int level);
        void updatePlayerInventory(int id, const Position& pos, const Inventory& inventory);
        void removePlayer(int id);
        const Player* getPlayer(int id) const;
        Player* getPlayer(int id);
        std::vector<const Player*> getPlayersOnTile(const Position& pos) const;
        std::vector<const Player*> getAllPlayers() const;

        // Team management
        void addTeam(const std::string& name);
        void updateTeamSlots(const std::string& team, int slots);
        const Team* getTeam(const std::string& name) const;
        std::vector<std::string> getAllTeamNames() const;

        // Egg management
        void addEgg(int id, const Position& pos, const std::string& team);
        void removeEgg(int id);
        void setEggHatching(int id, bool hatching);
        const Egg* getEgg(int id) const;
        std::vector<const Egg*> getEggsOnTile(const Position& pos) const;

        // Incantation management
        void startIncantation(const Position& pos, int level, const std::vector<int>& playerIds);
        void endIncantation(const Position& pos, bool success);
        const Incantation* getIncantationAt(const Position& pos) const;
        std::vector<const Incantation*> getAllIncantations() const;

        // Time management
        void setTimeUnit(int timeUnit) { timeUnit_ = timeUnit; }
        int getTimeUnit() const { return timeUnit_; }

        // State queries
        bool isEmpty() const;
        void clear();

        // Debug
        void printState() const;

    private:
        Position mapSize_{0, 0};
        int timeUnit_ = 100;

        std::map<int, std::unique_ptr<Tile>> tilesById_;
        std::map<int, std::unique_ptr<Player>> players_;
        std::map<std::string, std::unique_ptr<Team>> teams_;
        std::map<int, std::unique_ptr<Egg>> eggs_;
        std::vector<std::unique_ptr<Incantation>> incantations_;

        // Helper functions
        std::string positionKey(const Position& pos) const;
        void updateTilePlayerList(const Position& pos);
        int getTileId(const Position& pos) const;
    };

} // namespace Zappy
