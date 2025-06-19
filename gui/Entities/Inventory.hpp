/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** Inventory
*/
#include <map>
#include <string>

#pragma once

class Inventory {
public:
  Inventory();
  ~Inventory();

  void addItem(const std::string &item, size_t quantity);

  void removeItem(const std::string &item, size_t quantity);

  size_t getItemQuantity(const std::string &item) const;

protected:
  std::map<std::string, size_t> items_;
};
