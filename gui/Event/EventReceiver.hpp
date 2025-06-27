/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** EventReceiver
*/

#pragma once
#include "../Entities/IEntity.hpp"
#include <irrlicht/irrlicht.h>
#include <memory>
#include <vector>

class TileEntity;

class EventReceiver : public irr::IEventReceiver {
public:
  /**
   * @brief Default constructor for EventReceiver
   */
  EventReceiver();
  
  /**
   * @brief Virtual destructor for EventReceiver
   */
  virtual ~EventReceiver();

  /**
   * @brief Handles all Irrlicht events (keyboard, mouse, GUI)
   * 
   * @param event The event to process
   * @return true if the event was handled, false otherwise
   */
  bool OnEvent(const irr::SEvent &event);

  /**
   * @brief Sets the Irrlicht device for this event receiver
   * 
   * @param dev Pointer to the Irrlicht device
   */
  void setDevice(irr::IrrlichtDevice *dev);

  /**
   * @brief Adds a tile entity cube to the receiver
   * 
   * @param c Pointer to the tile entity to add
   */
  void addCube(TileEntity *c);

  /**
   * @brief Adds an entity to the receiver's entity list
   * 
   * @param entity Shared pointer to the entity to add
   */
  void addEntity(std::shared_ptr<IEntity> entity);

  /**
   * @brief Removes an entity by its ID
   * 
   * @param id The ID of the entity to remove
   */
  void removeEntity(int id);

  /**
   * @brief Clears all entities from the receiver
   */
  void clearAllEntities();

  /**
   * @brief Sets the static text element for displaying information
   * 
   * @param t Pointer to the GUI static text element
   */
  void setText(irr::gui::IGUIStaticText *t);
  
  /**
   * @brief Sets the static text element for displaying player information
   * 
   * @param t Pointer to the GUI static text element for player info
   */
  void setPlayerText(irr::gui::IGUIStaticText *t) { textPlayer = t; }

  /**
   * @brief Sets the animated node for character movement
   * 
   * @param node Pointer to the animated mesh scene node
   */
  void setAnimatedNode(irr::scene::IAnimatedMeshSceneNode *node);

  // Getters
  /**
   * @brief Gets the current moving state
   * 
   * @return true if currently moving, false otherwise
   */
  bool getIsMoving() const { return isMoving; }
  
  /**
   * @brief Gets the movement start time
   * 
   * @return Movement start time in milliseconds
   */
  irr::u32 getMoveStartTime() const { return moveStartTime; }
  
  /**
   * @brief Gets the movement start X position
   * 
   * @return Starting X coordinate
   */
  float getMoveStartX() const { return moveStartX; }
  
  /**
   * @brief Gets the movement start Z position
   * 
   * @return Starting Z coordinate
   */
  float getMoveStartZ() const { return moveStartZ; }
  
  /**
   * @brief Gets the current Y rotation
   * 
   * @return Current Y rotation in degrees
   */
  float getCurrentRotationY() const { return currentRotationY; }
  
  /**
   * @brief Gets the current entity ID
   * 
   * @return Current entity identifier
   */
  int getCurrentEntityId() const { return currentEntityId; }

  // Setters
  /**
   * @brief Sets the moving state
   * 
   * @param value True to set as moving, false otherwise
   */
  void setIsMoving(bool value) { isMoving = value; }
  
  /**
   * @brief Sets the movement start time
   * 
   * @param value Movement start time in milliseconds
   */
  void setMoveStartTime(irr::u32 value) { moveStartTime = value; }
  
  /**
   * @brief Sets the movement start X position
   * 
   * @param value Starting X coordinate
   */
  void setMoveStartX(float value) { moveStartX = value; }
  
  /**
   * @brief Sets the movement start Z position
   * 
   * @param value Starting Z coordinate
   */
  void setMoveStartZ(float value) { moveStartZ = value; }
  
  /**
   * @brief Sets the current Y rotation
   * 
   * @param value Y rotation in degrees
   */
  void setCurrentRotationY(float value) { currentRotationY = value; }
  /**
   * @brief Sets the current entity ID
   * 
   * @param id Entity identifier to set
   */
  void setCurrentEntityId(int id) { currentEntityId = id; }

  /**
   * @brief Handles keyboard input events
   * 
   * @param keyInput Keyboard input event data
   * @return true if the input was handled, false otherwise
   */
  bool handleKeyInput(const irr::SEvent::SKeyInput &keyInput);
  
  /**
   * @brief Handles mouse input events
   * 
   * @param mouseInput Mouse input event data
   * @return true if the input was handled, false otherwise
   */
  bool handleMouseInput(const irr::SEvent::SMouseInput &mouseInput);
  
  /**
   * @brief Handles GUI events
   * 
   * @param guiEvent GUI event data
   * @return true if the event was handled, false otherwise
   */
  bool handleGUIEvent(const irr::SEvent::SGUIEvent &guiEvent);
  
  /**
   * @brief Handles camera movement based on mouse delta
   * 
   * @param deltaX Mouse movement delta in X direction
   * @param deltaY Mouse movement delta in Y direction
   * @return true if movement was handled, false otherwise
   */
  bool handleCameraMovement(int deltaX, int deltaY);
  
  /**
   * @brief Handles mouse wheel movement for camera zoom
   * 
   * @param wheel Mouse wheel movement value
   * @return true if wheel movement was handled, false otherwise
   */
  bool handleMouseWheelMovement(float wheel);
  
  /**
   * @brief Handles node selection via mouse input
   * 
   * @param mouseInput Mouse input event data
   * @return true if node selection was handled, false otherwise
   */
  bool handleNodeSelection(const irr::SEvent::SMouseInput &mouseInput);
  
  /**
   * @brief Handles character movement via keyboard input
   * 
   * @param key Key code for movement direction
   * @return true if character movement was handled, false otherwise
   */
  bool handleCharacterMovement(irr::EKEY_CODE key);
  
  /**
   * @brief Moves the camera based on key input
   * 
   * @param key Key code for camera movement
   * @param camera Pointer to the camera scene node
   * @return true if camera was moved, false otherwise
   */
  bool moveCamera(irr::EKEY_CODE key, irr::scene::ICameraSceneNode *camera);
  
  /**
   * @brief Handles player selection via mouse input
   * 
   * @param mouseInput Mouse input event data
   * @return true if player selection was handled, false otherwise
   */
  bool handlePlayerSelection(const irr::SEvent::SMouseInput &mouseInput);

protected:
  irr::IrrlichtDevice *device;
  irr::scene::ISceneManager *smgr;
  irr::video::IVideoDriver *driver;
  bool isMouseDown;
  int mouseX;
  int mouseY;
  irr::scene::IAnimatedMeshSceneNode *animatedNode;
  bool isMoving;
  irr::u32 moveStartTime;
  float moveStartX;
  float moveStartZ;
  float currentRotationY;
  int currentEntityId;

  std::vector<TileEntity *> cubes;
  irr::gui::IGUIStaticText *textCube;
  irr::gui::IGUIStaticText *textPlayer;
  std::vector<std::shared_ptr<IEntity>> entity_;
};
