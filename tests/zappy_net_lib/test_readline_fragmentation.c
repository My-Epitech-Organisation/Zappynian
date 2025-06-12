/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Test zn_readline() with TCP fragmentation support
*/

#define _POSIX_C_SOURCE 200809L //for usleep
#include <criterion/criterion.h>
#include <errno.h>
#include <unistd.h>
#include "../../libzappy_net/include/zappy_net.h"

Test(zn_readline, invalid_parameters_test)
{
    char *line;

    cr_assert_eq(zn_init(), 0, "Failed to initialize zappy_net");
    line = zn_readline(NULL);
    cr_assert_null(line, "Should return NULL for NULL socket");
    cr_assert_eq(errno, EINVAL, "Should set errno to EINVAL");
    zn_cleanup();
}

Test(zn_readline, basic_functionality_test)
{
    zn_socket_t server_sock, client_sock;
    char *line;

    cr_assert_eq(zn_init(), 0, "Failed to initialize zappy_net");
    
    // Create server socket on a specific port
    server_sock = zn_server_listen(8083);
    cr_assert_not_null(server_sock, "Failed to create server socket");
    
    // Create client socket
    client_sock = zn_client_connect("127.0.0.1", 8083);
    cr_assert_not_null(client_sock, "Failed to connect to server");
    
    // Test readline on a socket without data - should return NULL
    line = zn_readline(client_sock);
    cr_assert_null(line, "Should return NULL when no data available");
    
    // Test that writing and flushing works (basic API test)
    cr_assert_eq(zn_write(client_sock, "test\n", 5), 5, "Should write 5 bytes");
    cr_assert_eq(zn_flush(client_sock), 5, "Should flush 5 bytes");
    
    zn_close(client_sock);
    zn_close(server_sock);
    zn_cleanup();
}
