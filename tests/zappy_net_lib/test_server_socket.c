/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Test server socket functionality
*/

#include <criterion/criterion.h>
#include <unistd.h>
#include <sys/socket.h>
#include "../../libzappy_net/include/zappy_net.h"

Test(server_socket, test_server_listen_invalid_port_zero)
{
    zn_socket_t sock = zn_server_listen(0);

    cr_assert_null(sock, "Server listen with port 0 should fail");
}

Test(server_socket, test_server_listen_invalid_port_negative)
{
    zn_socket_t sock = zn_server_listen(-1);

    cr_assert_null(sock, "Server listen with negative port should fail");
}

Test(server_socket, test_server_listen_invalid_port_too_high)
{
    zn_socket_t sock = zn_server_listen(65536);

    cr_assert_null(sock, "Server listen with port > 65535 should fail");
}

Test(server_socket, test_server_listen_success)
{
    zn_socket_t sock = zn_server_listen(8080);

    cr_assert_not_null(sock, "Server listen should succeed");
    cr_assert_eq(zn_close(sock), 0, "Socket close should succeed");
}

Test(server_socket, test_server_listen_privileged_port)
{
    zn_socket_t sock = zn_server_listen(80);

    if (geteuid() == 0) {
        cr_assert_not_null(sock, "Server listen on port 80 should succeed as root");
        if (sock)
            zn_close(sock);
    } else {
        cr_assert_null(sock, "Server listen on port 80 should fail as non-root");
    }
}

Test(server_socket, test_server_listen_port_in_use)
{
    zn_socket_t sock1 = zn_server_listen(8081);
    zn_socket_t sock2 = zn_server_listen(8081);

    cr_assert_not_null(sock1, "First server listen should succeed");
    cr_assert_null(sock2, "Second server listen on same port should fail");
    if (sock1)
        zn_close(sock1);
}
