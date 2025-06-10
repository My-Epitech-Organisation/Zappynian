/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Zappy Network Library - Server Functions
*/

#include "zappy_net_internal.h"

#include <sys/socket.h>

static int setup_server_socket(zn_socket_t sock, int port)
{
    int opt = 1;

    if (setsockopt(sock->fd, SOL_SOCKET, SO_REUSEADDR, &opt,
        sizeof(opt)) < 0) {
        return -1;
    }
    sock->addr.sin_family = AF_INET;
    sock->addr.sin_addr.s_addr = INADDR_ANY;
    sock->addr.sin_port = htons(port);
    sock->type = 1;
    return 0;
}

static int bind_and_listen(zn_socket_t sock)
{
    if (bind(sock->fd, (struct sockaddr *)&sock->addr,
        sizeof(sock->addr)) < 0) {
        return -1;
    }
    if (listen(sock->fd, SOMAXCONN) < 0) {
        return -1;
    }
    return 0;
}

static int create_and_init_server_socket(zn_socket_t *sock)
{
    *sock = zn_socket_create();
    if (*sock == NULL) {
        return -1;
    }
    if (zn_socket_init(*sock) < 0) {
        zn_socket_destroy(*sock);
        return -1;
    }
    return 0;
}

zn_socket_t zn_server_listen(int port)
{
    zn_socket_t sock;

    if (port <= 0 || port > 65535) {
        return NULL;
    }
    if (create_and_init_server_socket(&sock) < 0) {
        return NULL;
    }
    if (setup_server_socket(sock, port) < 0) {
        zn_socket_destroy(sock);
        return NULL;
    }
    if (bind_and_listen(sock) < 0) {
        zn_socket_destroy(sock);
        return NULL;
    }
    return sock;
}
