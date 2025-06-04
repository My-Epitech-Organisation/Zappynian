/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** EntityManager
*/

#include "EntityManager.hpp"

void EntityManager::createPlayers(irr::scene::ISceneManager *smgr_,
                                  irr::video::IVideoDriver *driver_,
                                  EventReceiver &receiver_) {
  std::vector<irr::io::path> texturesArcher = {
      mediaPath_ + "archer_texture/DarkBrown_CrossBow01.png",
      mediaPath_ + "archer_texture/Black_CrossBow01.png",
      mediaPath_ + "archer_texture/Brown_CrossBow01.png",
      mediaPath_ + "archer_texture/ay_head.png",
      mediaPath_ + "archer_texture/RedTeam_Archer_Armor_Red.png",
      mediaPath_ + "archer_texture/BlueTeam_Archer_Helmet_Black.png",
      mediaPath_ + "archer_texture/RedTeam_Archer_Armor_Red.png",
      mediaPath_ + "archer_texture/RedTeam_Archer_Armor_DarkBrown.png",
      mediaPath_ + "archer_texture/RedTeam_Archer_Armor_Brown.png",
      mediaPath_ + "archer_texture/RedTeam_Archer_Gloves_DarkRed.png",
      mediaPath_ + "archer_texture/RedTeam_Archer_Shoulders_Gray.png",
      mediaPath_ + "archer_texture/RedTeam_Archer_Shoulders_Gray.png",
      mediaPath_ + "archer_texture/RedTeam_Archer_Quiver_Brown.png",
      mediaPath_ + "archer_texture/RedTeam_Archer_Belt_Brown.png"};

  entity_.push_back(std::make_shared<PlayerEntity>(
      1, irr::core::vector3df(0, 6, 0), irr::core::vector3df(8.0f, 8.0f, 8.0f),
      Direction::NORTH, "Red", texturesArcher, mediaPath_ + "archer.b3d"));

  entity_[0]->createNode(smgr_, driver_, receiver_);
  receiver_.setAnimatedNode(entity_[0]->getNode());
}

void EntityManager::createStones(irr::scene::ISceneManager *smgr_,
                                 irr::video::IVideoDriver *driver_,
                                 EventReceiver &receiver_) {
  std::vector<irr::io::path> texturesStone = {
      mediaPath_ + "archer_texture/RedTeam_Archer_Armor_Red.png"};

  entity_.push_back(std::make_shared<Stone>(
      1, irr::core::vector3df(0, 5, -60),
      irr::core::vector3df(0.01f, 0.01f, 0.01f), texturesStone,
      mediaPath_ + "ruby.b3d", "linemate"));

  entity_[1]->createNode(smgr_, driver_, receiver_);
}