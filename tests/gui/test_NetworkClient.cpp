// Unitary tests for NetworkClient (GUI)
#include "../../gui/Network/NetworkClient.hpp"
#include <gtest/gtest.h>
#include <memory>

// Test parseMessage for pnw command
TEST(NetworkClientTest, ParseMessagePNW) {
    irr::IrrlichtDevice *device = nullptr;
    irr::scene::ISceneManager *smgr = nullptr;
    irr::video::IVideoDriver *driver = nullptr;
    EventReceiver receiver;
    irr::io::path mediaPath = "assets/";
    NetworkClient client(device, smgr, driver, receiver, mediaPath);
    std::string msg = "pnw 1 2 3 NORTH 4 Red";
    EXPECT_NO_THROW(client.parseMessage(msg));
}

// Test parseMessage for bct (map) command
TEST(NetworkClientTest, ParseMessageBCTMap) {
    irr::IrrlichtDevice *device = nullptr;
    irr::scene::ISceneManager *smgr = nullptr;
    irr::video::IVideoDriver *driver = nullptr;
    EventReceiver receiver;
    irr::io::path mediaPath = "assets/";
    NetworkClient client(device, smgr, driver, receiver, mediaPath);
    std::string msg = "bct 1 2 3 4 5 6 7 8 9 10 11";
    EXPECT_NO_THROW(client.parseMessage(msg));
}

// Test parseMessage for bct (tile) command
TEST(NetworkClientTest, ParseMessageBCTTile) {
    irr::IrrlichtDevice *device = nullptr;
    irr::scene::ISceneManager *smgr = nullptr;
    irr::video::IVideoDriver *driver = nullptr;
    EventReceiver receiver;
    irr::io::path mediaPath = "assets/";
    NetworkClient client(device, smgr, driver, receiver, mediaPath);
    std::string msg = "bct 1 2 3 4 5 6 7 8 9 10";
    EXPECT_NO_THROW(client.parseMessage(msg));
}

// Test parseMessage with invalid command
TEST(NetworkClientTest, ParseMessageInvalid) {
    irr::IrrlichtDevice *device = nullptr;
    irr::scene::ISceneManager *smgr = nullptr;
    irr::video::IVideoDriver *driver = nullptr;
    EventReceiver receiver;
    irr::io::path mediaPath = "assets/";
    NetworkClient client(device, smgr, driver, receiver, mediaPath);
    std::string msg = "invalid 1 2 3";
    EXPECT_NO_THROW(client.parseMessage(msg));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
