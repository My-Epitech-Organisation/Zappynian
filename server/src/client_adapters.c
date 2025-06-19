/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** Client adapters for network integration
*/

#include "../include/server.h"

void init_client_zappy_socket(client_t *client, zn_socket_t zn_sock)
{
    client->zn_sock = zn_sock;
}

void cleanup_client_zappy_socket(client_t *client)
{
    if (client->zn_sock != NULL) {
        zn_close(client->zn_sock);
        client->zn_sock = NULL;
    }
}
