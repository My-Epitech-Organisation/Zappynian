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

enum class Direction { NORTH, SOUTH, EAST, WEST };

class PlayerEntity : public AEntity {
public:
  PlayerEntity(int id, const irr::core::vector3df &pos,
               const irr::core::vector3df &scale, Direction direction,
               const std::string &team,
               const std::vector<irr::io::path> &textures,
               const irr::io::path &entityB3D)
      : AEntity(), direction_(direction), team_(team) {
    setId(id);
    setPosition(pos);
    setScale(scale);
    textures_ = textures;
    entityB3D_ = entityB3D;
  }

  ~PlayerEntity() override = default;

  Direction getDirection() const { return direction_; }
  std::string getTeam() const { return team_; }

private:
  Direction direction_;
  std::string team_;

  int level_ = 1;
};
