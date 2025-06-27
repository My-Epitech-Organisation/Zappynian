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
  /**
   * @brief Constructs a TileEntity with specified parameters
   * 
   * @param id Unique identifier for the tile
   * @param pos 3D position of the tile
   * @param scale 3D scale of the tile
   * @param textures Vector of texture paths
   * @param entityB3D Path to the 3D model file
   * @param textureTile Texture for the tile surface
   * @param name Name of the tile
   */
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
  
  /**
   * @brief Virtual destructor for TileEntity
   */
  ~TileEntity() override = default;

  /**
   * @brief Creates the 3D scene node for this tile
   * 
   * @param smgr Pointer to the scene manager
   */
  void createTileNode(irr::scene::ISceneManager *smgr);

  /**
   * @brief Gets the tile's mesh scene node
   * 
   * @return Pointer to the tile's mesh scene node
   */
  irr::scene::IMeshSceneNode *getTileNode() const { return tileNode_; }

  /**
   * @brief Gets the tile's inventory (mutable version)
   * 
   * @return Reference to the tile's inventory
   */
  Inventory &getInventory() { return inventory_; }
  
  /**
   * @brief Gets the tile's inventory (const version)
   * 
   * @return Const reference to the tile's inventory
   */
  const Inventory &getInventory() const { return inventory_; }

protected:
  irr::video::ITexture *textureTile_;
  std::vector<irr::scene::IMeshSceneNode *> tileNodes_;
  irr::core::stringc name_;
  irr::scene::IMeshSceneNode *tileNode_;
  Inventory inventory_;

private:
};
