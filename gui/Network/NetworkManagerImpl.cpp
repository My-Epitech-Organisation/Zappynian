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

    std::cout << "DEBUG: Attempting to connect to " << host << ":" << port << std::endl;
    state_ = ConnectionState::CONNECTING;
    host_ = host;
    port_ = port;

    socket_ = zn_client_connect(host.c_str(), port);

    if (socket_ == nullptr) {
        std::cout << "ERROR: Failed to connect to " << host << ":" << port << std::endl;
        setError("Failed to connect to " + host + ":" + std::to_string(port));
        state_ = ConnectionState::ERROR;
        return false;
    }

    std::cout << "DEBUG: Successfully connected to " << host << ":" << port << std::endl;
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
    std::cout << "DEBUG: Starting handshake..." << std::endl;
    if (!isConnected()) {
        setError("Not connected to server");
        return false;
    }

    state_ = ConnectionState::HANDSHAKE_IN_PROGRESS;

    std::string welcome;
    for (int attempts = 0; attempts < 10 && welcome.empty(); ++attempts) {
        welcome = receiveMessage();
        if (welcome.empty()) {
            usleep(100000);
        }
    }

    if (welcome.empty()) {
        setError("No welcome message received after 10 attempts");
        state_ = ConnectionState::ERROR;
        return false;
    }

    if (!isConnected()) {
        setError("Connection lost before sending GRAPHIC");
        state_ = ConnectionState::ERROR;
        return false;
    }

    if (!sendMessage("GRAPHIC")) {
        setError("Failed to send GRAPHIC command");
        state_ = ConnectionState::ERROR;
        return false;
    }

    std::cout << "DEBUG: Sent GRAPHIC command, waiting for initial messages..." << std::endl;
    usleep(100000); // 100ms au lieu de 2 secondes

    int messagesAvailable = 0;
    std::string message;
    std::vector<std::string> initialMessages;

    std::cout << "DEBUG: Starting message collection loop..." << std::endl;
    for (int attempts = 0; attempts < 200; ++attempts) {
        std::cout << "DEBUG: Attempt " << attempts << ", trying to receive message..." << std::endl;
        message = receiveMessage();
        if (!message.empty()) {
            messagesAvailable++;
            initialMessages.push_back(message);
            std::cout << "DEBUG: Received initial message " << messagesAvailable << ": '" << message << "'" << std::endl;
            usleep(10000); // 10ms
        } else {
            std::cout << "DEBUG: No message received on attempt " << attempts << std::endl;
            usleep(50000); // 50ms
            if (messagesAvailable > 10 && attempts > messagesAvailable + 20) {
                std::cout << "DEBUG: Breaking after " << attempts << " attempts with " << messagesAvailable << " messages" << std::endl;
                break;
            }
            // Sortir si on n'a rien reçu pendant longtemps
            if (attempts > 50 && messagesAvailable == 0) {
                std::cout << "DEBUG: No messages received after 50 attempts, breaking..." << std::endl;
                break;
            }
        }
    }

    std::cout << "DEBUG: Total initial messages received: " << messagesAvailable << std::endl;
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
        std::cout << "ERROR: Failed to send message: '" << message << "'" << std::endl;
        setError("Failed to send message: " + message);
        return false;
    }

    std::cout << "DEBUG: Message sent successfully: '" << message << "'" << std::endl;
    return true;
}

std::string NetworkManagerImpl::receiveMessage() {
    if (!isConnected()) {
        setError("Not connected");
        return "";
    }

    if (!initialMessages_.empty()) {
        std::string message = initialMessages_.front();
        initialMessages_.pop_front();
        std::cout << "DEBUG: Returning cached message: '" << message << "'" << std::endl;
        return message;
    }

    // Toujours essayer de lire, même sans nouvelles données socket
    // car les messages peuvent être dans le buffer interne
    std::cout << "DEBUG: Attempting to receive message with timeout..." << std::endl;
    char* raw_message = zn_receive_message(socket_);
    if (raw_message != nullptr) {
        std::string message(raw_message);
        free(raw_message);
        std::cout << "DEBUG: Received new message: '" << message << "'" << std::endl;
        return message;
    } else {
        std::cout << "DEBUG: zn_receive_message returned nullptr (no message available)" << std::endl;
    }

    return "";
}

bool NetworkManagerImpl::hasData() const {
    if (!isConnected()) {
        return false;
    }

    return true;
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

void NetworkManagerImpl::initializeParser() {
    std::cout << "DEBUG: Initializing protocol parser..." << std::endl;
    parser_ = std::make_unique<Zappy::ProtocolParser>(gameState_);

    parser_->setOnPlayerConnected([](const std::string& msg) {
        std::cout << "EVENT: Player connected - " << msg << std::endl;
    });

    parser_->setOnPlayerDisconnected([](const std::string& msg) {
        std::cout << "EVENT: Player disconnected - " << msg << std::endl;
    });

    parser_->setOnBroadcast([](const std::string& msg) {
        std::cout << "BROADCAST: " << msg << std::endl;
    });

    parser_->setOnGameEnd([](const std::string& msg) {
        std::cout << "GAME END: " << msg << std::endl;
    });

    std::cout << "DEBUG: Protocol parser initialized successfully" << std::endl;
}

void NetworkManagerImpl::updateFromServer() {
    if (!isConnected()) {
        return;
    }

    processIncomingMessages();
}

void NetworkManagerImpl::processIncomingMessages() {
    if (!parser_) {
        std::cerr << "ERROR: Parser not initialized" << std::endl;
        return;
    }

    std::cout << "DEBUG: Processing incoming messages. Cached messages: " << initialMessages_.size() << std::endl;

    while (!initialMessages_.empty()) {
        std::string message = initialMessages_.front();
        initialMessages_.pop_front();

        if (!message.empty()) {
            std::cout << "DEBUG: Processing cached message: '" << message << "'" << std::endl;
            parser_->parseMessage(message);
        }
    }

    int maxAttempts = 50;
    int attempts = 0;

    while (hasData() && attempts < maxAttempts) {
        std::string message = receiveMessage();
        if (!message.empty()) {
            std::cout << "DEBUG: Processing new message: '" << message << "'" << std::endl;
            parser_->parseMessage(message);
        } else {
            break;
        }
        attempts++;
    }

    // Vérifier l'état de la map
    if (!gameStateSynchronized_ && !gameState_.isEmpty()) {
        auto mapSize = gameState_.getMapSize();
        std::cout << "DEBUG: Checking map synchronization. Map size: " << mapSize.X << "x" << mapSize.Y << std::endl;
        if (mapSize.X > 0 && mapSize.Y > 0) {
            gameStateSynchronized_ = true;
            std::cout << "DEBUG: Game state synchronized! Map size confirmed: " << mapSize.X << "x" << mapSize.Y << std::endl;
        } else {
            std::cout << "DEBUG: Map size still invalid, not synchronized yet" << std::endl;
        }
    } else if (gameStateSynchronized_) {
        auto mapSize = gameState_.getMapSize();
        std::cout << "DEBUG: Game already synchronized. Current map size: " << mapSize.X << "x" << mapSize.Y << std::endl;
    } else {
        std::cout << "DEBUG: GameState is empty, waiting for map data..." << std::endl;
    }
}

bool NetworkManagerImpl::isGameStateSynchronized() const {
    return gameStateSynchronized_;
}
