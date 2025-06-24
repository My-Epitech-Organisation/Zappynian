/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** NetworkManagerImpl - Implémentation privée de NetworkManager
*/

#pragma once

#include "../../libzappy_net/include/zappy_net.h"
#include "../Core/GameState.hpp"
#include <string>
#include <deque>
#include <memory>

// Forward declarations
enum class ConnectionState;
namespace Zappy {
    class ProtocolParser;
}

/**
 * @brief Implémentation privée du NetworkManager
 * Cette classe contient tous les détails de libzappy_net
 * et n'est jamais exposée dans les headers publics.
 * Elle gère également la synchronisation complète de l'état du jeu.
 */
class NetworkManagerImpl {
public:
    NetworkManagerImpl();
    ~NetworkManagerImpl();

    // Méthodes de gestion de la connexion
    bool initializeLibrary();
    bool connect(const std::string& host, int port);
    void disconnect();
    bool performHandshake();

    // Méthodes de communication
    bool sendMessage(const std::string& message);
    std::string receiveMessage();
    bool hasData() const;

    const Zappy::GameState& getGameState() const { return gameState_; }
    Zappy::GameState& getGameState() { return gameState_; }
    void updateFromServer(); // Force la mise à jour depuis le serveur
    bool isGameStateSynchronized() const; // Vérifie si l'état initial est complet

    // Getters pour l'état
    ConnectionState getState() const;
    std::string getLastError() const;
    bool isConnected() const;

private:
    zn_socket_t socket_;            ///< Socket libzappy_net
    ConnectionState state_;         ///< État actuel
    std::string lastError_;         ///< Dernière erreur
    std::string host_;              ///< Host de connexion
    int port_;                      ///< Port de connexion
    bool initialized_;              ///< Flag d'initialisation
    std::deque<std::string> initialMessages_;  ///< Messages initiaux stockés

    Zappy::GameState gameState_;               ///< État du jeu synchronisé
    std::unique_ptr<Zappy::ProtocolParser> parser_;  ///< Parser de protocole
    bool gameStateSynchronized_;               ///< Flag de synchronisation complète

    // Méthodes privées
    void cleanup();
    void setError(const std::string& error);
    void processIncomingMessages();            // Traite tous les messages reçus
    void initializeParser();                   // Initialise le parser
};
