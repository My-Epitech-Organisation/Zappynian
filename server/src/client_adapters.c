/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** Client adapters for network integration
*/

#include "../includes/server.h"

static void init_client_socket_field(client_t *client, zn_socket_t zn_sock)
{
    client->zn_sock = zn_sock;
    if (zn_sock != NULL) {
        client->fd = zn_get_fd(zn_sock);
    } else {
        client->fd = -1;
    }
}

static void cleanup_client_socket_field(client_t *client)
{
    if (client->zn_sock != NULL) {
        zn_close(client->zn_sock);
        client->zn_sock = NULL;
    }
    client->fd = -1;
}

void init_client_zappy_socket(client_t *client, zn_socket_t zn_sock)
{
    init_client_socket_field(client, zn_sock);
}

void cleanup_client_zappy_socket(client_t *client)
{
    cleanup_client_socket_field(client);
}
