/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** EventReceiver stub for testing (minimal implementation)
*/

#include "../../gui/Event/EventReceiver.hpp"
#include <algorithm>

EventReceiver::EventReceiver() 
    : device(nullptr), smgr(nullptr), driver(nullptr), isMouseDown(false),
      mouseX(0), mouseY(0), animatedNode(nullptr), isMoving(false),
      moveStartTime(0), moveStartX(0.0f), moveStartZ(0.0f),
      currentRotationY(0.0f), currentEntityId(0), textCube(nullptr),
      textPlayer(nullptr) {}

EventReceiver::~EventReceiver() {}

bool EventReceiver::OnEvent(const irr::SEvent &event) {
    (void)event;
    return false;
}

void EventReceiver::setDevice(irr::IrrlichtDevice *dev) {
    device = dev;
}

void EventReceiver::addCube(TileEntity *c) {
    if (c) {
        cubes.push_back(c);
    }
}

void EventReceiver::addEntity(std::shared_ptr<IEntity> entity) {
    if (entity) {
        entity_.push_back(entity);
    }
}

void EventReceiver::removeEntity(int id) {
    entity_.erase(
        std::remove_if(entity_.begin(), entity_.end(),
            [id](const std::shared_ptr<IEntity>& e) {
                return e && e->getId() == id;
            }),
        entity_.end());
}

void EventReceiver::clearAllEntities() {
    entity_.clear();
}

void EventReceiver::setText(irr::gui::IGUIStaticText *t) {
    textCube = t;
}

void EventReceiver::setAnimatedNode(irr::scene::IAnimatedMeshSceneNode *node) {
    animatedNode = node;
}

bool EventReceiver::handleKeyInput(const irr::SEvent::SKeyInput &keyInput) {
    (void)keyInput;
    return false;
}

bool EventReceiver::handleMouseInput(const irr::SEvent::SMouseInput &mouseInput) {
    (void)mouseInput;
    return false;
}

bool EventReceiver::handleGUIEvent(const irr::SEvent::SGUIEvent &guiEvent) {
    (void)guiEvent;
    return false;
}

bool EventReceiver::handleCameraMovement(int deltaX, int deltaY) {
    (void)deltaX; (void)deltaY;
    return false;
}

bool EventReceiver::handleMouseWheelMovement(float wheel) {
    (void)wheel;
    return false;
}

bool EventReceiver::handleNodeSelection(const irr::SEvent::SMouseInput &mouseInput) {
    (void)mouseInput;
    return false;
}

bool EventReceiver::moveCamera(irr::EKEY_CODE key, irr::scene::ICameraSceneNode *camera) {
    (void)key; (void)camera;
    return false;
}

bool EventReceiver::handlePlayerSelection(const irr::SEvent::SMouseInput &mouseInput) {
    (void)mouseInput;
    return false;
}
