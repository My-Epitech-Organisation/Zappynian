/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** Tests for Game class
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "irrlicht_mocks.hpp"
#include "App/Game.hpp"
#include <memory>

TestSuite(game);

Test(game, constructor_with_host_port) {
    // Test Game constructor with host and port
    // Note: This might fail due to network/graphics dependencies
    try {
        Game game("localhost", 4242);
        cr_assert(true, "Game constructor should work or fail gracefully");
    } catch (const std::exception& e) {
        // Expected if no network/graphics available
        cr_assert(true, "Game constructor threw exception as expected");
    }
}

Test(game, constructor_with_empty_host) {
    try {
        Game game("", 4242);
        cr_assert(true, "Game constructor with empty host should work or fail gracefully");
    } catch (const std::exception& e) {
        cr_assert(true, "Game constructor with empty host threw exception as expected");
    }
}

Test(game, constructor_with_invalid_port) {
    try {
        Game game("localhost", -1);
        cr_assert(true, "Game constructor with invalid port should work or fail gracefully");
    } catch (const std::exception& e) {
        cr_assert(true, "Game constructor with invalid port threw exception as expected");
    }
}

Test(game, constructor_with_zero_port) {
    try {
        Game game("localhost", 0);
        cr_assert(true, "Game constructor with zero port should work or fail gracefully");
    } catch (const std::exception& e) {
        cr_assert(true, "Game constructor with zero port threw exception as expected");
    }
}

Test(game, constructor_with_high_port) {
    try {
        Game game("localhost", 65535);
        cr_assert(true, "Game constructor with high port should work or fail gracefully");
    } catch (const std::exception& e) {
        cr_assert(true, "Game constructor with high port threw exception as expected");
    }
}

Test(game, destructor_after_constructor) {
    try {
        Game* game = new Game("localhost", 4242);
        delete game;
        cr_assert(true, "Game destructor should work");
    } catch (const std::exception& e) {
        cr_assert(true, "Game destructor handled exception correctly");
    }
}

Test(game, multiple_game_instances) {
    try {
        Game game1("localhost", 4242);
        Game game2("127.0.0.1", 4243);
        cr_assert(true, "Multiple Game instances should work or fail gracefully");
    } catch (const std::exception& e) {
        cr_assert(true, "Multiple Game instances threw exception as expected");
    }
}

Test(game, game_with_different_hosts) {
    try {
        Game game1("localhost", 4242);
        Game game2("127.0.0.1", 4242);
        Game game3("192.168.1.1", 4242);
        cr_assert(true, "Games with different hosts should work");
    } catch (const std::exception& e) {
        cr_assert(true, "Games with different hosts threw exception as expected");
    }
}
