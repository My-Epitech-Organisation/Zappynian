/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Zappy Network Library - Server Functions
*/

#include "zappy_net_internal.h"

#include <sys/socket.h>
#include <string.h>
#include <unistd.h>

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
    printf("sock->fd = %d\n", sock->fd);
    printf("sock->initialized = %d\n", sock->initialized);
    printf("sock->type = %d\n", sock->type);
    printf("sock->addr.sin_family = %d\n", sock->addr.sin_family);
    printf("sock->addr.sin_port = %d\n", ntohs(sock->addr.sin_port));
    printf("sock->addr.sin_addr.s_addr = %u\n", ntohl(sock->addr.sin_addr.s_addr));
    if (bind(sock->fd, (struct sockaddr *)&sock->addr,
        sizeof(sock->addr)) < 0) {
        printf("je suis nul 1\n");
        return -1;
    }
    if (listen(sock->fd, SOMAXCONN) < 0) {
        printf("je suis nul 2\n");
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

    if (port < 0 || port > 65535) {
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
        printf("%d\n", bind_and_listen(sock));
        printf("mais nn pourquoi je destroy\n");
        zn_socket_destroy(sock);
        return NULL;
    }
    printf("sock->fd = %d\n", sock->fd);
    printf("sock->initialized = %d\n", sock->initialized);
    printf("sock->type = %d\n", sock->type);
    printf("sock->addr.sin_family = %d\n", sock->addr.sin_family);
    printf("sock->addr.sin_port = %d\n", ntohs(sock->addr.sin_port));
    printf("sock->addr.sin_addr.s_addr = %u\n", ntohl(sock->addr.sin_addr.s_addr));
    if (sock == NULL)
        printf("je suis null cognio\n");
    return sock;
}

static void setup_default_addr_params(struct sockaddr **addr,
    socklen_t **len, struct sockaddr_in *client_addr, socklen_t *addr_len)
{
    if (*addr == NULL || *len == NULL) {
        *addr = (struct sockaddr *)client_addr;
        *addr_len = sizeof(*client_addr);
        *len = addr_len;
    }
}

static zn_socket_t create_client_socket(int client_fd,
    struct sockaddr *addr, socklen_t addr_len)
{
    zn_socket_t client_sock;

    client_sock = zn_socket_create();
    if (client_sock == NULL) {
        close(client_fd);
        return NULL;
    }
    client_sock->fd = client_fd;
    client_sock->initialized = 1;
    client_sock->type = 0;
    memcpy(&client_sock->addr, addr, addr_len);
    return client_sock;
}

zn_socket_t zn_accept(zn_socket_t server_sock,
    struct sockaddr *addr, socklen_t *len)
{
    int client_fd;
    socklen_t addr_len;
    struct sockaddr_in client_addr;

    if (server_sock == NULL || server_sock->fd < 0) {
        return NULL;
    }
    setup_default_addr_params(&addr, &len, &client_addr, &addr_len);
    client_fd = accept(server_sock->fd, addr, len);
    if (client_fd < 0) {
        return NULL;
    }
    return create_client_socket(client_fd, addr, *len);
}
