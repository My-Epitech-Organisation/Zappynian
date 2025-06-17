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
  EventReceiver();
  virtual ~EventReceiver();

  bool OnEvent(const irr::SEvent &event);

  void setDevice(irr::IrrlichtDevice *dev);

  void addCube(TileEntity *c);

  void addEntity(std::shared_ptr<IEntity> entity);

  void removeEntity(int id);

  void clearAllEntities();

  void setText(irr::gui::IGUIStaticText *t);
  void setPlayerText(irr::gui::IGUIStaticText *t) { textPlayer = t; }

  void setAnimatedNode(irr::scene::IAnimatedMeshSceneNode *node);

  // Getters
  bool getIsMoving() const { return isMoving; }
  irr::u32 getMoveStartTime() const { return moveStartTime; }
  float getMoveStartX() const { return moveStartX; }
  float getMoveStartZ() const { return moveStartZ; }
  float getCurrentRotationY() const { return currentRotationY; }
  int getCurrentEntityId() const { return currentEntityId; }

  // Setters
  void setIsMoving(bool value) { isMoving = value; }
  void setMoveStartTime(irr::u32 value) { moveStartTime = value; }
  void setMoveStartX(float value) { moveStartX = value; }
  void setMoveStartZ(float value) { moveStartZ = value; }
  void setCurrentRotationY(float value) { currentRotationY = value; }
  void setCurrentEntityId(int id) { currentEntityId = id; }

  bool handleKeyInput(const irr::SEvent::SKeyInput &keyInput);
  bool handleMouseInput(const irr::SEvent::SMouseInput &mouseInput);
  bool handleGUIEvent(const irr::SEvent::SGUIEvent &guiEvent);
  bool handleCameraMovement(int deltaX, int deltaY);
  bool handleMouseWheelMovement(float wheel);
  bool handleNodeSelection(const irr::SEvent::SMouseInput &mouseInput);
  bool handleCharacterMovement(irr::EKEY_CODE key);
  bool moveCamera(irr::EKEY_CODE key, irr::scene::ICameraSceneNode *camera);
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
