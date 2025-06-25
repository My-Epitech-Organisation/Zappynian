/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** NetworkManager - Wrapper haut niveau pour libzappy_net (utilise PIMPL)
*/

#include "NetworkManager.hpp"
#include "NetworkManagerImpl.hpp"
#include <memory>

NetworkManager::NetworkManager() : impl_(std::make_unique<NetworkManagerImpl>()) {
}

NetworkManager::~NetworkManager() = default;

bool NetworkManager::connect(const std::string& host, int port) {
    return impl_->connect(host, port);
}

void NetworkManager::disconnect() {
    impl_->disconnect();
}

bool NetworkManager::performHandshake() {
    return impl_->performHandshake();
}

bool NetworkManager::sendMessage(const std::string& message) {
    return impl_->sendMessage(message);
}

std::string NetworkManager::receiveMessage() {
    return impl_->receiveMessage();
}

bool NetworkManager::hasData() const {
    return impl_->hasData();
}

ConnectionState NetworkManager::getState() const {
    return impl_->getState();
}

std::string NetworkManager::getLastError() const {
    return impl_->getLastError();
}

bool NetworkManager::isConnected() const {
    return impl_->isConnected();
}

const Zappy::GameState& NetworkManager::getGameState() const {
    return impl_->getGameState();
}

void NetworkManager::updateFromServer() {
    impl_->updateFromServer();
}

bool NetworkManager::isGameStateSynchronized() const {
    return impl_->isGameStateSynchronized();
}
