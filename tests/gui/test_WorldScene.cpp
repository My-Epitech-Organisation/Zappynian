// Unitary tests for WorldScene (logic only)
#include "../../gui/Core/WorldScene.hpp"
#include <gtest/gtest.h>
#include <memory>

// Utilise des nullptr pour device/smgr/driver, EventReceiver réel

TEST(WorldSceneTest, AddTeamAndGetEntities) {
    EventReceiver receiver;
    irr::IrrlichtDevice *device = nullptr;
    irr::scene::ISceneManager *smgr = nullptr;
    irr::video::IVideoDriver *driver = nullptr;
    irr::io::path mediaPath = "assets/";
    WorldScene scene(device, smgr, driver, receiver, mediaPath);
    scene.addTeam("Red");
    scene.addTeam("Blue");
    EXPECT_TRUE(scene.getEntities().empty());
}

TEST(WorldSceneTest, SetPlayerInventoryNoCrash) {
    EventReceiver receiver;
    irr::IrrlichtDevice *device = nullptr;
    irr::scene::ISceneManager *smgr = nullptr;
    irr::video::IVideoDriver *driver = nullptr;
    irr::io::path mediaPath = "assets/";
    WorldScene scene(device, smgr, driver, receiver, mediaPath);
    // Ne crash pas même si aucun joueur n'est présent
    scene.setPlayerInventory(1, 0, 0, 1, 2, 3, 4, 5, 6, 7);
    SUCCEED();
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
