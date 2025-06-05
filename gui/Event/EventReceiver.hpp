/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** EventReceiver
*/

#pragma once
#include <irrlicht/irrlicht.h>
#include <vector>

class EventReceiver : public irr::IEventReceiver {
public:
  EventReceiver();
  virtual ~EventReceiver();

  bool OnEvent(const irr::SEvent &event);

  void setDevice(irr::IrrlichtDevice *dev);

  void addCube(irr::scene::IMeshSceneNode *c);

  void setText(irr::gui::IGUIStaticText *t);

  void setAnimatedNode(irr::scene::IAnimatedMeshSceneNode *node);

  // Getters
  bool getIsMoving() const { return isMoving; }
  irr::u32 getMoveStartTime() const { return moveStartTime; }
  float getMoveStartX() const { return moveStartX; }
  float getMoveStartZ() const { return moveStartZ; }
  float getCurrentRotationY() const { return currentRotationY; }

  // Setters
  void setIsMoving(bool value) { isMoving = value; }
  void setMoveStartTime(irr::u32 value) { moveStartTime = value; }
  void setMoveStartX(float value) { moveStartX = value; }
  void setMoveStartZ(float value) { moveStartZ = value; }
  void setCurrentRotationY(float value) { currentRotationY = value; }

  bool handleKeyInput(const irr::SEvent::SKeyInput &keyInput);
  bool handleMouseInput(const irr::SEvent::SMouseInput &mouseInput);
  bool handleCameraMovement(int deltaX, int deltaY);
  bool handleMouseWheelMovement(float wheel);
  bool handleNodeSelection(const irr::SEvent::SMouseInput &mouseInput);
  bool handleCharacterMovement(irr::EKEY_CODE key);

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

  std::vector<irr::scene::IMeshSceneNode *> cubes;
  irr::gui::IGUIStaticText *text;
};
