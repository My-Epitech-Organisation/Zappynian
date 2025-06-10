/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Zappy Network Library - Socket Management
*/

#include "../include/zappy_net.h"

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <errno.h>
#include <netdb.h>

/**
 * @brief Internal socket structure definition
 *
 * This structure contains the internal representation of a socket
 * handle. It is opaque to library users to maintain encapsulation.
 */
struct zn_socket {
    int fd; /* Socket file descriptor */
    int initialized; /* Initialization state flag */
    struct sockaddr_in addr; /* Socket address information */
    int type; /* Socket type (server/client) */
};

/* Allocate and return new socket handle */
zn_socket_t zn_socket_create(void)
{
    zn_socket_t socket = malloc(sizeof(struct zn_socket));
    if (socket == NULL) {
        return NULL;
    }

    socket->fd = -1;
    socket->initialized = 0;
    socket->type = 0;
    memset(&socket->addr, 0, sizeof(socket->addr));

    return socket;
}

/* Destroy socket handle and free resources */
void zn_socket_destroy(zn_socket_t sock)
{
    if (sock == NULL) {
        return;
    }

    if (sock->fd >= 0) {
        close(sock->fd);
    }

    free(sock);
}

/* Initialize socket handle */
int zn_socket_init(zn_socket_t sock)
{
    if (sock == NULL) {
        return -1;
    }

    sock->fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock->fd < 0) {
        return -1;
    }

    /* Set socket to non-blocking mode */
    int flags = fcntl(sock->fd, F_GETFL, 0);
    if (flags < 0 || fcntl(sock->fd, F_SETFL, flags | O_NONBLOCK) < 0) {
        close(sock->fd);
        sock->fd = -1;
        return -1;
    }

    sock->initialized = 1;
    return 0;
}

/* Clean up socket handle */
void zn_socket_cleanup(zn_socket_t sock)
{
    if (sock == NULL) {
        return;
    }

    if (sock->fd >= 0) {
        close(sock->fd);
        sock->fd = -1;
    }

    sock->initialized = 0;
}

/* Create a server socket and listen on specified port */
zn_socket_t zn_server_listen(int port)
{
    if (port <= 0 || port > 65535) {
        return NULL;
    }

    zn_socket_t sock = zn_socket_create();
    if (sock == NULL) {
        return NULL;
    }

    /* Initialize the socket */
    if (zn_socket_init(sock) < 0) {
        zn_socket_destroy(sock);
        return NULL;
    }

    /* Enable address reuse */
    int opt = 1;
    if (setsockopt(sock->fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        zn_socket_destroy(sock);
        return NULL;
    }

    /* Setup server address */
    sock->addr.sin_family = AF_INET;
    sock->addr.sin_addr.s_addr = INADDR_ANY;
    sock->addr.sin_port = htons(port);
    sock->type = 1; /* Mark as server socket */

    /* Bind socket to address */
    if (bind(sock->fd, (struct sockaddr *)&sock->addr, sizeof(sock->addr)) < 0) {
        zn_socket_destroy(sock);
        return NULL;
    }

    /* Start listening */
    if (listen(sock->fd, SOMAXCONN) < 0) {
        zn_socket_destroy(sock);
        return NULL;
    }

    return sock;
}

/* Connect to a server as a client */
zn_socket_t zn_client_connect(const char *host, int port)
{
    if (host == NULL || port <= 0 || port > 65535) {
        return NULL;
    }

    zn_socket_t sock = zn_socket_create();
    if (sock == NULL) {
        return NULL;
    }

    /* Initialize the socket */
    if (zn_socket_init(sock) < 0) {
        zn_socket_destroy(sock);
        return NULL;
    }

    /* Resolve hostname */
    struct hostent *he = gethostbyname(host);
    if (he == NULL) {
        /* Try to parse as IP address */
        if (inet_pton(AF_INET, host, &sock->addr.sin_addr) <= 0) {
            zn_socket_destroy(sock);
            return NULL;
        }
    } else {
        memcpy(&sock->addr.sin_addr, he->h_addr_list[0], he->h_length);
    }

    /* Setup client address */
    sock->addr.sin_family = AF_INET;
    sock->addr.sin_port = htons(port);
    sock->type = 2; /* Mark as client socket */

    /* Attempt connection (non-blocking) */
    int result = connect(sock->fd, (struct sockaddr *)&sock->addr, sizeof(sock->addr));
    if (result < 0 && errno != EINPROGRESS) {
        zn_socket_destroy(sock);
        return NULL;
    }

    return sock;
}

/* Close a socket connection */
int zn_close(zn_socket_t sock)
{
    if (sock == NULL) {
        return -1;
    }

    int result = 0;

    /* Close socket with proper error handling */
    if (sock->fd >= 0) {
        do {
            result = close(sock->fd);
        } while (result < 0 && errno == EINTR);
    }

    /* Clean up and destroy the socket */
    zn_socket_destroy(sock);

    return result;
}
