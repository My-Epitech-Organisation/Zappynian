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

    // Ne rien faire ici, la version correcte prend WorldScene&
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

    socket_ = zn_client_connect(host.c_str(), port);

    if (socket_ == nullptr) {
        setError("Failed to connect to " + host + ":" + std::to_string(port));
        state_ = ConnectionState::ERROR;
        return false;
    }

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

    usleep(100000);

    int messagesAvailable = 0;
    std::string message;
    std::vector<std::string> initialMessages;

    for (int attempts = 0; attempts < 200; ++attempts) {
        message = receiveMessage();
        if (!message.empty()) {
            messagesAvailable++;
            initialMessages.push_back(message);
            usleep(10000);
        } else {
            usleep(50000);
            if (messagesAvailable > 10 && attempts > messagesAvailable + 20) {
                break;
            }
        }
    }
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
    int result = zn_send_message(socket_, message.c_str());
    if (result < 0) {
        setError("Failed to send message: " + message);
        return false;
    }
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
        return message;
    }
    char* raw_message = zn_receive_message(socket_);
    if (raw_message != nullptr) {
        std::string message(raw_message);
        free(raw_message);
        return message;
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

void NetworkManagerImpl::initializeParser(WorldScene& scene) {
    parser_ = std::make_unique<Zappy::ProtocolParser>(gameState_, scene);
}

void NetworkManagerImpl::updateFromServer() {
    if (!isConnected()) {
        return;
    }

    processIncomingMessages();
}

void NetworkManagerImpl::processIncomingMessages() {
    if (!parser_) {
        return;
    }

    while (!initialMessages_.empty()) {
        std::string message = initialMessages_.front();
        initialMessages_.pop_front();

        if (!message.empty()) {
            parser_->parseMessage(message);
        }
    }

    int maxAttempts = 50;
    int attempts = 0;

    while (hasData() && attempts < maxAttempts) {
        std::string message = receiveMessage();
        if (!message.empty()) {
            parser_->parseMessage(message);
        } else {
            break;
        }
        attempts++;
    }
    if (!gameStateSynchronized_ && !gameState_.isEmpty()) {
        auto mapSize = gameState_.getMapSize();
        if (mapSize.X > 0 && mapSize.Y > 0) {
            gameStateSynchronized_ = true;
        }
    }
}

bool NetworkManagerImpl::isGameStateSynchronized() const {
    return gameStateSynchronized_;
}
