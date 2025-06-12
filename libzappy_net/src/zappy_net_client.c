/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Zappy Network Library - Client Functions
*/

#include "zappy_net_internal.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <errno.h>

static int resolve_hostname(const char *host, struct sockaddr_in *addr)
{
    struct hostent *he;

    he = gethostbyname(host);
    if (he == NULL) {
        if (inet_pton(AF_INET, host, &addr->sin_addr) <= 0) {
            return -1;
        }
    } else {
        memcpy(&addr->sin_addr, he->h_addr_list[0], he->h_length);
    }
    return 0;
}

static int setup_client_socket(zn_socket_t sock, const char *host, int port)
{
    if (resolve_hostname(host, &sock->addr) < 0) {
        return -1;
    }
    sock->addr.sin_family = AF_INET;
    sock->addr.sin_port = htons(port);
    sock->type = 2;
    return 0;
}

static int connect_to_server(zn_socket_t sock)
{
    int result;

    result = connect(sock->fd, (struct sockaddr *)&sock->addr,
        sizeof(sock->addr));
    if (result < 0 && errno != EINPROGRESS) {
        return -1;
    }
    return 0;
}

static int create_and_init_socket(zn_socket_t *sock)
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

zn_socket_t zn_client_connect(const char *host, int port)
{
    zn_socket_t sock;

    if (host == NULL || port <= 0 || port > 65535) {
        return NULL;
    }
    if (create_and_init_socket(&sock) < 0) {
        return NULL;
    }
    if (setup_client_socket(sock, host, port) < 0) {
        zn_socket_destroy(sock);
        return NULL;
    }
    if (connect_to_server(sock) < 0) {
        zn_socket_destroy(sock);
        return NULL;
    }
    return sock;
}
