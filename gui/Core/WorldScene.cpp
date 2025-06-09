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

void WorldScene::createEntities(int x, int y, int q0, int q1, int q2, int q3,
                                int q4, int q5, int q6, int nbTiles) {
  entityManager_.createStones(x, y, q0, q1, q2, q3, q4, q5, q6, nbTiles);
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

void WorldScene::createPlane(int x, int y) { entityManager_.createTiles(x, y); }

void WorldScene::createText() {
  irr::gui::IGUIStaticText *text = smgr_->getGUIEnvironment()->addStaticText(
      L"Hello bb chat <3 !", irr::core::rect<irr::s32>(10, 10, 220, 200),
      false);

  irr::gui::IGUIFont *font = smgr_->getGUIEnvironment()->getFont(
      mediaPath_ + "fonthaettenschweiler.bmp");
  if (font) {
    printf("Font loaded successfully.\n");
    text->setOverrideFont(font);
  }

  receiver_.setText(text);
}