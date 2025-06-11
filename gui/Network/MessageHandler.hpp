/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** MessageDispatcher
*/

#pragma once
#include <unordered_map>
#include <memory>
#include "INetworkHandler.hpp"

class MessageHandler {
public:
    void registerHandler(const std::string& command, std::shared_ptr<INetworkHandler> handler);
    void handleCommand(const std::string& cmd, const std::vector<std::string>& args);

private:
    std::unordered_map<std::string, std::shared_ptr<INetworkHandler>> _handlers;
};

