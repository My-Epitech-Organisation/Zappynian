/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** EventReceiver
*/

#include "EventReceiver.hpp"
#include "../Entities/PlayerEntity.hpp"
#include "../Entities/TileEntity.hpp"
#include <iostream>
#include <string>

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
    if (handleCharacterMovement(keyInput.Key))
      return true;
  }

  if (keyInput.Key == irr::KEY_ESCAPE && !keyInput.PressedDown) {
    device->closeDevice();
    return true;
  }

  if (!smgr)
    return false;

  irr::scene::ICameraSceneNode *camera = smgr->getActiveCamera();
  if (!camera)
    return false;

  if (moveCamera(keyInput.Key, camera)) {
    return true;
  }

  return false;
}

bool EventReceiver::moveCamera(irr::EKEY_CODE key,
                               irr::scene::ICameraSceneNode *camera) {
  float moveSpeed = 5.0f;
  irr::core::vector3df pos = camera->getPosition();
  irr::core::vector3df target = camera->getTarget();
  irr::core::vector3df direction = (target - pos).normalize();
  irr::core::vector3df rightVector =
      direction.crossProduct(irr::core::vector3df(0, 1, 0));
  rightVector.normalize();

  switch (key) {
  case irr::KEY_UP:
    pos += direction * moveSpeed;
    target += direction * moveSpeed;
    break;
  case irr::KEY_DOWN:
    pos -= direction * moveSpeed;
    target -= direction * moveSpeed;
    break;
  case irr::KEY_LEFT:
    pos += rightVector * moveSpeed;
    target += rightVector * moveSpeed;
    break;
  case irr::KEY_RIGHT:
    pos -= rightVector * moveSpeed;
    target -= rightVector * moveSpeed;
    break;
  default:
    return false;
  }

  camera->setPosition(pos);
  camera->setTarget(target);

  return true;
}

bool EventReceiver::handleCharacterMovement(irr::EKEY_CODE key) {
  switch (key) {
  case irr::KEY_KEY_Z:
    setCurrentRotationY(180.0f);
    animatedNode->setRotation(
        irr::core::vector3df(0, getCurrentRotationY(), 0));
    return true;
  case irr::KEY_KEY_S:
    setCurrentRotationY(0.0f);
    animatedNode->setRotation(
        irr::core::vector3df(0, getCurrentRotationY(), 0));
    return true;
  case irr::KEY_KEY_Q:
    setCurrentRotationY(90.0f);
    animatedNode->setRotation(
        irr::core::vector3df(0, getCurrentRotationY(), 0));
    return true;
  case irr::KEY_KEY_D:
    setCurrentRotationY(270.0f);
    animatedNode->setRotation(
        irr::core::vector3df(0, getCurrentRotationY(), 0));
    return true;
  case irr::KEY_SPACE:
    if (!isMoving) {
      setIsMoving(true);
      setMoveStartTime(device->getTimer()->getTime());
      irr::core::vector3df pos = animatedNode->getPosition();
      setMoveStartX(pos.X);
      setMoveStartZ(pos.Z);
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
    return handleNodeSelection(mouseInput) || handlePlayerSelection(mouseInput);

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
    if (c->getName() == selectedNode->getName()) {
      irr::core::stringw selectedText =
          irr::core::stringw(L"Selected Cube: ") + c->getName().c_str();
      std::vector<std::string> stoneNames = {"food",    "linemate", "deraumere",
                                             "sibur",   "mendiane", "phiras",
                                             "thystame"};
      for (std::size_t i = 0; i < stoneNames.size(); ++i) {
        selectedText += L"\n: ";
        selectedText += irr::core::stringw(stoneNames[i].c_str());
        selectedText += L" (";
        selectedText += irr::core::stringw(
            c->getInventory().getItemQuantity(stoneNames[i]));
        selectedText += L")";
      }
      textCube->setText(selectedText.c_str());
      return true;
    }
  }
  return false;
}

bool EventReceiver::handlePlayerSelection(
    const irr::SEvent::SMouseInput &mouseInput) {
  if (!smgr || !driver || !textPlayer)
    return false;

  irr::core::line3d<irr::f32> ray =
      smgr->getSceneCollisionManager()->getRayFromScreenCoordinates(
          irr::core::position2d<irr::s32>(mouseInput.X, mouseInput.Y),
          smgr->getActiveCamera());

  irr::scene::ISceneNode *selectedNode =
      smgr->getSceneCollisionManager()->getSceneNodeFromRayBB(ray);

  for (const auto &entity : entity_) {
    auto player = std::dynamic_pointer_cast<PlayerEntity>(entity);
    if (player && player->getNode() == selectedNode) {
      irr::core::stringw selectedText = L"Player Info:\n";
      selectedText += L"ID: ";
      selectedText += irr::core::stringw(player->getId());
      selectedText += L"\nTeam: ";
      selectedText += irr::core::stringw(player->getTeam().c_str());
      selectedText += L"\nLevel: ";
      selectedText += irr::core::stringw(player->getLevel());
      textPlayer->setText(selectedText.c_str());
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

void EventReceiver::addCube(TileEntity *c) { cubes.push_back(c); }

void EventReceiver::addEntity(std::shared_ptr<IEntity> entity) {
  entity_.push_back(entity);
}

void EventReceiver::setText(irr::gui::IGUIStaticText *t) { textCube = t; }

void EventReceiver::setAnimatedNode(irr::scene::IAnimatedMeshSceneNode *node) {
  animatedNode = node;
}
