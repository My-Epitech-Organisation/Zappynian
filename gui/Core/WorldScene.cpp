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
  EntityManager entityManager;
  entityManager.createPlayers(smgr, driver, receiver);
  entityManager.createStones(smgr, driver, receiver);
  entity_ = entityManager.getEntities();
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
  irr::video::ITexture *image2 = driver->getTexture(mediaPath + "grass.png");
  driver->makeColorKeyTexture(image2, irr::core::position2d<irr::s32>(0, 0));
  std::vector<irr::scene::IMeshSceneNode *> cubes;
  float cubeX = -100.0f;
  float cubeY = -100.0f;
  for (int j = 0; j < 10; ++j) {
    for (int i = 0; i < 10; ++i) {
      irr::scene::IMeshSceneNode *cubeNode = smgr->addCubeSceneNode(
          20.0f, 0, -1, irr::core::vector3df(cubeX, 0, cubeY),
          irr::core::vector3df(0, 0, 0),
          irr::core::vector3df(1.0f, 0.5f, 1.0f));
      if (cubeNode) {
        cubeNode->setMaterialFlag(irr::video::EMF_ZBUFFER, true);
        cubeNode->setMaterialFlag(irr::video::EMF_LIGHTING, false);
        cubeNode->setMaterialTexture(0, image2);
        irr::core::stringc name = "Cube info: ";
        name += "row ";
        name += i;
        name += " col ";
        name += j;
        cubeNode->setName(name.c_str());
        cubes.push_back(cubeNode);
        receiver.addCube(cubeNode);
        cubeX += 20.0f;
      }
    }
    cubeX = -100.0f;
    cubeY += 20.0f;
  }
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