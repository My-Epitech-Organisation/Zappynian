/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Test API extensions: zn_accept and zn_set_nonblocking
*/

#include <criterion/criterion.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "../../libzappy_net/include/zappy_net.h"

TestSuite(api_extensions);

Test(api_extensions, test_zn_set_nonblocking_enable)
{
    zn_socket_t sock = zn_socket_create();
    cr_assert_not_null(sock, "Socket creation should succeed");

    cr_assert_eq(zn_socket_init(sock), 0, "Socket initialization should succeed");

    cr_assert_eq(zn_set_nonblocking(sock, 1), 0, "Enabling non-blocking mode should succeed");

    zn_socket_destroy(sock);
}

Test(api_extensions, test_zn_set_nonblocking_disable)
{
    zn_socket_t sock = zn_socket_create();
    cr_assert_not_null(sock, "Socket creation should succeed");

    cr_assert_eq(zn_socket_init(sock), 0, "Socket initialization should succeed");

    // First enable non-blocking
    cr_assert_eq(zn_set_nonblocking(sock, 1), 0, "Enabling non-blocking mode should succeed");

    // Then disable it
    cr_assert_eq(zn_set_nonblocking(sock, 0), 0, "Disabling non-blocking mode should succeed");

    zn_socket_destroy(sock);
}

Test(api_extensions, test_zn_set_nonblocking_null_socket)
{
    cr_assert_eq(zn_set_nonblocking(NULL, 1), -1, "Setting non-blocking on NULL socket should fail");
}

Test(api_extensions, test_zn_set_nonblocking_invalid_socket)
{
    zn_socket_t sock = zn_socket_create();
    cr_assert_not_null(sock, "Socket creation should succeed");

    // Don't initialize the socket, so fd is -1
    cr_assert_eq(zn_set_nonblocking(sock, 1), -1, "Setting non-blocking on uninitialized socket should fail");

    zn_socket_destroy(sock);
}

Test(api_extensions, test_zn_accept_null_server)
{
    zn_socket_t client_sock = zn_accept(NULL, NULL, NULL);
    cr_assert_null(client_sock, "Accept on NULL server socket should return NULL");
}

Test(api_extensions, test_zn_accept_no_pending_connections)
{
    zn_socket_t server_sock = zn_server_listen(0);
    cr_assert_not_null(server_sock, "Server creation should succeed");

    zn_socket_t client_sock = zn_accept(server_sock, NULL, NULL);
    cr_assert_null(client_sock, "Accept with no pending connections should return NULL");

    zn_close(server_sock);
}

Test(api_extensions, test_zn_accept_with_address_parameters)
{
    zn_socket_t server_sock = zn_server_listen(0);
    cr_assert_not_null(server_sock, "Server creation should succeed");

    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);

    // Should not crash with valid address parameters but no pending connections
    zn_socket_t client_sock = zn_accept(server_sock, (struct sockaddr*)&addr, &addr_len);
    cr_assert_null(client_sock, "Accept with address params but no connections should return NULL");

    zn_close(server_sock);
}

Test(api_extensions, test_zn_get_fd_existing_function)
{
    zn_socket_t sock = zn_socket_create();
    cr_assert_not_null(sock, "Socket creation should succeed");

    cr_assert_eq(zn_socket_init(sock), 0, "Socket initialization should succeed");

    int fd = zn_get_fd(sock);
    cr_assert_geq(fd, 0, "zn_get_fd should return valid file descriptor");

    zn_socket_destroy(sock);
}

Test(api_extensions, test_zn_get_fd_null_socket)
{
    cr_assert_eq(zn_get_fd(NULL), -1, "zn_get_fd on NULL socket should return -1");
}
