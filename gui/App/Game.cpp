/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** Game
*/

#include "Game.hpp"

Game::Game() { initWindow(); }

Game::~Game() {}

void Game::initWindow() {
  device_ = irr::createDevice(irr::video::EDT_OPENGL,
                              irr::core::dimension2d<irr::u32>(1920, 1080), 16,
                              false, true, false, &receiver_);
  driver_ = device_->getVideoDriver();
  smgr_ = device_->getSceneManager();
  guienv_ = device_->getGUIEnvironment();
}

void Game::gameLoop() {
  if (!device_)
    return;

  receiver_.setDevice(device_);

  device_->setWindowCaption(L"Zappy FPS: 0");

  const irr::io::path mediaPath = "assets/";

  irr::video::ITexture *image2 = driver_->getTexture(mediaPath + "grass.png");
  driver_->makeColorKeyTexture(image2, irr::core::position2d<irr::s32>(0, 0));

  std::vector<irr::io::path> texturesArcher = {
      mediaPath + "archer_texture/DarkBrown_CrossBow01.png",
      mediaPath + "archer_texture/Black_CrossBow01.png",
      mediaPath + "archer_texture/Brown_CrossBow01.png",
      mediaPath + "archer_texture/ay_head.png",
      mediaPath + "archer_texture/RedTeam_Archer_Armor_Red.png",
      mediaPath + "archer_texture/BlueTeam_Archer_Helmet_Black.png",
      mediaPath + "archer_texture/RedTeam_Archer_Armor_Red.png",
      mediaPath + "archer_texture/RedTeam_Archer_Armor_DarkBrown.png",
      mediaPath + "archer_texture/RedTeam_Archer_Armor_Brown.png",
      mediaPath + "archer_texture/RedTeam_Archer_Gloves_DarkRed.png",
      mediaPath + "archer_texture/RedTeam_Archer_Shoulders_Gray.png",
      mediaPath + "archer_texture/RedTeam_Archer_Shoulders_Gray.png",
      mediaPath + "archer_texture/RedTeam_Archer_Quiver_Brown.png",
      mediaPath + "archer_texture/RedTeam_Archer_Belt_Brown.png"};

  entity_.push_back(std::make_shared<PlayerEntity>(
      1, irr::core::vector3df(0, 6, 0),
      irr::core::vector3df(8.0f, 8.0f, 8.0f), Direction::NORTH, "Red",
      texturesArcher, mediaPath + "archer.b3d"));

  entity_[0]->createNode(smgr_, driver_, receiver_);

  std::vector<irr::io::path> texturesStone = {
      mediaPath + "archer_texture/RedTeam_Archer_Armor_Red.png"};

  entity_.push_back(std::make_shared<Stone>(
      1, irr::core::vector3df(0, 5, -60),
      irr::core::vector3df(0.01f, 0.01f, 0.01f), texturesStone,
      mediaPath + "ruby.b3d", "linemate"));

  entity_[1]->createNode(smgr_, driver_, receiver_);

  irr::u32 frames = 0;
  irr::u32 frames_cube = 0;

  std::vector<irr::scene::IMeshSceneNode *> cubes;
  float cubeX = -100.0f;
  float cubeY = -100.0f;
  for (int j = 0; j < 10; ++j) {
    for (int i = 0; i < 10; ++i) {
      irr::scene::IMeshSceneNode *cubeNode = smgr_->addCubeSceneNode(
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
        receiver_.addCube(cubeNode);
        cubeX += 20.0f;
      }
    }
    cubeX = -100.0f;
    cubeY += 20.0f;
  }

  smgr_->addLightSceneNode(0, irr::core::vector3df(0, 100, -100),
                           irr::video::SColorf(1.0f, 1.0f, 1.0f), 800.0f);
  smgr_->addLightSceneNode(0, irr::core::vector3df(-100, 100, -100),
                           irr::video::SColorf(0.6f, 0.6f, 0.6f), 800.0f);
  smgr_->addLightSceneNode(0, irr::core::vector3df(100, 100, -100),
                           irr::video::SColorf(0.6f, 0.6f, 0.6f), 800.0f);

  smgr_->addCameraSceneNode(0, irr::core::vector3df(0, 50, -100),
                            irr::core::vector3df(0, 30, 0));

  irr::core::vector3df rotation(0.0f, 0.0f, 0.0f);

  irr::gui::IGUIStaticText *text = guienv_->addStaticText(
      L"Hello bb chat <3 !", irr::core::rect<irr::s32>(10, 10, 220, 27), true);

  irr::gui::IGUIFont *font =
      guienv_->getFont(mediaPath + "fonthaettenschweiler.bmp");
  if (font) {
    printf("Font loaded successfully.\n");
    text->setOverrideFont(font);
  }

  receiver_.setText(text);
  // receiver_.setAnimatedNode(node);

  while (device_->run()) {
    irr::u32 currentTime = device_->getTimer()->getTime();

    if (receiver_.isMoving) {
      float elapsedTime = (currentTime - receiver_.moveStartTime) / 1000.0f;
      if (elapsedTime >= 1.0f) {
        irr::core::vector3df pos;
        float angle = receiver_.currentRotationY * M_PI / 180.0f;
        pos.X = receiver_.moveStartX - 20.0f * sin(angle);
        pos.Z = receiver_.moveStartZ - 20.0f * cos(angle);
        pos.Y = entity_[0]->getNode()->getPosition().Y;
        entity_[0]->getNode()->setPosition(pos);
        receiver_.isMoving = false;
        entity_[0]->getNode()->setAnimationSpeed(0.0f);
      } else {
        float progress = elapsedTime / 1.0f;
        irr::core::vector3df pos;
        float angle = receiver_.currentRotationY * M_PI / 180.0f;
        pos.X = receiver_.moveStartX - (20.0f * progress * sin(angle));
        pos.Z = receiver_.moveStartZ - (20.0f * progress * cos(angle));
        pos.Y = entity_[0]->getNode()->getPosition().Y;
        entity_[0]->getNode()->setPosition(pos);
      }
    }

    driver_->beginScene(true, true, irr::video::SColor(255, 255, 128, 0));

    guienv_->drawAll();
    smgr_->drawAll();

    driver_->endScene();

    if (++frames == 100) {
      irr::core::stringw str = L"Zappy ";
      str += L" FPS: ";
      str += (irr::s32)driver_->getFPS();

      device_->setWindowCaption(str.c_str());
      frames = 0;
    }
  }

  device_->drop();
}