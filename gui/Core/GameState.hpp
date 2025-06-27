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
        /**
         * @brief Default constructor for GameState
         */
        GameState() = default;
        
        /**
         * @brief Default destructor for GameState
         */
        ~GameState() = default;

        /**
         * @brief Sets the map dimensions
         * 
         * @param width Map width
         * @param height Map height
         */
        void setMapSize(int width, int height);
        
        /**
         * @brief Gets the map size as a 3D vector
         * 
         * @return Map size vector (width, height, 0)
         */
        irr::core::vector3df getMapSize() const { return mapSize_; }
        
        /**
         * @brief Checks if a position is valid on the map
         * 
         * @param pos Position to validate
         * @return true if position is valid, false otherwise
         */
        bool isValidPosition(const irr::core::vector3df& pos) const;

        /**
         * @brief Updates a tile's resource content
         * 
         * @param pos Tile position
         * @param resources Resources to set on the tile
         */
        void updateTile(const irr::core::vector3df& pos, const Inventory& resources);
        
        /**
         * @brief Gets a tile at specified position (const version)
         * 
         * @param pos Tile position
         * @return Const pointer to the tile, nullptr if not found
         */
        const TileEntity* getTile(const irr::core::vector3df& pos) const;
        
        /**
         * @brief Gets a tile at specified position (mutable version)
         * 
         * @param pos Tile position
         * @return Pointer to the tile, nullptr if not found
         */
        TileEntity* getTile(const irr::core::vector3df& pos);
        
        /**
         * @brief Gets all tiles in the game
         * 
         * @return Vector of const pointers to all tiles
         */
        std::vector<const TileEntity*> getAllTiles() const;

        /**
         * @brief Adds a new player to the game
         * 
         * @param id Player identifier
         * @param pos Player position
         * @param dir Player direction
         * @param level Player level
         * @param team Player team name
         */
        void addPlayer(int id, const irr::core::vector3df& pos, Direction dir, int level, const std::string& team);
        
        /**
         * @brief Updates a player's position and direction
         * 
         * @param id Player identifier
         * @param pos New position
         * @param dir New direction
         */
        void updatePlayerPosition(int id, const irr::core::vector3df& pos, Direction dir);
        
        /**
         * @brief Updates a player's level
         * 
         * @param id Player identifier
         * @param level New level
         */
        void updatePlayerLevel(int id, int level);
        
        /**
         * @brief Updates a player's inventory
         * 
         * @param id Player identifier
         * @param pos Player position
         * @param inventory New inventory contents
         */
        void updatePlayerInventory(int id, const irr::core::vector3df& pos, const Inventory& inventory);
        
        /**
         * @brief Removes a player from the game
         * 
         * @param id Player identifier
         */
        void removePlayer(int id);
        
        /**
         * @brief Gets a player by ID (const version)
         * 
         * @param id Player identifier
         * @return Const pointer to the player, nullptr if not found
         */
        const PlayerEntity* getPlayer(int id) const;
        
        /**
         * @brief Gets a player by ID (mutable version)
         * 
         * @param id Player identifier
         * @return Pointer to the player, nullptr if not found
         */
        PlayerEntity* getPlayer(int id);
        
        /**
         * @brief Gets all players on a specific tile
         * 
         * @param pos Tile position
         * @return Vector of const pointers to players on the tile
         */
        std::vector<const PlayerEntity*> getPlayersOnTile(const irr::core::vector3df& pos) const;
        
        /**
         * @brief Gets all players in the game
         * 
         * @return Vector of const pointers to all players
         */
        std::vector<const PlayerEntity*> getAllPlayers() const;

        /**
         * @brief Sets the game time unit
         * 
         * @param timeUnit Time unit value
         */
        void setTimeUnit(int timeUnit) { timeUnit_ = timeUnit; }
        
        /**
         * @brief Gets the current time unit
         * 
         * @return Current time unit value
         */
        int getTimeUnit() const { return timeUnit_; }

        /**
         * @brief Checks if the game state is empty
         * 
         * @return true if no players or tiles exist, false otherwise
         */
        bool isEmpty() const;
        
        /**
         * @brief Clears all game state data
         */
        void clear();


        /**
         * @brief Adds a team to the game
         * 
         * @param name Team name to add
         */
        void addTeam(const std::string& name) {
            if (std::find(teams_.begin(), teams_.end(), name) == teams_.end()) {
                teams_.push_back(name);
            }
        }
        
        /**
         * @brief Gets all team names
         * 
         * @return Vector of team names
         */
        std::vector<std::string> getAllTeamNames() const { return teams_; }
        
        /**
         * @brief Adds an egg to the game
         * 
         * @param id Egg identifier
         * @param pos Egg position
         * @param team Team that owns the egg
         */
        void addEgg(int id, const irr::core::vector3df& pos, const std::string& team) {
            eggs_[id] = std::make_unique<Egg>(id, pos, irr::core::vector3df(1, 1, 1), std::vector<irr::io::path>(), irr::io::path());
            eggTeams_[id] = team;
        }
        
        /**
         * @brief Sets an egg's hatching state
         * 
         * @param id Egg identifier
         * @param hatching True if egg is hatching, false otherwise
         */
        void setEggHatching(int id, bool hatching) {
            auto it = eggs_.find(id);
            if (it != eggs_.end()) {
                it->second->setType(hatching ? "hatching" : "egg");
            }
        }
        
        /**
         * @brief Removes an egg from the game
         * 
         * @param id Egg identifier
         */
        void removeEgg(int id) {
            eggs_.erase(id);
            eggTeams_.erase(id);
        }
        
        /**
         * @brief Gets an egg by ID
         * 
         * @param id Egg identifier
         * @return Const pointer to the egg, nullptr if not found
         */
        const Egg* getEgg(int id) const {
            auto it = eggs_.find(id);
            return (it != eggs_.end()) ? it->second.get() : nullptr;
        }
        
        /**
         * @brief Gets all eggs in the game
         * 
         * @return Vector of const pointers to all eggs
         */
        std::vector<const Egg*> getAllEggs() const {
            std::vector<const Egg*> result;
            for (const auto& pair : eggs_) {
                result.push_back(pair.second.get());
            }
            return result;
        }
        
        /**
         * @brief Gets the team that owns an egg
         * 
         * @param id Egg identifier
         * @return Team name, empty string if not found
         */
        std::string getEggTeam(int id) const {
            auto it = eggTeams_.find(id);
            return (it != eggTeams_.end()) ? it->second : "";
        }
        
        /**
         * @brief Spawns a player from an egg
         * 
         * @param eggId Egg identifier to remove
         * @param playerId New player identifier
         * @param pos Player spawn position
         * @param dir Player initial direction
         * @param level Player initial level
         * @param team Player team
         */
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
        /**
         * @brief Starts an incantation at specified position
         * 
         * @param pos Incantation position
         * @param level Required level for incantation
         * @param playerIds Vector of participating player IDs
         */
        void startIncantation(const irr::core::vector3df& pos, int level, const std::vector<int>& playerIds) {
            incantations_.push_back({pos, level, playerIds, true});
        }
        
        /**
         * @brief Ends an incantation at specified position
         * 
         * @param pos Incantation position
         * @param success Whether the incantation was successful
         */
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
        
        /**
         * @brief Gets all incantations in the game
         * 
         * @return Vector of all incantations
         */
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

        /**
         * @brief Generates a unique key for a position
         * 
         * @param pos Position to generate key for
         * @return String key representing the position
         */
        std::string positionKey(const irr::core::vector3df& pos) const;
        
        /**
         * @brief Updates the list of players on a tile
         * 
         * @param pos Tile position to update
         */
        void updateTilePlayerList(const irr::core::vector3df& pos);
        
        /**
         * @brief Gets the tile ID for a given position
         * 
         * @param pos Position to get tile ID for
         * @return Tile identifier
         */
        int getTileId(const irr::core::vector3df& pos) const;
    };

} // namespace Zappy
