/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** Inventory
*/

#include "Inventory.hpp"

Inventory::Inventory() {
  items_["food"] = 0;
  items_["linemate"] = 0;
  items_["deraumere"] = 0;
  items_["sibur"] = 0;
  items_["mendiane"] = 0;
  items_["phiras"] = 0;
  items_["thystame"] = 0;
  items_["egg"] = 0;
}

Inventory::~Inventory() {}

void Inventory::addItem(const std::string &item, size_t quantity) {
  items_[item] += quantity;
}

void Inventory::removeItem(const std::string &item, size_t quantity) {
  if (items_.find(item) != items_.end() && items_[item] >= quantity) {
    items_[item] -= quantity;
  }
}

size_t Inventory::getItemQuantity(const std::string &item) const {
  if (items_.find(item) != items_.end()) {
    return items_.at(item);
  }
  return 0;
}
