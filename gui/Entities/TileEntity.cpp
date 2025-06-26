/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** TileEntity
*/

#include "TileEntity.hpp"

void TileEntity::createTileNode(irr::scene::ISceneManager *smgr) {
  tileNode_ =
      smgr->addCubeSceneNode(20.0f, 0, -6, pos_, irr::core::vector3df(0, 0, 0),
                             irr::core::vector3df(1.0f, 0.5f, 1.0f));
  tileNode_->setMaterialFlag(irr::video::EMF_ZBUFFER, true);
  tileNode_->setMaterialFlag(irr::video::EMF_LIGHTING, false);
  tileNode_->setMaterialTexture(0, textureTile_);
  tileNode_->setName(name_.c_str());
  setName(name_.c_str());
  setId(-6);
}
