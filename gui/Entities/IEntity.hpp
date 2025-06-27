/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** IEntity
*/

#pragma once
#include "Direction.hpp"
#include "Inventory.hpp"
#include <irrlicht/irrlicht.h>
#include <string>
#include <vector>

class EventReceiver;

class IEntity {
public:
  /**
   * @brief Virtual destructor for IEntity
   */
  virtual ~IEntity() = default;

  /**
   * @brief Creates the 3D scene node for this entity
   * 
   * @param smgr Pointer to the scene manager
   * @param driver Pointer to the video driver
   */
  virtual void createNode(irr::scene::ISceneManager *smgr,
                          irr::video::IVideoDriver *driver) = 0;

  /**
   * @brief Sets the entity's unique identifier
   * 
   * @param id The ID to set
   */
  virtual void setId(int id) = 0;
  
  /**
   * @brief Gets the entity's unique identifier
   * 
   * @return The entity's ID
   */
  virtual int getId() const = 0;

  /**
   * @brief Sets the entity's name
   * 
   * @param name The name to set
   */
  virtual void setName(const std::string &name) = 0;
  
  /**
   * @brief Gets the entity's name
   * 
   * @return The entity's name
   */
  virtual std::string getName() const = 0;

  /**
   * @brief Sets the entity's 3D position
   * 
   * @param pos The position vector to set
   */
  virtual void setPosition(const irr::core::vector3df &pos) = 0;
  
  /**
   * @brief Gets the entity's 3D position
   * 
   * @return The entity's position vector
   */
  virtual irr::core::vector3df getPosition() const = 0;

  /**
   * @brief Sets the entity's 3D scale
   * 
   * @param scale The scale vector to set
   */
  virtual void setScale(const irr::core::vector3df &scale) = 0;
  
  /**
   * @brief Gets the entity's 3D scale
   * 
   * @return The entity's scale vector
   */
  virtual irr::core::vector3df getScale() const = 0;

  /**
   * @brief Gets the entity's animated mesh scene node
   * 
   * @return Pointer to the animated mesh scene node
   */
  virtual irr::scene::IAnimatedMeshSceneNode *getNode() const = 0;

  /**
   * @brief Sets the entity's level
   * 
   * @param level The level to set
   */
  virtual void setLevel(int level) = 0;
  
  /**
   * @brief Gets the entity's level
   * 
   * @return The entity's level
   */
  virtual int getLevel() const = 0;

  /**
   * @brief Sets the entity's facing direction
   * 
   * @param direction The direction to set
   */
  virtual void setDirection(Direction direction) = 0;
  
  /**
   * @brief Gets the entity's facing direction
   * 
   * @return The entity's direction
   */
  virtual Direction getDirection() const = 0;

  /**
   * @brief Gets the entity's inventory
   * 
   * @return Reference to the entity's inventory
   */
  virtual Inventory &getInventory() = 0;

protected:
private:
};
