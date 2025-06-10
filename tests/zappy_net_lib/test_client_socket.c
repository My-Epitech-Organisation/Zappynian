/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Test client socket functionality
*/

#include <criterion/criterion.h>
#include <unistd.h>
#include <pthread.h>
#include "../../libzappy_net/include/zappy_net.h"

Test(client_socket, test_client_connect_null_host)
{
    zn_socket_t sock = zn_client_connect(NULL, 8080);

    cr_assert_null(sock, "Client connect with NULL host should fail");
}

Test(client_socket, test_client_connect_invalid_port_zero)
{
    zn_socket_t sock = zn_client_connect("localhost", 0);

    cr_assert_null(sock, "Client connect with port 0 should fail");
}

Test(client_socket, test_client_connect_invalid_port_negative)
{
    zn_socket_t sock = zn_client_connect("localhost", -1);

    cr_assert_null(sock, "Client connect with negative port should fail");
}

Test(client_socket, test_client_connect_invalid_port_too_high)
{
    zn_socket_t sock = zn_client_connect("localhost", 65536);

    cr_assert_null(sock, "Client connect with port > 65535 should fail");
}

Test(client_socket, test_client_connect_invalid_host)
{
    zn_socket_t sock = zn_client_connect("invalid.host.name.test", 8080);

    cr_assert_null(sock, "Client connect to invalid host should fail");
}

Test(client_socket, test_client_connect_localhost_ip)
{
    zn_socket_t sock = zn_client_connect("127.0.0.1", 22);

    cr_assert_not_null(sock, "Client connect to 127.0.0.1:22 should succeed");
    if (sock)
        zn_close(sock);
}

Test(client_socket, test_client_connect_localhost_name)
{
    zn_socket_t sock = zn_client_connect("localhost", 22);

    cr_assert_not_null(sock, "Client connect to localhost:22 should succeed");
    if (sock)
        zn_close(sock);
}
