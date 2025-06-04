/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** EventReceiver
*/

#include "EventReceiver.hpp"

EventReceiver::EventReceiver()
    : device(nullptr), smgr(nullptr), driver(nullptr), isMouseDown(false),
      mouseX(0), mouseY(0), animatedNode(nullptr), isMoving(false),
      moveStartTime(0), moveStartX(0.0f), moveStartZ(0.0f),
      currentRotationY(0.0f) {}

EventReceiver::~EventReceiver() = default;

bool EventReceiver::OnEvent(const irr::SEvent &event) {
  if (event.EventType == irr::EET_KEY_INPUT_EVENT) {
    return handleKeyInput(event.KeyInput);
  }
  if (event.EventType == irr::EET_MOUSE_INPUT_EVENT) {
    return handleMouseInput(event.MouseInput);
  }
  return false;
}

bool EventReceiver::handleKeyInput(const irr::SEvent::SKeyInput &keyInput) {
  if (keyInput.PressedDown && animatedNode && !isMoving) {
    if (handleCharacterMovement(keyInput.Key)) {
      return true;
    }
  }

  if (keyInput.Key == irr::KEY_ESCAPE && !keyInput.PressedDown) {
    device->closeDevice();
    return true;
  }
  return false;
}

bool EventReceiver::handleCharacterMovement(irr::EKEY_CODE key) {
  switch (key) {
  case irr::KEY_KEY_Z:
    currentRotationY = 180.0f;
    animatedNode->setRotation(irr::core::vector3df(0, currentRotationY, 0));
    return true;
  case irr::KEY_KEY_S:
    currentRotationY = 0.0f;
    animatedNode->setRotation(irr::core::vector3df(0, currentRotationY, 0));
    return true;
  case irr::KEY_KEY_Q:
    currentRotationY = 90.0f;
    animatedNode->setRotation(irr::core::vector3df(0, currentRotationY, 0));
    return true;
  case irr::KEY_KEY_D:
    currentRotationY = 270.0f;
    animatedNode->setRotation(irr::core::vector3df(0, currentRotationY, 0));
    return true;
  case irr::KEY_SPACE:
    if (!isMoving) {
      isMoving = true;
      moveStartTime = device->getTimer()->getTime();
      irr::core::vector3df pos = animatedNode->getPosition();
      moveStartX = pos.X;
      moveStartZ = pos.Z;
      animatedNode->setAnimationSpeed(26.0f);
      return true;
    }
    break;
  }
  return false;
}

bool EventReceiver::handleMouseInput(
    const irr::SEvent::SMouseInput &mouseInput) {
  switch (mouseInput.Event) {
  case irr::EMIE_MOUSE_MOVED:
    if (isMouseDown && smgr) {
      int deltaX = mouseInput.X - mouseX;
      int deltaY = mouseInput.Y - mouseY;
      if (handleCameraMovement(deltaX, deltaY)) {
        mouseX = mouseInput.X;
        mouseY = mouseInput.Y;
        return true;
      }
    }
    mouseX = mouseInput.X;
    mouseY = mouseInput.Y;
    return true;

  case irr::EMIE_LMOUSE_PRESSED_DOWN:
    isMouseDown = true;
    mouseX = mouseInput.X;
    mouseY = mouseInput.Y;
    return true;

  case irr::EMIE_LMOUSE_LEFT_UP:
    isMouseDown = false;
    return handleNodeSelection(mouseInput);

  case irr::EMIE_MOUSE_WHEEL:
    return handleMouseWheelMovement(mouseInput.Wheel);
  }
  return false;
}

bool EventReceiver::handleCameraMovement(int deltaX, int deltaY) {
  if (!smgr)
    return false;

  irr::scene::ICameraSceneNode *camera = smgr->getActiveCamera();
  irr::core::vector3df pos = camera->getPosition();
  irr::core::vector3df target = camera->getTarget();
  irr::core::vector3df dir = pos - target;
  irr::core::matrix4 mat;

  mat.setRotationDegrees(irr::core::vector3df(0.0f, -deltaX * 0.5f, 0.0f));
  mat.rotateVect(dir);

  irr::core::vector3df hAxis =
      dir.crossProduct(irr::core::vector3df(0.0f, 1.0f, 0.0f));
  hAxis.normalize();
  mat.setRotationAxisRadians(-deltaY * 0.01f, hAxis);
  mat.rotateVect(dir);

  camera->setPosition(target + dir);
  camera->setTarget(target);
  return true;
}

bool EventReceiver::handleMouseWheelMovement(float wheel) {
  if (!smgr)
    return false;

  irr::scene::ICameraSceneNode *camera = smgr->getActiveCamera();
  irr::core::vector3df pos = camera->getPosition();
  irr::core::vector3df target = camera->getTarget();
  irr::core::vector3df dir = (target - pos);
  dir.normalize();

  float distance = 10.0f;
  if (wheel < 0) {
    camera->setPosition(pos - dir * distance);
  } else {
    camera->setPosition(pos + dir * distance);
  }
  return true;
}

bool EventReceiver::handleNodeSelection(
    const irr::SEvent::SMouseInput &mouseInput) {
  if (!smgr || !driver)
    return false;

  irr::core::line3d<irr::f32> ray =
      smgr->getSceneCollisionManager()->getRayFromScreenCoordinates(
          irr::core::position2d<irr::s32>(mouseInput.X, mouseInput.Y),
          smgr->getActiveCamera());

  irr::scene::ISceneNode *selectedNode =
      smgr->getSceneCollisionManager()->getSceneNodeFromRayBB(ray);

  for (auto &c : cubes) {
    if (c == selectedNode) {
      printf("Cube Name: %s\n", c->getName());
      irr::core::stringw selectedText =
          irr::core::stringw(L"Selected Cube: ") + c->getName();
      text->setText(selectedText.c_str());
      return true;
    }
  }
  return false;
}

void EventReceiver::setDevice(irr::IrrlichtDevice *dev) {
  device = dev;
  if (device) {
    smgr = device->getSceneManager();
    driver = device->getVideoDriver();
  }
}

void EventReceiver::addCube(irr::scene::IMeshSceneNode *c) {
  cubes.push_back(c);
}

void EventReceiver::setText(irr::gui::IGUIStaticText *t) { text = t; }

void EventReceiver::setAnimatedNode(irr::scene::IAnimatedMeshSceneNode *node) {
  animatedNode = node;
}
