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

    /**
     * @brief Default constructor for NetworkManager
     */
    NetworkManager();

    /**
     * @brief Destructor for NetworkManager
     */
    ~NetworkManager();

    /**
     * @brief Connects to the specified Zappy server
     * 
     * @param host Server hostname or IP address
     * @param port Server port number
     * @return true if connection was successful, false otherwise
     */
    bool connect(const std::string& host, int port);

    /**
     * @brief Disconnects from the server
     */
    void disconnect();

    /**
     * @brief Performs the GUI handshake with the server
     * 
     * @return true if handshake was successful, false otherwise
     */
    bool performHandshake();

    /**
     * @brief Sends a message to the server
     * 
     * @param message Message string to send
     * @return true if message was sent successfully, false otherwise
     */
    bool sendMessage(const std::string& message);

    /**
     * @brief Receives a message from the server
     * 
     * @return Received message string
     */
    std::string receiveMessage();

    /**
     * @brief Checks if there is data available to read
     * 
     * @return true if data is available, false otherwise
     */
    bool hasData() const;

    /**
     * @brief Gets the current connection state
     * 
     * @return Current connection state
     */
    ConnectionState getState() const;

    /**
     * @brief Gets the last error message
     * 
     * @return Last error message string
     */
    std::string getLastError() const;

    /**
     * @brief Checks if currently connected to the server
     * 
     * @return true if connected, false otherwise
     */
    bool isConnected() const;

    /**
     * @brief Gets the current game state
     * 
     * @return Const reference to the game state
     */
    const Zappy::GameState& getGameState() const;

    /**
     * @brief Updates the game state from server messages
     */
    void updateFromServer();

    /**
     * @brief Checks if the game state is synchronized with the server
     * 
     * @return true if synchronized, false otherwise
     */
    bool isGameStateSynchronized() const;

    /**
     * @brief Initializes the protocol parser with a world scene
     * 
     * @param scene Reference to the world scene
     */
    void initializeParser(WorldScene& scene);

private:
    std::unique_ptr<NetworkManagerImpl> impl_;
};
