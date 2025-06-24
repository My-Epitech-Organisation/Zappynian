/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** NetworkManager - Wrapper haut niveau pour libzappy_net
*/

#pragma once

#include <string>
#include <memory>

// Forward declarations
class NetworkManagerImpl;
namespace Zappy {
    class GameState;
}

/**
 * @brief États de connexion possibles
 */
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
 * @brief Gestionnaire réseau principal utilisant libzappy_net
 *
 * Cette classe encapsule toutes les interactions avec libzappy_net
 * et fournit une interface thread-safe pour le GUI.
 */
class NetworkManager {
public:
    /**
     * @brief Constructeur
     */
    NetworkManager();

    /**
     * @brief Destructeur - nettoie automatiquement les ressources
     */
    ~NetworkManager();

    /**
     * @brief Se connecter au serveur Zappy
     * @param host Adresse IP ou nom d'hôte du serveur
     * @param port Port du serveur
     * @return true si la connexion est réussie, false sinon
     */
    bool connect(const std::string& host, int port);

    /**
     * @brief Fermer la connexion
     */
    void disconnect();

    /**
     * @brief Effectuer le handshake GUI avec le serveur
     * @return true si le handshake est réussi, false sinon
     */
    bool performHandshake();

    /**
     * @brief Envoyer un message au serveur
     * @param message Message à envoyer (sans \n)
     * @return true si envoyé avec succès, false sinon
     */
    bool sendMessage(const std::string& message);

    /**
     * @brief Recevoir un message du serveur (non-bloquant)
     * @return Message reçu (sans \n) ou chaîne vide si aucun message
     */
    std::string receiveMessage();

    /**
     * @brief Vérifier s'il y a des données à lire
     * @return true si des données sont disponibles
     */
    bool hasData() const;

    /**
     * @brief Obtenir l'état actuel de la connexion
     * @return État de connexion
     */
    ConnectionState getState() const;

    /**
     * @brief Obtenir le dernier message d'erreur
     * @return Description de la dernière erreur
     */
    std::string getLastError() const;

    /**
     * @brief Vérifier si la connexion est active
     * @return true si connecté et prêt
     */
    bool isConnected() const;

    // ========== Phase 3: Synchronisation de l'état du jeu ==========

    /**
     * @brief Accéder à l'état du jeu synchronisé (lecture seule)
     * @return Référence constante vers l'état du jeu
     */
    const Zappy::GameState& getGameState() const;

    /**
     * @brief Forcer la mise à jour depuis le serveur
     * Cette méthode traite tous les messages en attente et met à jour l'état du jeu
     */
    void updateFromServer();

    /**
     * @brief Vérifier si l'état initial du jeu est complètement synchronisé
     * @return true si l'état initial (map, teams, players) est reçu et traité
     */
    bool isGameStateSynchronized() const;

private:
    std::unique_ptr<NetworkManagerImpl> impl_;  ///< Implémentation cachée (PIMPL pattern)
};
