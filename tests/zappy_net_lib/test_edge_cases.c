/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Test socket edge cases and error handling
*/

#include <criterion/criterion.h>
#include <sys/resource.h>
#include <unistd.h>
#include "../../libzappy_net/include/zappy_net.h"

Test(edge_cases, test_multiple_servers_different_ports)
{
    zn_socket_t sock1 = zn_server_listen(8085);
    zn_socket_t sock2 = zn_server_listen(8086);
    zn_socket_t sock3 = zn_server_listen(8087);

    cr_assert_not_null(sock1, "First server should start");
    cr_assert_not_null(sock2, "Second server should start");
    cr_assert_not_null(sock3, "Third server should start");

    zn_close(sock1);
    zn_close(sock2);
    zn_close(sock3);
}

Test(edge_cases, test_multiple_clients_same_server)
{
    zn_socket_t server = zn_server_listen(8088);
    zn_socket_t client1 = zn_client_connect("localhost", 8088);
    zn_socket_t client2 = zn_client_connect("localhost", 8088);
    zn_socket_t client3 = zn_client_connect("localhost", 8088);

    cr_assert_not_null(server, "Server should start");
    cr_assert_not_null(client1, "First client should connect");
    cr_assert_not_null(client2, "Second client should connect");
    cr_assert_not_null(client3, "Third client should connect");

    zn_close(server);
    zn_close(client1);
    zn_close(client2);
    zn_close(client3);
}

Test(edge_cases, test_rapid_connect_disconnect)
{
    for (int i = 0; i < 10; i++) {
        zn_socket_t sock = zn_client_connect("localhost", 22);
        if (sock) {
            zn_close(sock);
        }
    }
}

Test(edge_cases, test_high_port_numbers)
{
    zn_socket_t sock1 = zn_server_listen(32767);
    zn_socket_t sock2 = zn_server_listen(65535);

    cr_assert_not_null(sock1, "Server on port 32767 should work");
    cr_assert_not_null(sock2, "Server on port 65535 should work");

    if (sock1)
        zn_close(sock1);
    if (sock2)
        zn_close(sock2);
}
