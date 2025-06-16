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
        zn_socket_destroy(sock);
        return NULL;
    }
    return sock;
}

zn_socket_t zn_accept(zn_socket_t server_sock, struct sockaddr *addr, socklen_t *len)
{
    zn_socket_t client_sock;
    int client_fd;
    socklen_t addr_len;
    struct sockaddr_in client_addr;

    if (server_sock == NULL || server_sock->fd < 0) {
        return NULL;
    }
    
    /* If no addr/len provided, use local variables */
    if (addr == NULL || len == NULL) {
        addr = (struct sockaddr *)&client_addr;
        addr_len = sizeof(client_addr);
        len = &addr_len;
    }
    
    /* Accept the connection */
    client_fd = accept(server_sock->fd, addr, len);
    if (client_fd < 0) {
        return NULL;  /* No connection available or error */
    }
    
    /* Create a new socket handle for the client */
    client_sock = zn_socket_create();
    if (client_sock == NULL) {
        close(client_fd);
        return NULL;
    }
    
    /* Set up the client socket */
    client_sock->fd = client_fd;
    client_sock->initialized = 1;
    client_sock->type = 0;  /* Client socket type */
    if (addr == (struct sockaddr *)&client_addr) {
        memcpy(&client_sock->addr, &client_addr, sizeof(client_addr));
    } else {
        memcpy(&client_sock->addr, addr, *len);
    }
    
    return client_sock;
}
