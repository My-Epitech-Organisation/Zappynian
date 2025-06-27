/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** Tests for Config class
*/

#include <criterion/criterion.h>
#include "App/Config.hpp"

TestSuite(config);

Test(config, construction_destruction) {
    // Test that Config can be constructed and destructed without issues
    Config* config = new Config();
    cr_assert_not_null(config);
    delete config;
}

Test(config, multiple_instances) {
    // Test that multiple Config instances can coexist
    Config config1;
    Config config2;
    Config config3;
    
    // Just test that they can be created without issues
    // Since Config is currently empty, there's not much to test
    cr_assert(true);
}

Test(config, stack_allocation) {
    // Test stack allocation
    Config config;
    cr_assert(true);
}

Test(config, constructor_destructor_balance) {
    // Test that constructor and destructor are properly balanced
    for (int i = 0; i < 10; ++i) {
        Config config;
        // Each iteration should construct and destruct cleanly
    }
    cr_assert(true);
}
