/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** MessageDispatcher
*/

#pragma once
#include "INetworkHandler.hpp"
#include <memory>
#include <unordered_map>

class MessageHandler {
public:
  void registerHandler(const std::string &command,
                       std::shared_ptr<INetworkHandler> handler);
  void handleCommand(const std::string &cmd,
                     const std::vector<std::string> &args);

private:
  std::unordered_map<std::string, std::shared_ptr<INetworkHandler>> _handlers;
};
