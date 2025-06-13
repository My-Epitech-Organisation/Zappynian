/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Simple unit tests for handshake functionality
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "../../libzappy_net/include/zappy_net.h"

TestSuite(simple_handshake);

Test(simple_handshake, role_enum_values)
{
    cr_assert_eq(zn_init(), 0, "Library initialization failed");

    // Test que les valeurs d'énumération sont cohérentes
    cr_assert(ZN_ROLE_AI != ZN_ROLE_GUI, "AI and GUI roles should be different");
    cr_assert(ZN_ROLE_UNKNOWN != ZN_ROLE_AI, "Unknown and AI roles should be different");
    cr_assert(ZN_ROLE_UNKNOWN != ZN_ROLE_GUI, "Unknown and GUI roles should be different");

    zn_cleanup();
}

Test(simple_handshake, handshake_result_structure)
{
    zn_handshake_result_t result = {0};

    cr_assert_eq(zn_init(), 0, "Library initialization failed");

    // Test que la structure peut être initialisée
    result.client_num = 42;
    result.world_x = 100;
    result.world_y = 200;

    cr_assert_eq(result.client_num, 42, "client_num should be set correctly");
    cr_assert_eq(result.world_x, 100, "world_x should be set correctly");
    cr_assert_eq(result.world_y, 200, "world_y should be set correctly");

    zn_cleanup();
}
