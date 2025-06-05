/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** AEntity
*/

#include "AEntity.hpp"
#include <iostream>

void AEntity::createNode(irr::scene::ISceneManager *smgr,
                         irr::video::IVideoDriver *driver) {
  node_ = smgr->addAnimatedMeshSceneNode(smgr->getMesh(entityB3D_), 0, 1 | 2);
  node_->setScale(scale_);
  node_->setPosition(pos_);
  node_->setRotation(irr::core::vector3df(0, 0, 0));
  node_->setAnimationSpeed(0.0f);
  for (irr::u32 i = 0; i < node_->getMaterialCount(); ++i) {
    node_->getMaterial(i).setFlag(irr::video::EMF_LIGHTING, false);
    node_->getMaterial(i).setFlag(irr::video::EMF_ZBUFFER, true);
    node_->getMaterial(i).setFlag(irr::video::EMF_ZWRITE_ENABLE, true);
    node_->getMaterial(i).setFlag(irr::video::EMF_BACK_FACE_CULLING, false);
    node_->getMaterial(i).Lighting = true;
    node_->getMaterial(i).NormalizeNormals = true;
    node_->getMaterial(i).setTexture(0, driver->getTexture(textures_[i]));
  }
}
