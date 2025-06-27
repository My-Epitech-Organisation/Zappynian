/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** PlayerEntity
*/
#include "AEntity.hpp"
#include "Event/EventReceiver.hpp"
#include "irrlicht/irrlicht.h"
#include <string>
#include <vector>

#pragma once

class Stone : public AEntity {
public:
  /**
   * @brief Constructs a Stone entity with specified parameters
   * 
   * @param id Unique identifier for the stone
   * @param pos 3D position of the stone
   * @param scale 3D scale of the stone
   * @param textures Vector of texture paths
   * @param entityB3D Path to the 3D model file
   * @param type Type of stone resource (default: "linemate")
   */
  Stone(int id, const irr::core::vector3df &pos,
        const irr::core::vector3df &scale,
        const std::vector<irr::io::path> &textures,
        const irr::io::path &entityB3D, std::string type = "linemate")
      : AEntity(), type_(type) {
    setName("Stone");
    setId(id);
    setPosition(pos);
    setScale(scale);
    textures_ = textures;
    entityB3D_ = entityB3D;
  }

  /**
   * @brief Virtual destructor for Stone
   */
  ~Stone() override = default;

  /**
   * @brief Sets the type of the stone
   * 
   * @param type Stone type string to set
   */
  void setType(const std::string &type) { type_ = type; }
  
  /**
   * @brief Gets the type of the stone
   * 
   * @return The stone's type string
   */
  std::string getType() const { return type_; }

private:
  std::string type_;
};
