/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** NetworkManagerImpl - Implémentation privée de NetworkManager
*/

#pragma once

#include "../../libzappy_net/include/zappy_net.h"
#include "../Core/GameState.hpp"
#include "../Core/WorldScene.hpp"
#include <string>
#include <deque>
#include <memory>

enum class ConnectionState;
namespace Zappy {
    class ProtocolParser;
}
/**
 * @brief Private implementation of NetworkManager
 * This class contains all details related to libzappy_net
 * and is never exposed in public headers.
 * It also manages the complete synchronization of the game state.
 */
class NetworkManagerImpl {
public:
    NetworkManagerImpl();
    ~NetworkManagerImpl();

    bool initializeLibrary();
    bool connect(const std::string& host, int port);
    void disconnect();
    bool performHandshake();

    bool sendMessage(const std::string& message);
    std::string receiveMessage();
    bool hasData() const;

    const Zappy::GameState& getGameState() const { return gameState_; }
    Zappy::GameState& getGameState() { return gameState_; }
    void updateFromServer();
    bool isGameStateSynchronized() const;

    ConnectionState getState() const;
    std::string getLastError() const;
    bool isConnected() const;
    void initializeParser(WorldScene& scene);

private:
    zn_socket_t socket_;
    ConnectionState state_;
    std::string lastError_;
    std::string host_;
    int port_;
    bool initialized_;
    std::deque<std::string> initialMessages_;

    Zappy::GameState gameState_;
    std::unique_ptr<Zappy::ProtocolParser> parser_;
    bool gameStateSynchronized_;

    void cleanup();
    void setError(const std::string& error);
    void processIncomingMessages();
    void initializeParser();
};
