/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** WorldScene
*/

#include "WorldScene.hpp"

WorldScene::~WorldScene() {}

void WorldScene::createEntities(int id, int x, int y, Direction direction,
                                int level, std::string team) {
  entityManager_.createPlayers(id, x, y, direction, level, team);
  entity_ = entityManager_.getEntities();
}

void WorldScene::createLights() {
  smgr_->addLightSceneNode(0, irr::core::vector3df(0, 100, -100),
                           irr::video::SColorf(1.0f, 1.0f, 1.0f), 800.0f);
  smgr_->addLightSceneNode(0, irr::core::vector3df(-100, 100, -100),
                           irr::video::SColorf(0.6f, 0.6f, 0.6f), 800.0f);
  smgr_->addLightSceneNode(0, irr::core::vector3df(100, 100, -100),
                           irr::video::SColorf(0.6f, 0.6f, 0.6f), 800.0f);
}

void WorldScene::createCamera() {
  smgr_->addCameraSceneNode(0, irr::core::vector3df(0, 50, -100),
                            irr::core::vector3df(0, 30, 0));
}

void WorldScene::createPlane(int x, int y) {
  entityManager_.createTiles(x, y);
  entityManager_.createStones();
  entity_ = entityManager_.getEntities();
}

void WorldScene::createText() {
  irr::gui::IGUIStaticText *text = smgr_->getGUIEnvironment()->addStaticText(
      L"Hello bb chat <3 !", irr::core::rect<irr::s32>(10, 10, 220, 27), true);

  irr::gui::IGUIFont *font = smgr_->getGUIEnvironment()->getFont(
      mediaPath_ + "fonthaettenschweiler.bmp");
  if (font) {
    printf("Font loaded successfully.\n");
    text->setOverrideFont(font);
  }

  receiver_.setText(text);
}