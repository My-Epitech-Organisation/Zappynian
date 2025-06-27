/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** NetworkManagerImpl - Implémentation privée de NetworkManager
*/

#pragma once

#include "../../libzappy_net/include/zappy_net.h"
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
    /**
     * @brief Default constructor for NetworkManagerImpl
     */
    NetworkManagerImpl();
    
    /**
     * @brief Destructor for NetworkManagerImpl
     */
    ~NetworkManagerImpl();

    /**
     * @brief Initializes the libzappy_net library
     * 
     * @return true if initialization was successful, false otherwise
     */
    bool initializeLibrary();
    
    /**
     * @brief Connects to the specified server
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
     * @brief Performs the initial handshake with the server
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
     * @brief Initializes the protocol parser with a world scene
     * 
     * @param scene Reference to the world scene
     */
    void initializeParser(WorldScene& scene);

private:
    zn_socket_t socket_;
    ConnectionState state_;
    std::string lastError_;
    std::string host_;
    int port_;
    bool initialized_;
    std::deque<std::string> initialMessages_;

    std::unique_ptr<Zappy::ProtocolParser> parser_;
    bool gameStateSynchronized_;

    /**
     * @brief Cleans up resources and resets state
     */
    void cleanup();
    
    /**
     * @brief Sets the last error message
     * 
     * @param error Error message to set
     */
    void setError(const std::string& error);
    
    /**
     * @brief Processes incoming messages from the server
     */
    void processIncomingMessages();
};
