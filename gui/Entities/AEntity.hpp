/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** AEntity
*/
#include "IEntity.hpp"

#pragma once

class AEntity : public IEntity {
public:
  AEntity() : id_(0), name_("unknow"), pos_(0, 0, 0), scale_(1.0f, 1.0f, 1.0f), node_(nullptr) {}
  virtual ~AEntity() = default;

  void setId(int id) override { id_ = id; }
  int getId() const override { return id_; }

  void setName(const std::string &name) override { name_ = name; }
  std::string getName() const override { return name_; }

  void setPosition(const irr::core::vector3df &pos) override { pos_ = pos; }
  irr::core::vector3df getPosition() const override { return pos_; }

  void setScale(const irr::core::vector3df &scale) override { 
    scale_ = scale;
    if (node_)
      node_->setScale(scale_);
  }
  irr::core::vector3df getScale() const override { return scale_; }

  irr::scene::IAnimatedMeshSceneNode *getNode() const override { return node_; }
  void createNode(irr::scene::ISceneManager *smgr,
                  irr::video::IVideoDriver *driver,
                  EventReceiver &receiver) override;

protected:
  int id_;
  std::string name_;
  irr::core::vector3df pos_;
  irr::core::vector3df scale_;
  irr::scene::IAnimatedMeshSceneNode *node_;
  std::vector<irr::io::path> textures_;
  irr::io::path entityB3D_;

private:
};
