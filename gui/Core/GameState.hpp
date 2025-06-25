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
#include <algorithm>
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


        void addTeam(const std::string& name) {
            if (std::find(teams_.begin(), teams_.end(), name) == teams_.end()) {
                teams_.push_back(name);
            }
        }
        std::vector<std::string> getAllTeamNames() const { return teams_; }
        void addEgg(int id, const irr::core::vector3df& pos, const std::string& team) {
            eggs_[id] = std::make_unique<Egg>(id, pos, irr::core::vector3df(1, 1, 1), std::vector<irr::io::path>(), irr::io::path());
            eggTeams_[id] = team;
        }
        void setEggHatching(int id, bool hatching) {
            auto it = eggs_.find(id);
            if (it != eggs_.end()) {
                it->second->setType(hatching ? "hatching" : "egg");
            }
        }
        void removeEgg(int id) {
            eggs_.erase(id);
            eggTeams_.erase(id);
        }
        const Egg* getEgg(int id) const {
            auto it = eggs_.find(id);
            return (it != eggs_.end()) ? it->second.get() : nullptr;
        }
        std::vector<const Egg*> getAllEggs() const {
            std::vector<const Egg*> result;
            for (const auto& pair : eggs_) {
                result.push_back(pair.second.get());
            }
            return result;
        }
        std::string getEggTeam(int id) const {
            auto it = eggTeams_.find(id);
            return (it != eggTeams_.end()) ? it->second : "";
        }
        void spawnFromEgg(int eggId, int playerId, const irr::core::vector3df& pos, Direction dir, int level, const std::string& team) {
            removeEgg(eggId);
            addPlayer(playerId, pos, dir, level, team);
        }
        struct Incantation {
            irr::core::vector3df pos;
            int level;
            std::vector<int> playerIds;
            bool active = true;
        };
        void startIncantation(const irr::core::vector3df& pos, int level, const std::vector<int>& playerIds) {
            incantations_.push_back({pos, level, playerIds, true});
        }
        void endIncantation(const irr::core::vector3df& pos, bool success) {
            for (auto& inc : incantations_) {
                if (inc.pos == pos && inc.active) {
                    inc.active = false;
                    if (success) {
                        for (int id : inc.playerIds) {
                            auto* p = getPlayer(id);
                            if (p) p->setLevel(inc.level + 1);
                        }
                    }
                }
            }
        }
        std::vector<Incantation> getAllIncantations() const {
            return incantations_;
        }

    private:
        irr::core::vector3df mapSize_{0, 0, 0};
        int timeUnit_ = 100;

        std::map<int, std::unique_ptr<TileEntity>> tilesById_;
        std::map<int, std::unique_ptr<PlayerEntity>> players_;
        std::map<int, std::unique_ptr<Egg>> eggs_;
        std::map<int, std::string> eggTeams_;

        std::vector<std::string> teams_;
        std::vector<Incantation> incantations_;

        std::string positionKey(const irr::core::vector3df& pos) const;
        void updateTilePlayerList(const irr::core::vector3df& pos);
        int getTileId(const irr::core::vector3df& pos) const;
    };

} // namespace Zappy
