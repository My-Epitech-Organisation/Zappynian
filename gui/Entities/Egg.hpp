/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** AEgg
*/

#include "AEntity.hpp"
#include "Event/EventReceiver.hpp"
#include "irrlicht/irrlicht.h"
#include <string>
#include <vector>

#pragma once

class Egg : public AEntity {
public:
  Egg(int id, const irr::core::vector3df &pos,
       const irr::core::vector3df &scale,
       const std::vector<irr::io::path> &textures,
       const irr::io::path &entityB3D)
      : AEntity(), type_("egg") {
      setId(id);
      setName("Egg");
      setPosition(pos);
      setScale(scale);
      textures_ = textures;
      entityB3D_ = entityB3D;
  }

  ~Egg() override = default;

  void setType(const std::string &type) { type_ = type; }
  std::string getType() const { return type_; }

private:
  std::string type_;
};
