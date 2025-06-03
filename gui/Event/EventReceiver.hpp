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

  bool isMoving;
  irr::u32 moveStartTime;
  float moveStartX;
  float moveStartZ;
  float currentRotationY;

private:
  irr::IrrlichtDevice *device;
  irr::scene::ISceneManager *smgr;
  irr::video::IVideoDriver *driver;
  std::vector<irr::scene::IMeshSceneNode *> cubes;
  irr::gui::IGUIStaticText *text;
  bool isMouseDown;
  int mouseX, mouseY;
  irr::scene::IAnimatedMeshSceneNode *animatedNode;
};
