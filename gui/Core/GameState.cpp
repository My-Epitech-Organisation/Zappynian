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
    mapSize_ = irr::core::vector3df(width, height, 0);
    tilesById_.clear();
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int tileId = getTileId(irr::core::vector3df(x, y, 0));
            tilesById_[tileId] = std::make_unique<TileEntity>(tileId, irr::core::vector3df(x, y, 0),
            irr::core::vector3df(1, 1, 1), std::vector<irr::io::path>(), irr::io::path(), nullptr, "Tile");
        }
    }
}

bool GameState::isValidPosition(const irr::core::vector3df& pos) const {
    return pos.X >= 0 && pos.X < mapSize_.X && pos.Y >= 0 && pos.Y < mapSize_.Y;
}

void GameState::updateTile(const irr::core::vector3df& pos, const Inventory& resources) {
    if (!isValidPosition(pos)) {
        std::cerr << "ERROR: Invalid position (" << pos.X << "," << pos.Y << ")" << std::endl;
        return;
    }
    int tileId = getTileId(pos);
    auto it = tilesById_.find(tileId);
    if (it != tilesById_.end()) {
        it->second->getInventory() = resources;
    } else {
        std::cerr << "ERROR: Tile not found at (" << pos.X << "," << pos.Y << ")" << std::endl;
    }
}

const TileEntity* GameState::getTile(const irr::core::vector3df& pos) const {
    if (!isValidPosition(pos)) return nullptr;
    int tileId = getTileId(pos);
    auto it = tilesById_.find(tileId);
    return (it != tilesById_.end()) ? it->second.get() : nullptr;
}

TileEntity* GameState::getTile(const irr::core::vector3df& pos) {
    if (!isValidPosition(pos)) return nullptr;
    int tileId = getTileId(pos);
    auto it = tilesById_.find(tileId);
    return (it != tilesById_.end()) ? it->second.get() : nullptr;
}

std::vector<const TileEntity*> GameState::getAllTiles() const {
    std::vector<const TileEntity*> tiles;
    tiles.reserve(tilesById_.size());
    for (const auto& pair : tilesById_) {
        tiles.push_back(pair.second.get());
    }
    return tiles;
}

void GameState::addPlayer(int id, const irr::core::vector3df& pos, Direction dir, int level, const std::string& team) {
    if (!isValidPosition(pos)) {
        std::cerr << "ERROR: Cannot add player " << id << " at invalid position (" << pos.X << "," << pos.Y << ")" << std::endl;
        return;
    }
    players_[id] = std::make_unique<PlayerEntity>(id, pos, irr::core::vector3df(1, 1, 1), dir, team, std::vector<irr::io::path>(), level, irr::io::path());
    updateTilePlayerList(pos);
}

void GameState::updatePlayerPosition(int id, const irr::core::vector3df& pos, Direction dir) {
    auto it = players_.find(id);
    if (it == players_.end()) {
        std::cerr << "ERROR: Player " << id << " not found for position update" << std::endl;
        return;
    }
    if (!isValidPosition(pos)) {
        std::cerr << "ERROR: Invalid position (" << pos.X << "," << pos.Y << ") for player " << id << std::endl;
        return;
    }
    irr::core::vector3df oldPos = it->second->getPosition();
    it->second->setPosition(pos);
    it->second->setDirection(dir);
    updateTilePlayerList(oldPos);
    updateTilePlayerList(pos);
}

void GameState::updatePlayerLevel(int id, int level) {
    auto it = players_.find(id);
    if (it == players_.end()) {
        std::cerr << "ERROR: Player " << id << " not found for level update" << std::endl;
        return;
    }
    it->second->setLevel(level);
}

void GameState::updatePlayerInventory(int id, const irr::core::vector3df& pos, const Inventory& inventory) {
    auto it = players_.find(id);
    if (it == players_.end()) {
        std::cerr << "ERROR: Player " << id << " not found for inventory update" << std::endl;
        return;
    }
    it->second->getInventory() = inventory;
    if (isValidPosition(pos) && it->second->getPosition() != pos) {
        updatePlayerPosition(id, pos, it->second->getDirection());
    }
}

void GameState::removePlayer(int id) {
    auto it = players_.find(id);
    if (it == players_.end()) {
        std::cerr << "ERROR: Player " << id << " not found for removal" << std::endl;
        return;
    }
    irr::core::vector3df pos = it->second->getPosition();
    players_.erase(it);
    updateTilePlayerList(pos);
}

const PlayerEntity* GameState::getPlayer(int id) const {
    auto it = players_.find(id);
    return (it != players_.end()) ? it->second.get() : nullptr;
}

PlayerEntity* GameState::getPlayer(int id) {
    auto it = players_.find(id);
    return (it != players_.end()) ? it->second.get() : nullptr;
}

std::vector<const PlayerEntity*> GameState::getPlayersOnTile(const irr::core::vector3df& pos) const {
    std::vector<const PlayerEntity*> result;
    for (const auto& pair : players_) {
        if (pair.second->getPosition() == pos) {
            result.push_back(pair.second.get());
        }
    }
    return result;
}

std::vector<const PlayerEntity*> GameState::getAllPlayers() const {
    std::vector<const PlayerEntity*> result;
    for (const auto& pair : players_) {
        result.push_back(pair.second.get());
    }
    return result;
}

bool GameState::isEmpty() const {
    return mapSize_.X == 0 || mapSize_.Y == 0;
}

void GameState::clear() {
    mapSize_ = irr::core::vector3df(0, 0, 0);
    tilesById_.clear();
    players_.clear();
    timeUnit_ = 100;
}

std::string GameState::positionKey(const irr::core::vector3df& pos) const {
    return std::to_string(static_cast<int>(pos.X)) + "," + std::to_string(static_cast<int>(pos.Y));
}

void GameState::updateTilePlayerList(const irr::core::vector3df& pos) {
    TileEntity* tile = getTile(pos);
    if (!tile) return;
}

int GameState::getTileId(const irr::core::vector3df& pos) const {
    return static_cast<int>(pos.Y) * static_cast<int>(mapSize_.X) + static_cast<int>(pos.X);
}

} // namespace Zappy
