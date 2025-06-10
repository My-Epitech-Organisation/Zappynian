/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Test socket closing functionality
*/

#include <criterion/criterion.h>
#include "../../libzappy_net/include/zappy_net.h"

Test(socket_close, test_close_null_socket)
{
    int result = zn_close(NULL);

    cr_assert_eq(result, -1, "Closing NULL socket should return -1");
}

Test(socket_close, test_close_server_socket)
{
    zn_socket_t sock = zn_server_listen(8083);

    cr_assert_not_null(sock, "Server should start successfully");
    cr_assert_eq(zn_close(sock), 0, "Closing server socket should succeed");
}

Test(socket_close, test_close_client_socket)
{
    zn_socket_t sock = zn_client_connect("localhost", 22);

    cr_assert_not_null(sock, "Client should connect successfully");
    cr_assert_eq(zn_close(sock), 0, "Closing client socket should succeed");
}

Test(socket_close, test_double_close)
{
    zn_socket_t sock = zn_server_listen(8084);

    cr_assert_not_null(sock, "Server should start successfully");
    cr_assert_eq(zn_close(sock), 0, "First close should succeed");
}
