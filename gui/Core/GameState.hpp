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
#include "../Entities/Direction.hpp"
#include "../Entities/PlayerEntity.hpp"
#include "../Entities/TileEntity.hpp"
#include "../Entities/Egg.hpp"
#include "../Entities/Inventory.hpp"

namespace Zappy {

    class GameState {
    public:
        GameState() = default;
        ~GameState() = default;

        void setMapSize(int width, int height);
        irr::core::vector3df getMapSize() const { return mapSize_; }
        bool isValidPosition(const irr::core::vector3df& pos) const;

        void updateTile(const irr::core::vector3df& pos, const Inventory& resources);
        const TileEntity* getTile(const irr::core::vector3df& pos) const;
        TileEntity* getTile(const irr::core::vector3df& pos);
        std::vector<const TileEntity*> getAllTiles() const;

        void addPlayer(int id, const irr::core::vector3df& pos, Direction dir, int level, const std::string& team);
        void updatePlayerPosition(int id, const irr::core::vector3df& pos, Direction dir);
        void updatePlayerLevel(int id, int level);
        void updatePlayerInventory(int id, const irr::core::vector3df& pos, const Inventory& inventory);
        void removePlayer(int id);
        const PlayerEntity* getPlayer(int id) const;
        PlayerEntity* getPlayer(int id);
        std::vector<const PlayerEntity*> getPlayersOnTile(const irr::core::vector3df& pos) const;
        std::vector<const PlayerEntity*> getAllPlayers() const;

        void setTimeUnit(int timeUnit) { timeUnit_ = timeUnit; }
        int getTimeUnit() const { return timeUnit_; }

        bool isEmpty() const;
        void clear();


        void addTeam(const std::string& name) {}
        std::vector<std::string> getAllTeamNames() const { return {}; }
        void addEgg(int, const irr::core::vector3df&, const std::string&) {}
        void setEggHatching(int, bool) {}
        void removeEgg(int) {}
        void startIncantation(const irr::core::vector3df&, int, const std::vector<int>&) {}
        void endIncantation(const irr::core::vector3df&, bool) {}

    private:
        irr::core::vector3df mapSize_{0, 0, 0};
        int timeUnit_ = 100;

        std::map<int, std::unique_ptr<TileEntity>> tilesById_;
        std::map<int, std::unique_ptr<PlayerEntity>> players_;

        std::string positionKey(const irr::core::vector3df& pos) const;
        void updateTilePlayerList(const irr::core::vector3df& pos);
        int getTileId(const irr::core::vector3df& pos) const;
    };

} // namespace Zappy
