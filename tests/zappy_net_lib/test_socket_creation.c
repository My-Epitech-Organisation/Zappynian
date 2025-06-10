/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Test socket creation and destruction
*/

#include <criterion/criterion.h>
#include "../../libzappy_net/include/zappy_net.h"

Test(socket_creation, test_socket_create_success)
{
    zn_socket_t sock = zn_socket_create();

    cr_assert_not_null(sock, "Socket creation should not return NULL");
    zn_socket_destroy(sock);
}

Test(socket_creation, test_socket_destroy_null)
{
    zn_socket_destroy(NULL);
}

Test(socket_creation, test_socket_init_success)
{
    zn_socket_t sock = zn_socket_create();

    cr_assert_not_null(sock, "Socket creation should not return NULL");
    cr_assert_eq(zn_socket_init(sock), 0, "Socket init should succeed");
    zn_socket_destroy(sock);
}

Test(socket_creation, test_socket_init_null)
{
    cr_assert_eq(zn_socket_init(NULL), -1, "Socket init with NULL should fail");
}

Test(socket_creation, test_socket_cleanup_null)
{
    zn_socket_cleanup(NULL);
}

Test(socket_creation, test_socket_cleanup_success)
{
    zn_socket_t sock = zn_socket_create();

    cr_assert_not_null(sock, "Socket creation should not return NULL");
    cr_assert_eq(zn_socket_init(sock), 0, "Socket init should succeed");
    zn_socket_cleanup(sock);
    zn_socket_destroy(sock);
}
