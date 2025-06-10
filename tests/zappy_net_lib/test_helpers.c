/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Test helper functions implementation
*/

#include "test_helpers.h"
#include <stdlib.h>

/**
 * @brief Internal socket structure definition (for testing only)
 */
struct zn_socket {
    int fd;
    int initialized;
    struct sockaddr_in addr;
    int type;
};

int test_get_socket_fd(zn_socket_t sock)
{
    if (sock == NULL) {
        return -1;
    }
    return ((struct zn_socket *)sock)->fd;
}

int test_is_socket_initialized(zn_socket_t sock)
{
    if (sock == NULL) {
        return 0;
    }
    return ((struct zn_socket *)sock)->initialized;
}
