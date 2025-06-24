/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** NetworkManagerImpl - Implémentation privée de NetworkManager
*/

#include "NetworkManagerImpl.hpp"
#include "NetworkManager.hpp"
#include "ProtocolParser.hpp"
#include <iostream>
#include <unistd.h>
#include <sstream>
#include <deque>

NetworkManagerImpl::NetworkManagerImpl() 
    : socket_(nullptr), state_(ConnectionState::DISCONNECTED), 
      port_(0), initialized_(false), gameStateSynchronized_(false) {
    
    if (!initializeLibrary()) {
        setError("Failed to initialize libzappy_net");
    }
    
    initializeParser();
}

NetworkManagerImpl::~NetworkManagerImpl() {
    cleanup();
}

bool NetworkManagerImpl::initializeLibrary() {
    if (zn_init() < 0) {
        setError("zn_init() failed");
        return false;
    }
    initialized_ = true;
    return true;
}

bool NetworkManagerImpl::connect(const std::string& host, int port) {
    if (!initialized_) {
        setError("Library not initialized");
        return false;
    }

    if (state_ != ConnectionState::DISCONNECTED) {
        disconnect();
    }

    state_ = ConnectionState::CONNECTING;
    host_ = host;
    port_ = port;

    std::cout << "DEBUG: Calling zn_client_connect with host=" << host 
              << ", port=" << port << std::endl;

    socket_ = zn_client_connect(host.c_str(), port);
    
    std::cout << "DEBUG: zn_client_connect returned socket=" << socket_ << std::endl;
    
    if (socket_ == nullptr) {
        setError("Failed to connect to " + host + ":" + std::to_string(port));
        state_ = ConnectionState::ERROR;
        std::cout << "DEBUG: Connection failed - socket is NULL" << std::endl;
        return false;
    }

    std::cout << "DEBUG: Connection successful, socket ptr=" << socket_ << std::endl;
    state_ = ConnectionState::CONNECTED;
    return true;
}

void NetworkManagerImpl::disconnect() {
    if (socket_ != nullptr) {
        zn_close(socket_);
        socket_ = nullptr;
    }
    state_ = ConnectionState::DISCONNECTED;
    lastError_.clear();
}

bool NetworkManagerImpl::performHandshake() {
    if (!isConnected()) {
        setError("Not connected to server");
        return false;
    }

    state_ = ConnectionState::HANDSHAKE_IN_PROGRESS;

    // Étape 1: Recevoir le message de bienvenue
    std::cout << "Waiting for welcome message..." << std::endl;
    
    std::string welcome;
    for (int attempts = 0; attempts < 10 && welcome.empty(); ++attempts) {
        welcome = receiveMessage();
        if (welcome.empty()) {
            std::cout << "Attempt " << (attempts + 1) << ": No message yet, waiting..." << std::endl;
            usleep(100000); // Attendre 100ms
        }
    }
    
    if (welcome.empty()) {
        setError("No welcome message received after 10 attempts");
        state_ = ConnectionState::ERROR;
        return false;
    }
    
    std::cout << "Received welcome: " << welcome << std::endl;

    // Étape 2: Envoyer "GRAPHIC" pour s'identifier comme GUI
    std::cout << "Sending GRAPHIC command..." << std::endl;
    
    // Vérifier que la connexion est toujours active
    if (!isConnected()) {
        setError("Connection lost before sending GRAPHIC");
        state_ = ConnectionState::ERROR;
        return false;
    }
    
    std::cout << "DEBUG: Connection still active, socket ptr = " << socket_ << std::endl;
    
    if (!sendMessage("GRAPHIC")) {
        setError("Failed to send GRAPHIC command");
        state_ = ConnectionState::ERROR;
        return false;
    }

    // Étape 3: Attendre que le serveur envoie les données initiales
    std::cout << "Waiting for initial server state after GRAPHIC..." << std::endl;
    
    // Attendre plus longtemps pour laisser le temps au serveur de répondre
    std::cout << "DEBUG: Waiting 2 seconds for server to prepare response..." << std::endl;
    sleep(2); // Attendre 2 secondes
    
    // Compter combien de messages sont maintenant disponibles
    int messagesAvailable = 0;
    std::string message;
    std::vector<std::string> initialMessages;
    
    std::cout << "DEBUG: Starting to read messages..." << std::endl;
    
    // Lire tous les messages disponibles maintenant avec plus de tentatives
    for (int attempts = 0; attempts < 200; ++attempts) {
        message = receiveMessage();
        if (!message.empty()) {
            messagesAvailable++;
            initialMessages.push_back(message);
            std::cout << "DEBUG: Initial message " << messagesAvailable << ": '" << message << "'" << std::endl;
            
            // Si on a reçu des messages, attendre un peu moins pour le suivant
            usleep(10000); // 10ms entre chaque message
        } else {
            // Pas de message, attendre un peu plus
            usleep(50000); // 50ms
            
            // Si on a déjà reçu beaucoup de messages et qu'il n'y en a plus depuis un moment, arrêter
            if (messagesAvailable > 10 && attempts > messagesAvailable + 20) {
                std::cout << "DEBUG: No more messages after " << attempts << " attempts, stopping" << std::endl;
                break;
            }
        }
        
        // Afficher un indicateur de progression
        if (attempts % 50 == 0 && attempts > 0) {
            std::cout << "DEBUG: Attempt " << attempts << ", messages so far: " << messagesAvailable << std::endl;
        }
    }
    
    std::cout << "DEBUG: Handshake phase - received " << messagesAvailable << " initial messages total" << std::endl;
    
    // Stocker les messages pour qu'ils puissent être re-lus plus tard
    for (const auto& msg : initialMessages) {
        initialMessages_.push_back(msg);
    }
    state_ = ConnectionState::READY;
    return true;
}

bool NetworkManagerImpl::sendMessage(const std::string& message) {
    if (!isConnected()) {
        setError("Not connected");
        return false;
    }

    std::cout << "DEBUG: Sending message: '" << message << "'" << std::endl;
    
    int result = zn_send_message(socket_, message.c_str());
    if (result < 0) {
        setError("Failed to send message: " + message);
        std::cout << "DEBUG: zn_send_message returned error: " << result << std::endl;
        return false;
    }

    std::cout << "DEBUG: Message sent successfully" << std::endl;
    return true;
}

std::string NetworkManagerImpl::receiveMessage() {
    if (!isConnected()) {
        setError("Not connected");
        return "";
    }

    // D'abord vérifier le cache des messages initiaux
    if (!initialMessages_.empty()) {
        std::string message = initialMessages_.front();
        initialMessages_.pop_front();
        std::cout << "DEBUG: Returning cached message: '" << message << "'" << std::endl;
        return message;
    }

    // Vérifier si des données sont disponibles avec zn_poll (avec timeout)
    zn_socket_t sockets[1] = { socket_ };
    short events[1] = { 1 }; // POLLIN equivalent
    
    zn_poll_result_t poll_result = zn_poll(sockets, events, 1, 100); // timeout 100ms
    
    if (!(poll_result.readable & 1)) {
        // Pas de données disponibles en lecture
        return "";
    }

    std::cout << "DEBUG: Data available, poll result - readable: " << poll_result.readable
              << ", writable: " << poll_result.writable << ", error: " << poll_result.error << std::endl;

    // Si error bit est set, la connexion a un problème
    if (poll_result.error & 1) {
        std::cout << "DEBUG: Poll indicates error on socket!" << std::endl;
        setError("Socket error detected");
        return "";
    }

    // Essayer de lire un message via zn_receive_message
    char* raw_message = zn_receive_message(socket_);
    if (raw_message != nullptr) {
        std::string message(raw_message);
        free(raw_message);
        std::cout << "DEBUG: Received message via zn_receive_message: '" << message << "'" << std::endl;
        return message;
    }

    std::cout << "DEBUG: zn_receive_message returned NULL, trying raw zn_read..." << std::endl;

    // Fallback: lecture brute et parsing manuel pour les messages multiples
    char buffer[4096];
    ssize_t bytes_read = zn_read(socket_, buffer, sizeof(buffer) - 1);
    
    if (bytes_read > 0) {
        buffer[bytes_read] = '\0';
        std::string data(buffer);
        std::cout << "DEBUG: Raw read returned " << bytes_read << std::endl;
        std::cout << "DEBUG: Raw read got " << bytes_read << " bytes: '" << data.substr(0, 500) << "'" << std::endl;
        
        // Parser les lignes et les mettre en cache
        std::istringstream iss(data);
        std::string line;
        std::vector<std::string> lines;
        
        while (std::getline(iss, line)) {
            if (!line.empty()) {
                lines.push_back(line);
            }
        }
        
        if (!lines.empty()) {
            // Retourner la première ligne et mettre le reste en cache
            std::string firstLine = lines[0];
            for (size_t i = 1; i < lines.size(); i++) {
                initialMessages_.push_back(lines[i]);
            }
            std::cout << "DEBUG: Parsed " << lines.size() << " lines, returning: '" << firstLine << "'" << std::endl;
            return firstLine;
        }
    } else if (bytes_read == 0) {
        std::cout << "DEBUG: Connection closed by remote (EOF)" << std::endl;
        setError("Connection closed by server");
    } else {
        std::cout << "DEBUG: Read error: " << errno << std::endl;
    }

    return "";
}

bool NetworkManagerImpl::hasData() const {
    if (!isConnected()) {
        return false;
    }

    // Pour l'instant, on assume qu'il peut y avoir des données
    // Une vraie implémentation nécessiterait une fonction dédiée dans libzappy_net
    return true;  // Simplifié pour la phase 1
}

ConnectionState NetworkManagerImpl::getState() const {
    return state_;
}

std::string NetworkManagerImpl::getLastError() const {
    return lastError_;
}

bool NetworkManagerImpl::isConnected() const {
    return socket_ != nullptr && 
           (state_ == ConnectionState::CONNECTED || 
            state_ == ConnectionState::HANDSHAKE_IN_PROGRESS ||
            state_ == ConnectionState::READY);
}

void NetworkManagerImpl::cleanup() {
    if (socket_ != nullptr) {
        zn_close(socket_);
        socket_ = nullptr;
    }
    
    if (initialized_) {
        zn_cleanup();
        initialized_ = false;
    }
    
    state_ = ConnectionState::DISCONNECTED;
}

void NetworkManagerImpl::setError(const std::string& error) {
    lastError_ = error;
    state_ = ConnectionState::ERROR;
    std::cerr << "NetworkManager Error: " << error << std::endl;
}

// Phase 3: Nouvelles méthodes pour la synchronisation
void NetworkManagerImpl::initializeParser() {
    parser_ = std::make_unique<Zappy::ProtocolParser>(gameState_);
    
    // Configurer les callbacks du parser
    parser_->setOnPlayerConnected([](const std::string& msg) {
        std::cout << "EVENT: " << msg << std::endl;
    });
    
    parser_->setOnPlayerDisconnected([](const std::string& msg) {
        std::cout << "EVENT: " << msg << std::endl;
    });
    
    parser_->setOnBroadcast([](const std::string& msg) {
        std::cout << "BROADCAST: " << msg << std::endl;
    });
    
    parser_->setOnGameEnd([](const std::string& msg) {
        std::cout << "GAME END: " << msg << std::endl;
    });
}

void NetworkManagerImpl::updateFromServer() {
    if (!isConnected()) {
        std::cout << "DEBUG: Not connected, cannot update from server" << std::endl;
        return;
    }
    
    processIncomingMessages();
}

void NetworkManagerImpl::processIncomingMessages() {
    if (!parser_) {
        std::cerr << "ERROR: Parser not initialized" << std::endl;
        return;
    }

    // Traiter d'abord les messages mis en cache
    while (!initialMessages_.empty()) {
        std::string message = initialMessages_.front();
        initialMessages_.pop_front();
        
        if (!message.empty()) {
            std::cout << "DEBUG: Processing cached message: '" << message << "'" << std::endl;
            parser_->parseMessage(message);
        }
    }

    // Puis traiter les nouveaux messages
    int maxAttempts = 50; // Éviter une boucle infinie
    int attempts = 0;
    
    while (hasData() && attempts < maxAttempts) {
        std::string message = receiveMessage();
        if (!message.empty()) {
            std::cout << "DEBUG: Processing new message: '" << message << "'" << std::endl;
            parser_->parseMessage(message);
        } else {
            break; // Plus de messages
        }
        attempts++;
    }
    
    // Vérifier si la synchronisation initiale est complète
    if (!gameStateSynchronized_ && !gameState_.isEmpty()) {
        // Considérer comme synchronisé si on a au moins la taille de la map
        if (gameState_.getMapSize().x > 0 && gameState_.getMapSize().y > 0) {
            gameStateSynchronized_ = true;
            std::cout << "DEBUG: Initial game state synchronization complete!" << std::endl;
            gameState_.printState();
        }
    }
}

bool NetworkManagerImpl::isGameStateSynchronized() const {
    return gameStateSynchronized_;
}
