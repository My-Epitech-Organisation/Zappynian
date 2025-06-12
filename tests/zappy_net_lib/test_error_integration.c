/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** Integration test for error handling in library functions
*/

#include <criterion/criterion.h>
#include "../../libzappy_net/include/zappy_net.h"

TestSuite(error_integration);

Test(error_integration, test_client_connect_invalid_params)
{
    zn_socket_t sock = zn_client_connect(NULL, 8080);

    cr_assert_eq(sock, NULL, "Should return NULL for invalid host");
    cr_assert_eq(zn_last_error(), ZN_ERROR_INVALID_PARAMS,
        "Should set INVALID_PARAMS error");
}

Test(error_integration, test_client_connect_invalid_port)
{
    zn_socket_t sock = zn_client_connect("localhost", -1);

    cr_assert_eq(sock, NULL, "Should return NULL for invalid port");
    cr_assert_eq(zn_last_error(), ZN_ERROR_INVALID_PARAMS,
        "Should set INVALID_PARAMS error");
}

Test(error_integration, test_client_connect_hostname_resolution_failure)
{
    zn_socket_t sock = zn_client_connect("invalid.nonexistent.host", 8080);

    cr_assert_eq(sock, NULL, "Should return NULL for invalid hostname");
    cr_assert_eq(zn_last_error(), ZN_ERROR_HOSTNAME_RESOLUTION,
        "Should set HOSTNAME_RESOLUTION error");
}
