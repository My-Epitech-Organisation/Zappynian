/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** write_to_client
*/

#include "../include/server.h"

void send_to_client(client_t *client, const char *msg)
{
    if (!client || !msg || !client->zn_sock)
        return;
    zn_send_message(client->zn_sock, msg);
}

void handle_client_write(server_connection_t *connection, int client_idx)
{
    client_t *client = connection->clients[client_idx];
    ssize_t bytes = 0;

    if (client && client->zn_sock) {
        bytes = zn_flush(client->zn_sock);
        if (bytes < 0 && errno != EAGAIN && errno != EWOULDBLOCK) {
            disconnect_client(connection, client_idx);
        }
    }
}
