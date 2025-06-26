/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** NetworkManager - Wrapper haut niveau pour libzappy_net
*/

#pragma once

#include <string>
#include <memory>
#include "../Core/WorldScene.hpp"

class NetworkManagerImpl;
namespace Zappy {
    class GameState;
}

enum class ConnectionState {
    DISCONNECTED,
    CONNECTING,
    CONNECTED,
    HANDSHAKE_IN_PROGRESS,
    READY,
    ERROR,
    RECONNECTING
};

/**
 * @class NetworkManager
 * @brief Manages the network connection and communication with the Zappy server for the GUI.
 *
 * This class provides methods to connect and disconnect from the Zappy server, perform the GUI handshake,
 * send and receive messages, and synchronize the game state. It uses the PIMPL pattern to hide implementation details.
 *
 * Main features:
 * - Establish and close connections to the server.
 * - Perform the initial handshake required by the Zappy protocol.
 * - Send and receive messages in a non-blocking manner.
 * - Monitor connection state and errors.
 * - Synchronize and access the current game state.
 *
 * @note All methods are designed to be thread-safe unless otherwise specified.
 */
class NetworkManager {
public:

    NetworkManager();

    ~NetworkManager();

    bool connect(const std::string& host, int port);

    void disconnect();

    bool performHandshake();

    bool sendMessage(const std::string& message);

    std::string receiveMessage();

    bool hasData() const;

    ConnectionState getState() const;

    std::string getLastError() const;

    bool isConnected() const;

    const Zappy::GameState& getGameState() const;

    void updateFromServer();

    bool isGameStateSynchronized() const;

    void initializeParser(WorldScene& scene);

private:
    std::unique_ptr<NetworkManagerImpl> impl_;
};
