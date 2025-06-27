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
  /**
   * @brief Default constructor for Inventory
   */
  Inventory();
  
  /**
   * @brief Destructor for Inventory
   */
  ~Inventory();

  /**
   * @brief Adds items to the inventory
   * 
   * @param item Name of the item to add
   * @param quantity Quantity of items to add
   */
  void addItem(const std::string &item, size_t quantity);

  /**
   * @brief Removes items from the inventory
   * 
   * @param item Name of the item to remove
   * @param quantity Quantity of items to remove
   */
  void removeItem(const std::string &item, size_t quantity);

  /**
   * @brief Gets the quantity of a specific item
   * 
   * @param item Name of the item to check
   * @return Quantity of the specified item
   */
  size_t getItemQuantity(const std::string &item) const;

protected:
  std::map<std::string, size_t> items_;
};
