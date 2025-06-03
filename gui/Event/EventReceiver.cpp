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
  if (event.EventType == irr::EET_KEY_INPUT_EVENT &&
      event.KeyInput.PressedDown) {
    if (animatedNode && !isMoving) {
      switch (event.KeyInput.Key) {
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
    }
  }

  if (event.EventType == irr::EET_KEY_INPUT_EVENT) {
    if (event.KeyInput.Key == irr::KEY_ESCAPE &&
        event.KeyInput.PressedDown == false) {
      device->closeDevice();
      return true;
    }
  }

  if (event.EventType == irr::EET_MOUSE_INPUT_EVENT) {
    if (event.MouseInput.Event == irr::EMIE_MOUSE_MOVED) {
      if (isMouseDown && smgr) {
        irr::scene::ICameraSceneNode *camera = smgr->getActiveCamera();
        irr::core::vector3df pos = camera->getPosition();
        irr::core::vector3df target = camera->getTarget();
        irr::core::vector3df dir = pos - target;
        irr::core::matrix4 mat;

        int deltaX = event.MouseInput.X - mouseX;
        int deltaY = event.MouseInput.Y - mouseY;

        mat.setRotationDegrees(
            irr::core::vector3df(0.0f, -deltaX * 0.5f, 0.0f));
        mat.rotateVect(dir);

        irr::core::vector3df hAxis =
            dir.crossProduct(irr::core::vector3df(0.0f, 1.0f, 0.0f));
        hAxis.normalize();
        mat.setRotationAxisRadians(-deltaY * 0.01f, hAxis);
        mat.rotateVect(dir);

        camera->setPosition(target + dir);
        camera->setTarget(target);
      }
      mouseX = event.MouseInput.X;
      mouseY = event.MouseInput.Y;
      return true;
    } else if (event.MouseInput.Event == irr::EMIE_LMOUSE_PRESSED_DOWN) {
      isMouseDown = true;
      mouseX = event.MouseInput.X;
      mouseY = event.MouseInput.Y;
      return true;
    } else if (event.MouseInput.Event == irr::EMIE_LMOUSE_LEFT_UP) {
      isMouseDown = false;
      if (smgr && driver) {
        irr::core::line3d<irr::f32> ray =
            smgr->getSceneCollisionManager()->getRayFromScreenCoordinates(
                irr::core::position2d<irr::s32>(event.MouseInput.X,
                                                event.MouseInput.Y),
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
      }
      return true;
    } else if (event.MouseInput.Event == irr::EMIE_MOUSE_WHEEL) {
      if (smgr) {
        irr::scene::ICameraSceneNode *camera = smgr->getActiveCamera();
        irr::core::vector3df pos = camera->getPosition();
        irr::core::vector3df target = camera->getTarget();
        irr::core::vector3df dir = (target - pos);
        dir.normalize();

        float distance = 10.0f;
        if (event.MouseInput.Wheel < 0) {
          camera->setPosition(pos - dir * distance);
        } else {
          camera->setPosition(pos + dir * distance);
        }
        return true;
      }
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
