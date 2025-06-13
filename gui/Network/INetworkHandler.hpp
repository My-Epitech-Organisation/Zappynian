/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** INetworkHandler
*/

#pragma once
#include <string>
#include <vector>

class INetworkHandler {
public:
  virtual ~INetworkHandler() = default;
  virtual void handleMessage(const std::string &cmd,
                             const std::vector<std::string> &args) = 0;
};
