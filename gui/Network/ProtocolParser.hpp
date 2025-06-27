/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** ProtocolParser - Parser complet du protocole Zappy
*/

#pragma once

#include "../Core/WorldScene.hpp"
#include <string>
#include <vector>
#include <functional>

namespace Zappy {

    /**
     * @brief Parser for all Zappy protocol messages.
     *
     * This class parses messages received from the server and updates
     * the game state accordingly.
     */
    class ProtocolParser {
    public:
        using MessageHandler = std::function<void(const std::string&)>;

        /**
         * @brief Constructs a ProtocolParser with a reference to the world scene
         * 
         * @param worldScene Reference to the world scene to update
         */
        ProtocolParser(WorldScene& worldScene);

        /**
         * @brief Default destructor
         */
        ~ProtocolParser() = default;

        /**
         * @brief Parse a server message and update the game state.
         * @param message The message to parse.
         * @return true if the message was recognized and handled.
         */
        bool parseMessage(const std::string& message);

        /**
         * @brief Parse multiple messages in batch.
         */
        void parseMessages(const std::vector<std::string>& messages);

        /**
         * @brief Sets the callback for player connection events
         * 
         * @param handler Function to call when a player connects
         */
        void setOnPlayerConnected(MessageHandler handler) { onPlayerConnected_ = handler; }
        
        /**
         * @brief Sets the callback for player disconnection events
         * 
         * @param handler Function to call when a player disconnects
         */
        void setOnPlayerDisconnected(MessageHandler handler) { onPlayerDisconnected_ = handler; }
        
        /**
         * @brief Sets the callback for broadcast events
         * 
         * @param handler Function to call when a broadcast is received
         */
        void setOnBroadcast(MessageHandler handler) { onBroadcast_ = handler; }
        
        /**
         * @brief Sets the callback for game end events
         * 
         * @param handler Function to call when the game ends
         */
        void setOnGameEnd(MessageHandler handler) { onGameEnd_ = handler; }

    private:
        WorldScene& worldScene_;
        MessageHandler onPlayerConnected_;
        MessageHandler onPlayerDisconnected_;
        MessageHandler onBroadcast_;
        MessageHandler onGameEnd_;

        /**
         * @brief Parses map size command (msz X Y)
         * 
         * @param args Tokenized command arguments
         * @return true if parsing was successful, false otherwise
         */
        bool parseMapSize(const std::vector<std::string>& args);           // msz X Y

        /**
         * @brief Parses tile content command (bct X Y q0 q1 q2 q3 q4 q5 q6)
         * 
         * @param args Tokenized command arguments
         * @return true if parsing was successful, false otherwise
         */
        bool parseTileContent(const std::vector<std::string>& args);       // bct X Y q0 q1 q2 q3 q4 q5 q6
        
        /**
         * @brief Parses team name command (tna team_name)
         * 
         * @param args Tokenized command arguments
         * @return true if parsing was successful, false otherwise
         */
        bool parseTeamName(const std::vector<std::string>& args);          // tna team_name
        
        /**
         * @brief Parses player new command (pnw id X Y dir level team)
         * 
         * @param args Tokenized command arguments
         * @return true if parsing was successful, false otherwise
         */
        bool parsePlayerNew(const std::vector<std::string>& args);         // pnw id X Y dir level team
        
        /**
         * @brief Parses player position command (ppo id X Y dir)
         * 
         * @param args Tokenized command arguments
         * @return true if parsing was successful, false otherwise
         */
        bool parsePlayerPosition(const std::vector<std::string>& args);    // ppo id X Y dir
        
        /**
         * @brief Parses player level command (plv id level)
         * 
         * @param args Tokenized command arguments
         * @return true if parsing was successful, false otherwise
         */
        bool parsePlayerLevel(const std::vector<std::string>& args);       // plv id level
        
        /**
         * @brief Parses player inventory command (pin id X Y q0 q1 q2 q3 q4 q5 q6)
         * 
         * @param args Tokenized command arguments
         * @return true if parsing was successful, false otherwise
         */
        bool parsePlayerInventory(const std::vector<std::string>& args);   // pin id X Y q0 q1 q2 q3 q4 q5 q6
        
        /**
         * @brief Parses player expulsion command (pex id)
         * 
         * @param args Tokenized command arguments
         * @return true if parsing was successful, false otherwise
         */
        bool parsePlayerExpulsion(const std::vector<std::string>& args);   // pex id
        
        /**
         * @brief Parses player broadcast command (pbc id message)
         * 
         * @param args Tokenized command arguments
         * @return true if parsing was successful, false otherwise
         */
        bool parsePlayerBroadcast(const std::vector<std::string>& args);   // pbc id message
        
        /**
         * @brief Parses player incantation start command (pic X Y level id1 id2 ...)
         * 
         * @param args Tokenized command arguments
         * @return true if parsing was successful, false otherwise
         */
        bool parsePlayerIncantStart(const std::vector<std::string>& args); // pic X Y level id1 id2 ...
        
        /**
         * @brief Parses player incantation end command (pie X Y result [level])
         * 
         * @param args Tokenized command arguments
         * @return true if parsing was successful, false otherwise
         */
        bool parsePlayerIncantEnd(const std::vector<std::string>& args);   // pie X Y result [level]
        
        /**
         * @brief Parses player fork command (pfk id)
         * 
         * @param args Tokenized command arguments
         * @return true if parsing was successful, false otherwise
         */
        bool parsePlayerFork(const std::vector<std::string>& args);        // pfk id
        
        /**
         * @brief Parses player resource drop command (pdr id resource)
         * 
         * @param args Tokenized command arguments
         * @return true if parsing was successful, false otherwise
         */
        bool parsePlayerResourceDrop(const std::vector<std::string>& args);// pdr id resource
        
        /**
         * @brief Parses player resource take command (pgt id resource)
         * 
         * @param args Tokenized command arguments
         * @return true if parsing was successful, false otherwise
         */
        bool parsePlayerResourceTake(const std::vector<std::string>& args);// pgt id resource
        
        /**
         * @brief Parses player death command (pdi id)
         * 
         * @param args Tokenized command arguments
         * @return true if parsing was successful, false otherwise
         */
        bool parsePlayerDeath(const std::vector<std::string>& args);       // pdi id
        
        /**
         * @brief Parses egg laid command (enw id id_parent X Y)
         * 
         * @param args Tokenized command arguments
         * @return true if parsing was successful, false otherwise
         */
        bool parseEggLaid(const std::vector<std::string>& args);           // enw id id_parent X Y
        
        /**
         * @brief Parses egg connection command (eht id)
         * 
         * @param args Tokenized command arguments
         * @return true if parsing was successful, false otherwise
         */
        bool parseEggConnection(const std::vector<std::string>& args);     // eht id
        
        /**
         * @brief Parses egg death command (edi id)
         * 
         * @param args Tokenized command arguments
         * @return true if parsing was successful, false otherwise
         */
        bool parseEggDeath(const std::vector<std::string>& args);          // edi id
        
        /**
         * @brief Parses server time command (sgt time_unit)
         * 
         * @param args Tokenized command arguments
         * @return true if parsing was successful, false otherwise
         */
        bool parseServerTime(const std::vector<std::string>& args);        // sgt time_unit
        
        /**
         * @brief Parses server time set command (sst time_unit)
         * 
         * @param args Tokenized command arguments
         * @return true if parsing was successful, false otherwise
         */
        bool parseServerTimeSet(const std::vector<std::string>& args);     // sst time_unit
        
        /**
         * @brief Parses server message command (smg message)
         * 
         * @param args Tokenized command arguments
         * @return true if parsing was successful, false otherwise
         */
        bool parseServerMessage(const std::vector<std::string>& args);     // smg message
        
        /**
         * @brief Parses server end command (seg team_name)
         * 
         * @param args Tokenized command arguments
         * @return true if parsing was successful, false otherwise
         */
        bool parseServerEnd(const std::vector<std::string>& args);         // seg team_name

        /**
         * @brief Tokenizes a message string into individual arguments
         * 
         * @param message Message string to tokenize
         * @return Vector of tokenized strings
         */
        std::vector<std::string> tokenize(const std::string& message) const;
        
        /**
         * @brief Parses a direction string into a Direction enum
         * 
         * @param dirStr Direction string to parse
         * @return Parsed Direction enum value
         */
        Direction parseDirection(const std::string& dirStr) const;
        
        /**
         * @brief Parses inventory arguments from a vector of strings
         * 
         * @param args Command arguments
         * @param startIndex Starting index for inventory data
         * @return Parsed Inventory object
         */
        Inventory parseInventoryArgs(const std::vector<std::string>& args, size_t startIndex) const;
        
        /**
         * @brief Parses player IDs from command arguments
         * 
         * @param args Command arguments
         * @param startIndex Starting index for player ID data
         * @return Vector of parsed player IDs
         */
        std::vector<int> parsePlayerIds(const std::vector<std::string>& args, size_t startIndex) const;
        
        /**
         * @brief Validates if a message is properly formatted
         * 
         * @param message Message string to validate
         * @return true if message is valid, false otherwise
         */
        bool isValidMessage(const std::string& message) const;
        
        /**
         * @brief Parses an ID string into an integer
         * 
         * @param idStr ID string to parse
         * @return Parsed integer ID
         */
        int parseId(const std::string& idStr) const;
    };

}
