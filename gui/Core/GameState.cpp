/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** GameState - Implémentation du modèle local
*/

#include "GameState.hpp"
#include <iostream>
#include <algorithm>

namespace Zappy {

    void GameState::setMapSize(int width, int height) {
        mapSize_ = Position(width, height);

        tilesById_.clear();
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                int tileId = getTileId(Position(x, y));
                tilesById_[tileId] = std::make_unique<Tile>(Position(x, y));
            }
        }
    }

    bool GameState::isValidPosition(const Position& pos) const {
        return pos.x >= 0 && pos.x < mapSize_.x && pos.y >= 0 && pos.y < mapSize_.y;
    }

    void GameState::updateTile(const Position& pos, const Inventory& resources) {
        if (!isValidPosition(pos)) {
            std::cerr << "ERROR: Invalid position (" << pos.x << "," << pos.y << ")" << std::endl;
            return;
        }

        int tileId = getTileId(pos);
        auto it = tilesById_.find(tileId);
        if (it != tilesById_.end()) {
            it->second->resources = resources;
        } else {
            std::cerr << "ERROR: Tile not found at (" << pos.x << "," << pos.y << ")" << std::endl;
        }
    }

    const Tile* GameState::getTile(const Position& pos) const {
        if (!isValidPosition(pos)) return nullptr;

        int tileId = getTileId(pos);
        auto it = tilesById_.find(tileId);
        return (it != tilesById_.end()) ? it->second.get() : nullptr;
    }

    Tile* GameState::getTile(const Position& pos) {
        if (!isValidPosition(pos)) return nullptr;

        int tileId = getTileId(pos);
        auto it = tilesById_.find(tileId);
        return (it != tilesById_.end()) ? it->second.get() : nullptr;
    }

    std::vector<const Tile*> GameState::getAllTiles() const {
        std::vector<const Tile*> tiles;
        tiles.reserve(tilesById_.size());
        for (const auto& pair : tilesById_) {
            tiles.push_back(pair.second.get());
        }
        return tiles;
    }

    void GameState::addPlayer(int id, const Position& pos, Direction dir, int level, const std::string& team) {
        if (!isValidPosition(pos)) {
            std::cerr << "ERROR: Cannot add player " << id << " at invalid position (" << pos.x << "," << pos.y << ")" << std::endl;
            return;
        }

        players_[id] = std::make_unique<Player>(id, pos, dir, level, team);

        updateTilePlayerList(pos);

        auto teamIt = teams_.find(team);
        if (teamIt != teams_.end()) {
            teamIt->second->playerIds.push_back(id);
        }
    }

    void GameState::updatePlayerPosition(int id, const Position& pos, Direction dir) {
        auto it = players_.find(id);
        if (it == players_.end()) {
            std::cerr << "ERROR: Player " << id << " not found for position update" << std::endl;
            return;
        }

        if (!isValidPosition(pos)) {
            std::cerr << "ERROR: Invalid position (" << pos.x << "," << pos.y << ") for player " << id << std::endl;
            return;
        }

        Position oldPos = it->second->pos;
        it->second->pos = pos;
        it->second->direction = dir;

        updateTilePlayerList(oldPos);
        updateTilePlayerList(pos);

    }

    void GameState::updatePlayerLevel(int id, int level) {
        auto it = players_.find(id);
        if (it == players_.end()) {
            std::cerr << "ERROR: Player " << id << " not found for level update" << std::endl;
            return;
        }
        it->second->level = level;
    }

    void GameState::updatePlayerInventory(int id, const Position& pos, const Inventory& inventory) {
        auto it = players_.find(id);
        if (it == players_.end()) {
            std::cerr << "ERROR: Player " << id << " not found for inventory update" << std::endl;
            return;
        }

        it->second->inventory = inventory;
        if (isValidPosition(pos) && !(it->second->pos == pos)) {
            updatePlayerPosition(id, pos, it->second->direction);
        }
    }

    void GameState::removePlayer(int id) {
        auto it = players_.find(id);
        if (it == players_.end()) {
            std::cerr << "ERROR: Player " << id << " not found for removal" << std::endl;
            return;
        }

        Position pos = it->second->pos;
        std::string team = it->second->team;

        auto teamIt = teams_.find(team);
        if (teamIt != teams_.end()) {
            auto& playerIds = teamIt->second->playerIds;
            playerIds.erase(std::remove(playerIds.begin(), playerIds.end(), id), playerIds.end());
        }
        players_.erase(it);
        updateTilePlayerList(pos);
    }

    const Player* GameState::getPlayer(int id) const {
        auto it = players_.find(id);
        return (it != players_.end()) ? it->second.get() : nullptr;
    }

    Player* GameState::getPlayer(int id) {
        auto it = players_.find(id);
        return (it != players_.end()) ? it->second.get() : nullptr;
    }

    std::vector<const Player*> GameState::getPlayersOnTile(const Position& pos) const {
        std::vector<const Player*> result;
        for (const auto& pair : players_) {
            if (pair.second->pos == pos && pair.second->isAlive) {
                result.push_back(pair.second.get());
            }
        }
        return result;
    }

    std::vector<const Player*> GameState::getAllPlayers() const {
        std::vector<const Player*> result;
        for (const auto& pair : players_) {
            if (pair.second->isAlive) {
                result.push_back(pair.second.get());
            }
        }
        return result;
    }

    void GameState::addTeam(const std::string& name) {
        if (teams_.find(name) == teams_.end()) {
            teams_[name] = std::make_unique<Team>(name);
        }
    }

    void GameState::updateTeamSlots(const std::string& team, int slots) {
        auto it = teams_.find(team);
        if (it != teams_.end()) {
            it->second->slotsAvailable = slots;
        }
    }

    const Team* GameState::getTeam(const std::string& name) const {
        auto it = teams_.find(name);
        return (it != teams_.end()) ? it->second.get() : nullptr;
    }

    std::vector<std::string> GameState::getAllTeamNames() const {
        std::vector<std::string> result;
        for (const auto& pair : teams_) {
            result.push_back(pair.first);
        }
        return result;
    }

    void GameState::addEgg(int id, const Position& pos, const std::string& team) {
        if (!isValidPosition(pos)) {
            std::cerr << "ERROR: Cannot add egg " << id << " at invalid position (" << pos.x << "," << pos.y << ")" << std::endl;
            return;
        }
        eggs_[id] = std::make_unique<Egg>(id, pos, team);
        Tile* tile = getTile(pos);
        if (tile) {
            tile->eggCount++;
        }
        auto teamIt = teams_.find(team);
        if (teamIt != teams_.end()) {
            teamIt->second->eggIds.push_back(id);
        }
    }

    void GameState::removeEgg(int id) {
        auto it = eggs_.find(id);
        if (it == eggs_.end()) {
            std::cerr << "ERROR: Egg " << id << " not found for removal" << std::endl;
            return;
        }
        Position pos = it->second->pos;
        std::string team = it->second->team;
        Tile* tile = getTile(pos);
        if (tile && tile->eggCount > 0) {
            tile->eggCount--;
        }
        auto teamIt = teams_.find(team);
        if (teamIt != teams_.end()) {
            auto& eggIds = teamIt->second->eggIds;
            eggIds.erase(std::remove(eggIds.begin(), eggIds.end(), id), eggIds.end());
        }
        eggs_.erase(it);
    }

    void GameState::setEggHatching(int id, bool hatching) {
        auto it = eggs_.find(id);
        if (it != eggs_.end()) {
            it->second->isHatching = hatching;
        }
    }

    const Egg* GameState::getEgg(int id) const {
        auto it = eggs_.find(id);
        return (it != eggs_.end()) ? it->second.get() : nullptr;
    }

    std::vector<const Egg*> GameState::getEggsOnTile(const Position& pos) const {
        std::vector<const Egg*> result;
        for (const auto& pair : eggs_) {
            if (pair.second->pos == pos) {
                result.push_back(pair.second.get());
            }
        }
        return result;
    }

    void GameState::startIncantation(const Position& pos, int level, const std::vector<int>& playerIds) {
        if (!isValidPosition(pos)) {
            std::cerr << "ERROR: Cannot start incantation at invalid position (" << pos.x << "," << pos.y << ")" << std::endl;
            return;
        }

        incantations_.push_back(std::make_unique<Incantation>(pos, level, playerIds));

        for (int playerId : playerIds) {
            Player* player = getPlayer(playerId);
            if (player) {
                player->isIncanting = true;
            }
        }
    }

    void GameState::endIncantation(const Position& pos, bool success) {
        auto it = std::find_if(incantations_.begin(), incantations_.end(),
                              [&pos](const std::unique_ptr<Incantation>& inc) {
                                  return inc->pos == pos && inc->isActive;
                              });

        if (it != incantations_.end()) {
            for (int playerId : (*it)->playerIds) {
                Player* player = getPlayer(playerId);
                if (player) {
                    player->isIncanting = false;
                    if (success) {
                        player->level++;
                    }
                }
            }
            (*it)->isActive = false;
            incantations_.erase(it);
        }
    }

    const Incantation* GameState::getIncantationAt(const Position& pos) const {
        auto it = std::find_if(incantations_.begin(), incantations_.end(),
                              [&pos](const std::unique_ptr<Incantation>& inc) {
                                  return inc->pos == pos && inc->isActive;
                              });
        return (it != incantations_.end()) ? it->get() : nullptr;
    }

    std::vector<const Incantation*> GameState::getAllIncantations() const {
        std::vector<const Incantation*> result;
        for (const auto& inc : incantations_) {
            if (inc->isActive) {
                result.push_back(inc.get());
            }
        }
        return result;
    }

    bool GameState::isEmpty() const {
        return mapSize_.x == 0 || mapSize_.y == 0;
    }

    void GameState::clear() {
        mapSize_ = Position(0, 0);
        tilesById_.clear();
        players_.clear();
        teams_.clear();
        eggs_.clear();
        incantations_.clear();
        timeUnit_ = 100;
    }

    void GameState::printState() const {
        std::cout << "=== GAME STATE ===" << std::endl;
        std::cout << "Map size: " << mapSize_.x << "x" << mapSize_.y << std::endl;
        std::cout << "Time unit: " << timeUnit_ << std::endl;
        std::cout << "Teams: " << teams_.size() << std::endl;
        for (const auto& pair : teams_) {
            std::cout << "  - " << pair.first << " (" << pair.second->playerIds.size()
                      << " players, " << pair.second->eggIds.size() << " eggs, "
                      << pair.second->slotsAvailable << " slots)" << std::endl;
        }
        std::cout << "Players: " << players_.size() << std::endl;
        std::cout << "Eggs: " << eggs_.size() << std::endl;
        std::cout << "Active incantations: " << getAllIncantations().size() << std::endl;
        std::cout << "==================" << std::endl;
    }

    std::string GameState::positionKey(const Position& pos) const {
        return std::to_string(pos.x) + "," + std::to_string(pos.y);
    }

    void GameState::updateTilePlayerList(const Position& pos) {
        Tile* tile = getTile(pos);
        if (!tile) return;

        tile->playerIds.clear();
        for (const auto& pair : players_) {
            if (pair.second->pos == pos && pair.second->isAlive) {
                tile->playerIds.push_back(pair.first);
            }
        }
    }

    int GameState::getTileId(const Position& pos) const {
        return pos.y * mapSize_.x + pos.x;
    }

} // namespace Zappy
