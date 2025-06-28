/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** AEntity
*/
#pragma once

#include "IEntity.hpp"
#include <vector>

class AEntity : public IEntity {
public:
  /**
   * @brief Default constructor for AEntity
   */
  AEntity()
      : id_(0), name_("unknown"), pos_(0, 0, 0), scale_(1.0f, 1.0f, 1.0f),
        node_(nullptr), level_(1) {}
        
  /**
   * @brief Virtual destructor for AEntity
   */
  virtual ~AEntity() = default;

  /**
   * @brief Sets the entity's unique identifier
   * 
   * @param id The ID to set
   */
  void setId(int id) override { id_ = id; }
  
  /**
   * @brief Gets the entity's unique identifier
   * 
   * @return The entity's ID
   */
  int getId() const override { return id_; }

  /**
   * @brief Sets the entity's name
   * 
   * @param name The name to set
   */
  void setName(const std::string &name) override { name_ = name; }
  
  /**
   * @brief Gets the entity's name
   * 
   * @return The entity's name
   */
  std::string getName() const override { return name_; }

  /**
   * @brief Sets the entity's 3D position
   * 
   * @param pos The position vector to set
   */
  void setPosition(const irr::core::vector3df &pos) override {
    pos_ = pos;
    if (node_) {
      node_->setPosition(pos_);
    }
  }
  
  /**
   * @brief Gets the entity's 3D position
   * 
   * @return The entity's position vector
   */
  irr::core::vector3df getPosition() const override { return pos_; }

  /**
   * @brief Sets the entity's 3D scale
   * 
   * @param scale The scale vector to set
   */
  void setScale(const irr::core::vector3df &scale) override {
    scale_ = scale;
    if (node_)
      node_->setScale(scale_);
  }

  /**
   * @brief Gets the entity's 3D scale
   * 
   * @return The entity's scale vector
   */
  irr::core::vector3df getScale() const override { return scale_; }

  /**
   * @brief Gets the entity's animated mesh scene node
   * 
   * @return Pointer to the animated mesh scene node
   */
  irr::scene::IAnimatedMeshSceneNode *getNode() const override { return node_; }

  /**
   * @brief Creates the 3D scene node for this entity
   * 
   * @param smgr Pointer to the scene manager
   * @param driver Pointer to the video driver
   */
  void createNode(irr::scene::ISceneManager *smgr,
                  irr::video::IVideoDriver *driver) override;

  /**
   * @brief Sets the entity's level
   * 
   * @param level The level to set
   */
  void setLevel(int level) override { level_ = level; }
  
  /**
   * @brief Gets the entity's level
   * 
   * @return The entity's level
   */
  int getLevel() const override { return level_; }


  void setTeam(const std::string &team) override { teams_ = team; }
  std::string getTeam() const override { return teams_; }
  /**
   * @brief Sets the entity's facing direction
   * 
   * @param direction The direction to set
   */
  void setDirection(Direction direction) override {
    if (node_) {
      node_->setRotation(
          irr::core::vector3df(0, static_cast<float>(direction) * 90.0f, 0));
    }
  }
  
  /**
   * @brief Gets the entity's facing direction
   * 
   * @return The entity's direction
   */
  Direction getDirection() const override {
    if (node_) {
      return static_cast<Direction>(
          static_cast<int>(node_->getRotation().Y / 90) % 4);
    }
    return Direction::NORTH;
  }

  /**
   * @brief Gets the entity's inventory (mutable version)
   * 
   * @return Reference to the entity's inventory
   */
  Inventory &getInventory() { return inventory_; }
  
  /**
   * @brief Gets the entity's inventory (const version)
   * 
   * @return Const reference to the entity's inventory
   */
  const Inventory &getInventory() const { return inventory_; }

protected:
  int id_;
  std::string name_;
  std::string teams_;
  irr::core::vector3df pos_;
  irr::core::vector3df scale_;
  irr::scene::IAnimatedMeshSceneNode *node_;
  std::vector<irr::io::path> textures_;
  irr::io::path entityB3D_;
  Inventory inventory_;
  int level_;

private:
};
