/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Zappy Network Library - Socket Operations
*/

#include "zappy_net_internal.h"

#include <fcntl.h>

int zn_set_nonblocking(zn_socket_t sock, int enabled)
{
    int flags;

    if (sock == NULL || sock->fd < 0) {
        return -1;
    }
    flags = fcntl(sock->fd, F_GETFL, 0);
    if (flags < 0) {
        return -1;
    }
    if (enabled) {
        flags |= O_NONBLOCK;
    } else {
        flags &= ~O_NONBLOCK;
    }
    if (fcntl(sock->fd, F_SETFL, flags) < 0) {
        return -1;
    }
    return 0;
}
