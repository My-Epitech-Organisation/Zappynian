/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** write_to_client
*/

#include "../includes/server.h"

void send_to_client(client_t *client, const char *msg)
{
    int len = strlen(msg);

    if (len > 0 && client && client->zn_sock) {
        zn_write(client->zn_sock, msg, len);
        zn_flush(client->zn_sock);
    }
}

void handle_client_write(server_connection_t *connection, int client_idx)
{
    client_t *client = connection->clients[client_idx];

    if (client && client->zn_sock) {
        ssize_t bytes = zn_flush(client->zn_sock);
        if (bytes < 0 && errno != EAGAIN && errno != EWOULDBLOCK) {
            disconnect_client(connection, client_idx);
        }
    }
}
