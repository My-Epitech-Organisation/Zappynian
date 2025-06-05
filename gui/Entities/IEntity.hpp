/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** IEntity
*/
#include "Event/EventReceiver.hpp"
#include "irrlicht/irrlicht.h"
#include <string>
#include <vector>

#pragma once

class IEntity {
public:
  virtual ~IEntity() = default;

  virtual void createNode(irr::scene::ISceneManager *smgr,
                          irr::video::IVideoDriver *driver) = 0;

  virtual void setId(int id) = 0;
  virtual int getId() const = 0;

  virtual void setName(const std::string &name) = 0;
  virtual std::string getName() const = 0;

  virtual void setPosition(const irr::core::vector3df &pos) = 0;
  virtual irr::core::vector3df getPosition() const = 0;

  virtual void setScale(const irr::core::vector3df &scale) = 0;
  virtual irr::core::vector3df getScale() const = 0;

  virtual irr::scene::IAnimatedMeshSceneNode *getNode() const = 0;

protected:
private:
};
