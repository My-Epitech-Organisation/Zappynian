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

static int should_disconnect_client(ssize_t bytes)
{
    if (bytes >= 0)
        return 0;
    return (errno != EAGAIN && errno != EWOULDBLOCK);
}

void handle_client_write(server_t *server, int client_idx)
{
    server_connection_t *connection = server->connection;
    client_t *client = connection->clients[client_idx];
    ssize_t bytes = 0;

    if (!client || !client->zn_sock)
        return;
    bytes = zn_flush(client->zn_sock);
    if (should_disconnect_client(bytes))
        disconnect_client(server, client_idx);
}
