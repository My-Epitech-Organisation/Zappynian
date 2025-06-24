/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** AEgg
*/

#include "AEntity.hpp"
// #include "EntityManager.hpp"
#include "irrlicht/irrlicht.h"
#include <memory>
#include <string>
#include <vector>

#pragma once

class Egg : public AEntity {
public:
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
    team_ = getTeamFromPlayerId(playerId, entity_);
  }
  ~Egg() override = default;

  std::string getTeamFromPlayerId(int playerId, const std::vector<std::shared_ptr<IEntity>> &entity_) const {
    for (const auto& entity : entity_) {
      if (entity && entity->getId() == playerId) {
        return entity->getTeam();
      }
    }
    return "unknown";
  }
  void setType(const std::string &type) { type_ = type; }
  std::string getType() const { return type_; }

private:
  std::string type_;
  int playerId_;
  Direction direction_;
};
