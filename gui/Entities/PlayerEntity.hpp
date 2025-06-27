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

  ~PlayerEntity() override = default;

  Direction getDirection() const { return direction_; }
  std::string getTeam() const { return team_; }
  int getLevel() const { return level_; }
  void setLevel(int level) { level_ = level; }
  void setDirection(Direction direction) { direction_ = direction; }

private:
  Direction direction_;
  std::string team_;

  int level_;
};
