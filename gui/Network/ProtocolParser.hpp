/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** ProtocolParser - Parser complet du protocole Zappy
*/

#pragma once

#include "../Core/GameState.hpp"
#include <string>
#include <vector>
#include <functional>

namespace Zappy {

    /**
     * @brief Parser pour tous les messages du protocole Zappy
     *
     * Cette classe parse les messages reçus du serveur et met à jour
     * l'état du jeu en conséquence.
     */
    class ProtocolParser {
    public:
        using MessageHandler = std::function<void(const std::string&)>;

        ProtocolParser(GameState& gameState);
        ~ProtocolParser() = default;

        /**
         * @brief Parse un message du serveur et met à jour l'état du jeu
         * @param message Le message à parser
         * @return true si le message a été reconnu et traité
         */
        bool parseMessage(const std::string& message);

        /**
         * @brief Parse plusieurs messages en lot
         */
        void parseMessages(const std::vector<std::string>& messages);

        // Callbacks pour les événements spéciaux (optionnel)
        void setOnPlayerConnected(MessageHandler handler) { onPlayerConnected_ = handler; }
        void setOnPlayerDisconnected(MessageHandler handler) { onPlayerDisconnected_ = handler; }
        void setOnBroadcast(MessageHandler handler) { onBroadcast_ = handler; }
        void setOnGameEnd(MessageHandler handler) { onGameEnd_ = handler; }

    private:
        GameState& gameState_;
        MessageHandler onPlayerConnected_;
        MessageHandler onPlayerDisconnected_;
        MessageHandler onBroadcast_;
        MessageHandler onGameEnd_;

        // Parsers pour chaque type de message
        bool parseMapSize(const std::vector<std::string>& args);           // msz X Y
        bool parseMapContent(const std::vector<std::string>& args);        // mct (multiple bct)
        bool parseTileContent(const std::vector<std::string>& args);       // bct X Y q0 q1 q2 q3 q4 q5 q6
        bool parseTeamName(const std::vector<std::string>& args);          // tna team_name
        bool parsePlayerNew(const std::vector<std::string>& args);         // pnw id X Y dir level team
        bool parsePlayerPosition(const std::vector<std::string>& args);    // ppo id X Y dir
        bool parsePlayerLevel(const std::vector<std::string>& args);       // plv id level
        bool parsePlayerInventory(const std::vector<std::string>& args);   // pin id X Y q0 q1 q2 q3 q4 q5 q6
        bool parsePlayerExpulsion(const std::vector<std::string>& args);   // pex id
        bool parsePlayerBroadcast(const std::vector<std::string>& args);   // pbc id message
        bool parsePlayerIncantStart(const std::vector<std::string>& args); // pic X Y level id1 id2 ...
        bool parsePlayerIncantEnd(const std::vector<std::string>& args);   // pie X Y result [level]
        bool parsePlayerFork(const std::vector<std::string>& args);        // pfk id
        bool parsePlayerResourceDrop(const std::vector<std::string>& args);// pdr id resource
        bool parsePlayerResourceTake(const std::vector<std::string>& args);// pgt id resource
        bool parsePlayerDeath(const std::vector<std::string>& args);       // pdi id
        bool parseEggLaid(const std::vector<std::string>& args);           // enw id id_parent X Y
        bool parseEggConnection(const std::vector<std::string>& args);     // eht id
        bool parseEggDeath(const std::vector<std::string>& args);          // edi id
        bool parseServerTime(const std::vector<std::string>& args);        // sgt time_unit
        bool parseServerTimeSet(const std::vector<std::string>& args);     // sst time_unit
        bool parseServerMessage(const std::vector<std::string>& args);     // smg message
        bool parseServerEnd(const std::vector<std::string>& args);         // seg team_name

        // Helper functions
        std::vector<std::string> tokenize(const std::string& message) const;
        Direction parseDirection(const std::string& dirStr) const;
        ResourceType parseResource(const std::string& resourceStr) const;
        Inventory parseInventoryArgs(const std::vector<std::string>& args, size_t startIndex) const;
        std::vector<int> parsePlayerIds(const std::vector<std::string>& args, size_t startIndex) const;
        bool isValidMessage(const std::string& message) const;

        // Debug
        void logParsedMessage(const std::string& command, const std::vector<std::string>& args) const;
    };

} // namespace Zappy
