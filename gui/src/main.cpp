/*
** EPITECH PROJECT, 2025
** testparshme
** File description:
** main
*/

#include <cmath>
#include <irrlicht/irrlicht.h>
#include <vector>

class EventReceiver : public irr::IEventReceiver {
public:
  EventReceiver()
      : device(nullptr), smgr(nullptr), driver(nullptr), isMouseDown(false),
        mouseX(0), mouseY(0), animatedNode(nullptr), isMoving(false),
        moveStartTime(0), moveStartX(0.0f), moveStartZ(0.0f),
        currentRotationY(0.0f) {}

  bool OnEvent(const irr::SEvent &event) {
    if (event.EventType == irr::EET_KEY_INPUT_EVENT &&
        event.KeyInput.PressedDown) {
      if (animatedNode && !isMoving) {
        switch (event.KeyInput.Key) {
        case irr::KEY_KEY_Z:
          currentRotationY = 180.0f;
          animatedNode->setRotation(
              irr::core::vector3df(0, currentRotationY, 0));
          return true;
        case irr::KEY_KEY_S:
          currentRotationY = 0.0f;
          animatedNode->setRotation(
              irr::core::vector3df(0, currentRotationY, 0));
          return true;
        case irr::KEY_KEY_Q:
          currentRotationY = 90.0f;
          animatedNode->setRotation(
              irr::core::vector3df(0, currentRotationY, 0));
          return true;
        case irr::KEY_KEY_D:
          currentRotationY = 270.0f;
          animatedNode->setRotation(
              irr::core::vector3df(0, currentRotationY, 0));
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

  void setDevice(irr::IrrlichtDevice *dev) {
    device = dev;
    if (device) {
      smgr = device->getSceneManager();
      driver = device->getVideoDriver();
    }
  }

  void addCube(irr::scene::IMeshSceneNode *c) { cubes.push_back(c); }

  void setText(irr::gui::IGUIStaticText *t) { text = t; }

  void setAnimatedNode(irr::scene::IAnimatedMeshSceneNode *node) {
    animatedNode = node;
  }

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

int main() {
  EventReceiver receiver;

  irr::IrrlichtDevice *device = irr::createDevice(
      irr::video::EDT_OPENGL, irr::core::dimension2d<irr::u32>(1920, 1080), 16,
      false, true, false, &receiver);

  if (!device)
    return 1;

  receiver.setDevice(device);

  irr::video::IVideoDriver *driver = device->getVideoDriver();
  irr::scene::ISceneManager *smgr = device->getSceneManager();
  irr::gui::IGUIEnvironment *guienv = device->getGUIEnvironment();

  device->setWindowCaption(L"Zappy FPS: 0");

  const irr::io::path mediaPath = "assets/";

  irr::video::ITexture *image2 = driver->getTexture(mediaPath + "grass.png");
  driver->makeColorKeyTexture(image2, irr::core::position2d<irr::s32>(0, 0));

  irr::scene::IAnimatedMeshSceneNode *node = smgr->addAnimatedMeshSceneNode(
      smgr->getMesh(mediaPath + "archer.b3d"), 0, 1 | 2);
  node->setScale(irr::core::vector3df(8.0f, 8.0f, 8.0f));
  node->setPosition(irr::core::vector3df(0, 6, 0));
  node->setRotation(irr::core::vector3df(0, 0, 0));
  node->setAnimationSpeed(0.0f);
  receiver.setAnimatedNode(node);
  for (int i = 0; i < node->getMaterialCount(); ++i) {
    node->getMaterial(i).setFlag(irr::video::EMF_LIGHTING, false);
    node->getMaterial(i).setFlag(irr::video::EMF_ZBUFFER, true);
    node->getMaterial(i).setFlag(irr::video::EMF_ZWRITE_ENABLE, true);
    node->getMaterial(i).setFlag(irr::video::EMF_BACK_FACE_CULLING, false);
    node->getMaterial(i).Lighting = true;
    node->getMaterial(i).NormalizeNormals = true;
  }
  node->getMaterial(0).setTexture(
      0, driver->getTexture(mediaPath +
                            "archer_texture/DarkBrown_CrossBow01.png"));
  node->getMaterial(1).setTexture(
      0, driver->getTexture(mediaPath + "archer_texture/Black_CrossBow01.png"));
  node->getMaterial(2).setTexture(
      0, driver->getTexture(mediaPath + "archer_texture/Brown_CrossBow01.png"));

  node->getMaterial(3).setTexture(
      0, driver->getTexture(mediaPath + "archer_texture/ay_head.png"));

  node->getMaterial(4).setTexture(
      0, driver->getTexture(mediaPath +
                            "archer_texture/RedTeam_Archer_Armor_Red.png"));
  node->getMaterial(5).setTexture(
      0, driver->getTexture(mediaPath +
                            "archer_texture/BlueTeam_Archer_Helmet_Black.png"));
  node->getMaterial(6).setTexture(
      0, driver->getTexture(mediaPath +
                            "archer_texture/RedTeam_Archer_Armor_Red.png"));
  node->getMaterial(7).setTexture(
      0, driver->getTexture(
             mediaPath + "archer_texture/RedTeam_Archer_Armor_DarkBrown.png"));
  node->getMaterial(8).setTexture(
      0, driver->getTexture(mediaPath +
                            "archer_texture/RedTeam_Archer_Armor_Brown.png"));
  node->getMaterial(9).setTexture(
      0, driver->getTexture(
             mediaPath + "archer_texture/RedTeam_Archer_Gloves_DarkRed.png"));
  node->getMaterial(10).setTexture(
      0, driver->getTexture(
             mediaPath + "archer_texture/RedTeam_Archer_Shoulders_Gray.png"));
  node->getMaterial(11).setTexture(
      0, driver->getTexture(
             mediaPath + "archer_texture/RedTeam_Archer_Shoulders_Gray.png"));
  node->getMaterial(12).setTexture(
      0, driver->getTexture(mediaPath +
                            "archer_texture/RedTeam_Archer_Quiver_Brown.png"));
  node->getMaterial(13).setTexture(
      0, driver->getTexture(mediaPath +
                            "archer_texture/RedTeam_Archer_Belt_Brown.png"));

  irr::scene::IAnimatedMeshSceneNode *Rnode = smgr->addAnimatedMeshSceneNode(smgr->getMesh(mediaPath + "ruby.b3d"),
						0, 1 | 2);
	Rnode->setScale(irr::core::vector3df(0.01f, 0.01f, 0.01f));
	Rnode->setPosition(irr::core::vector3df(0,5,-60));
	Rnode->setRotation(irr::core::vector3df(0,90,0));
	Rnode->setAnimationSpeed(10.0f);
  for (int i = 0; i < Rnode->getMaterialCount(); ++i) {
    Rnode->getMaterial(i).Lighting = true;
    Rnode->getMaterial(i).NormalizeNormals = true;
  }
  Rnode->getMaterial(0).setTexture(0, driver->getTexture(mediaPath + "archer_texture/RedTeam_Archer_Armor_Red.png"));

  irr::u32 frames = 0;
  irr::u32 frames_cube = 0;

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

  smgr->addLightSceneNode(0, irr::core::vector3df(0, 100, -100),
                          irr::video::SColorf(1.0f, 1.0f, 1.0f), 800.0f);
  smgr->addLightSceneNode(0, irr::core::vector3df(-100, 100, -100),
                          irr::video::SColorf(0.6f, 0.6f, 0.6f), 800.0f);
  smgr->addLightSceneNode(0, irr::core::vector3df(100, 100, -100),
                          irr::video::SColorf(0.6f, 0.6f, 0.6f), 800.0f);

  smgr->addCameraSceneNode(0, irr::core::vector3df(0, 50, -100),
                           irr::core::vector3df(0, 30, 0));

  irr::core::vector3df rotation(0.0f, 0.0f, 0.0f);

  irr::gui::IGUIStaticText *text = guienv->addStaticText(
      L"Hello bb chat <3 !", irr::core::rect<irr::s32>(10, 10, 220, 27), true);

  irr::gui::IGUIFont *font =
      guienv->getFont(mediaPath + "fonthaettenschweiler.bmp");
  if (font) {
    printf("Font loaded successfully.\n");
    text->setOverrideFont(font);
  }

  receiver.setText(text);
  receiver.setAnimatedNode(node);

  while (device->run()) {
    irr::u32 currentTime = device->getTimer()->getTime();

    if (receiver.isMoving) {
      float elapsedTime = (currentTime - receiver.moveStartTime) / 1000.0f;
      if (elapsedTime >= 1.0f) {
        irr::core::vector3df pos;
        float angle = receiver.currentRotationY * M_PI / 180.0f;
        pos.X = receiver.moveStartX - 20.0f * sin(angle);
        pos.Z = receiver.moveStartZ - 20.0f * cos(angle);
        pos.Y = node->getPosition().Y;
        node->setPosition(pos);
        receiver.isMoving = false;
        node->setAnimationSpeed(0.0f);
      } else {
        float progress = elapsedTime / 1.0f;
        irr::core::vector3df pos;
        float angle = receiver.currentRotationY * M_PI / 180.0f;
        pos.X = receiver.moveStartX - (20.0f * progress * sin(angle));
        pos.Z = receiver.moveStartZ - (20.0f * progress * cos(angle));
        pos.Y = node->getPosition().Y;
        node->setPosition(pos);
      }
    }

    driver->beginScene(true, true, irr::video::SColor(255, 255, 128, 0));

    guienv->drawAll();
    smgr->drawAll();

    driver->endScene();

    if (++frames == 100) {
      irr::core::stringw str = L"Zappy ";
      str += L" FPS: ";
      str += (irr::s32)driver->getFPS();

      device->setWindowCaption(str.c_str());
      frames = 0;
    }
  }

  device->drop();
  return 0;
}
