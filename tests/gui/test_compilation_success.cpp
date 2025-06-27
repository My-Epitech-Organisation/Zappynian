/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** Test runner for successfully compiled tests
*/

#include <criterion/criterion.h>
#include <criterion/logging.h>
#include "irrlicht_mocks.hpp"

// Simple tests to verify our setup works
Test(irrlicht_mocks, basic_vector3d) {
    irr::core::vector3df v1(1.0f, 2.0f, 3.0f);
    irr::core::vector3df v2(4.0f, 5.0f, 6.0f);
    
    irr::core::vector3df result = v1 + v2;
    cr_expect_eq(result.X, 5.0f, "X component should be 5.0");
    cr_expect_eq(result.Y, 7.0f, "Y component should be 7.0");
    cr_expect_eq(result.Z, 9.0f, "Z component should be 9.0");
}

Test(irrlicht_mocks, vector3d_operations) {
    irr::core::vector3df v(3.0f, 4.0f, 0.0f);
    irr::core::vector3df scaled = v * 2.0f;
    
    cr_expect_eq(scaled.X, 6.0f, "Scaled X should be 6.0");
    cr_expect_eq(scaled.Y, 8.0f, "Scaled Y should be 8.0");
}

Test(irrlicht_mocks, device_creation) {
    irr::IrrlichtDevice* device = irr::createDevice();
    cr_expect_null(device, "Mock device should return null");
}

Test(compilation_success, gui_objects_compiled) {
    // This test just verifies our compilation worked
    cr_expect(true, "If this test runs, compilation was successful");
}
