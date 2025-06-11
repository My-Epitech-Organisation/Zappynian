/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Zappy Network Library - Socket Management (Basic Functions)
*/

#include "zappy_net_internal.h"
#include "../include/zappy_net_ringbuf.h"

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <errno.h>

zn_socket_t zn_socket_create(void)
{
    zn_socket_t socket = malloc(sizeof(struct zn_socket));

    if (socket == NULL) {
        return NULL;
    }
    socket->fd = -1;
    socket->initialized = 0;
    socket->type = 0;
    socket->buffer_initialized = 0;
    memset(&socket->addr, 0, sizeof(socket->addr));
    return socket;
}

void zn_socket_destroy(zn_socket_t sock)
{
    if (sock == NULL) {
        return;
    }
    if (sock->fd >= 0) {
        close(sock->fd);
    }
    if (sock->buffer_initialized) {
        zn_ringbuf_cleanup(&sock->read_buffer);
        zn_ringbuf_cleanup(&sock->write_buffer);
    }
    free(sock);
}

int zn_socket_init(zn_socket_t sock)
{
    int flags;

    if (sock == NULL) {
        return -1;
    }
    sock->fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock->fd < 0) {
        return -1;
    }
    flags = fcntl(sock->fd, F_GETFL, 0);
    if (flags < 0 || fcntl(sock->fd, F_SETFL, flags | O_NONBLOCK) < 0) {
        close(sock->fd);
        sock->fd = -1;
        return -1;
    }
    sock->initialized = 1;
    return 0;
}

void zn_socket_cleanup(zn_socket_t sock)
{
    if (sock == NULL) {
        return;
    }
    if (sock->fd >= 0) {
        close(sock->fd);
        sock->fd = -1;
    }
    if (sock->buffer_initialized) {
        zn_ringbuf_cleanup(&sock->read_buffer);
        zn_ringbuf_cleanup(&sock->write_buffer);
        sock->buffer_initialized = 0;
    }
    sock->initialized = 0;
}

int zn_close(zn_socket_t sock)
{
    int result = 0;

    if (sock == NULL) {
        return -1;
    }
    if (sock->fd >= 0) {
        do {
            result = close(sock->fd);
        } while (result < 0 && errno == EINTR);
    }
    zn_socket_destroy(sock);
    return result;
}
