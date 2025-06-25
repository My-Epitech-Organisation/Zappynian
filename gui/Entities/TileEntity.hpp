/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** TileEntity
*/
#pragma once
#include "AEntity.hpp"
#include "Inventory.hpp"
#include <string>
#include <vector>

class TileEntity : public AEntity {
public:
  TileEntity(int id, const irr::core::vector3df &pos,
             const irr::core::vector3df &scale,
             const std::vector<irr::io::path> &textures,
             const irr::io::path &entityB3D, irr::video::ITexture *textureTile,
             irr::core::stringc name)
      : AEntity(), textureTile_(textureTile), name_(name) {
    setId(id);
    setPosition(pos);
    setScale(scale);
    textures_ = textures;
    entityB3D_ = entityB3D;
  }
  ~TileEntity() override = default;

  void createTileNode(irr::scene::ISceneManager *smgr);

  irr::scene::IMeshSceneNode *getTileNode() const { return tileNode_; }

  Inventory &getInventory() { return inventory_; }
  const Inventory &getInventory() const { return inventory_; }

protected:
  irr::video::ITexture *textureTile_;
  std::vector<irr::scene::IMeshSceneNode *> tileNodes_;
  irr::core::stringc name_;
  irr::scene::IMeshSceneNode *tileNode_;
  Inventory inventory_;

private:
};
