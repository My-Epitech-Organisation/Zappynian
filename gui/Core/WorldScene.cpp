/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** WorldScene
*/

#include "WorldScene.hpp"

WorldScene::WorldScene() {}

WorldScene::~WorldScene() {}

void WorldScene::createEntities(irr::scene::ISceneManager *smgr,
                                irr::video::IVideoDriver *driver,
                                EventReceiver &receiver) {
  entityManager_.createPlayers(smgr, driver, receiver);
  entityManager_.createStones(smgr, driver, receiver);
  entity_ = entityManager_.getEntities();
}

void WorldScene::createLights(irr::scene::ISceneManager *smgr) {
  smgr->addLightSceneNode(0, irr::core::vector3df(0, 100, -100),
                          irr::video::SColorf(1.0f, 1.0f, 1.0f), 800.0f);
  smgr->addLightSceneNode(0, irr::core::vector3df(-100, 100, -100),
                          irr::video::SColorf(0.6f, 0.6f, 0.6f), 800.0f);
  smgr->addLightSceneNode(0, irr::core::vector3df(100, 100, -100),
                          irr::video::SColorf(0.6f, 0.6f, 0.6f), 800.0f);
}

void WorldScene::createCamera(irr::scene::ISceneManager *smgr) {
  smgr->addCameraSceneNode(0, irr::core::vector3df(0, 50, -100),
                           irr::core::vector3df(0, 30, 0));
}

void WorldScene::createPlane(irr::scene::ISceneManager *smgr,
                             irr::video::IVideoDriver *driver,
                             EventReceiver &receiver,
                             const irr::io::path &mediaPath) {
  entityManager_.createTiles(smgr, driver, receiver);
}

void WorldScene::createText(irr::gui::IGUIEnvironment *guienv,
                            const irr::io::path &mediaPath,
                            EventReceiver &receiver) {
  irr::gui::IGUIStaticText *text = guienv->addStaticText(
      L"Hello bb chat <3 !", irr::core::rect<irr::s32>(10, 10, 220, 27), true);

  irr::gui::IGUIFont *font =
      guienv->getFont(mediaPath + "fonthaettenschweiler.bmp");
  if (font) {
    printf("Font loaded successfully.\n");
    text->setOverrideFont(font);
  }

  receiver.setText(text);
}