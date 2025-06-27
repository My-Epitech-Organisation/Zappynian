/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** PlayerEntity
*/
#include "AEntity.hpp"
#include "Direction.hpp"
#include "Event/EventReceiver.hpp"
#include "irrlicht/irrlicht.h"
#include <string>
#include <vector>

#pragma once

class PlayerEntity : public AEntity {
public:
  /**
   * @brief Constructs a PlayerEntity with specified parameters
   * 
   * @param id Unique identifier for the player
   * @param pos 3D position of the player
   * @param scale 3D scale of the player
   * @param direction Initial facing direction
   * @param team Team name the player belongs to
   * @param textures Vector of texture paths
   * @param level Initial player level
   * @param entityB3D Path to the 3D model file
   */
  PlayerEntity(int id, const irr::core::vector3df &pos,
               const irr::core::vector3df &scale, Direction direction,
               const std::string &team,
               const std::vector<irr::io::path> &textures, int level,
               const irr::io::path &entityB3D)
      : AEntity(), direction_(direction), team_(team), level_(level) {
    setId(id);
    setPosition(pos);
    setScale(scale);
    setName("Player_" + std::to_string(id));
    textures_ = textures;
    entityB3D_ = entityB3D;
  }

  /**
   * @brief Virtual destructor for PlayerEntity
   */
  ~PlayerEntity() override = default;

  /**
   * @brief Gets the player's facing direction
   * 
   * @return The player's current direction
   */
  Direction getDirection() const { return direction_; }
  
  /**
   * @brief Gets the player's team name
   * 
   * @return The team name string
   */
  std::string getTeam() const { return team_; }
  
  /**
   * @brief Gets the player's current level
   * 
   * @return The player's level
   */
  int getLevel() const { return level_; }
  
  /**
   * @brief Sets the player's level
   * 
   * @param level The level to set
   */
  void setLevel(int level) { level_ = level; }
  
  /**
   * @brief Sets the player's facing direction
   * 
   * @param direction The direction to set
   */
  void setDirection(Direction direction) { direction_ = direction; }

private:
  Direction direction_;
  std::string team_;

  int level_;
};
