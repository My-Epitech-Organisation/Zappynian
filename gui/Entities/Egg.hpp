/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** AEgg
*/

#include "AEntity.hpp"
#include "irrlicht/irrlicht.h"
#include <string>
#include <vector>

#pragma once

class Egg : public AEntity {
public:
  /**
   * @brief Constructs an Egg entity with specified parameters
   * 
   * @param id Unique identifier for the egg
   * @param pos 3D position of the egg
   * @param scale 3D scale of the egg
   * @param textures Vector of texture paths
   * @param entityB3D Path to the 3D model file
   */
  Egg(int id, int playerId, const irr::core::vector3df &pos,
      const irr::core::vector3df &scale,
      const std::vector<irr::io::path> &textures,
      const irr::io::path &entityB3D,
      const std::vector<std::shared_ptr<IEntity>> &entity_,
      const Direction direction)
      : AEntity(), type_("egg"), playerId_(playerId),
        direction_(direction) {
    setId(id);
    setName("Egg");
    setPosition(pos);
    setScale(scale);
    textures_ = textures;
    entityB3D_ = entityB3D;
    teams_ = getTeamFromPlayerId(playerId, entity_);
  }

  /**
   * @brief Virtual destructor for Egg
   */
  ~Egg() override = default;

  std::string getTeamFromPlayerId(int playerId, const std::vector<std::shared_ptr<IEntity>> &entity_) const {
    for (const auto& entity : entity_) {
      if (entity && entity->getId() == playerId) {
        return entity->getTeam();
      }
    }
    return "unknown";
  }
  /**
   * @brief Sets the type of the egg
   * 
   * @param type Type string to set (e.g., "egg", "hatching")
   */
  void setType(const std::string &type) { type_ = type; }
  
  /**
   * @brief Gets the type of the egg
   * 
   * @return The egg's type string
   */
  std::string getType() const { return type_; }

private:
  std::string type_;
  int playerId_;
  Direction direction_;
  std::string teams_;
};
