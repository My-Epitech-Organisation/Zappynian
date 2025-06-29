/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** Server polling operations
*/

#include "../include/server.h"

void setup_socket_array(server_connection_t *connection,
    zn_socket_t *sockets, int *count)
{
    *count = 0;
    sockets[0] = connection->zn_server;
    *count = 1;
    for (int i = 0; i < connection->client_count &&
        *count < ZN_POLL_MAX_SOCKETS;
        i++) {
        if (connection->clients[i] != NULL &&
            connection->clients[i]->zn_sock != NULL) {
            sockets[*count] = connection->clients[i]->zn_sock;
            (*count)++;
        }
    }
}

int find_client_by_socket(server_connection_t *connection,
    zn_socket_t socket)
{
    int j;

    for (j = 0; j < connection->client_count; j++) {
        if (connection->clients[j] != NULL &&
            connection->clients[j]->zn_sock == socket) {
            return j;
        }
    }
    return -1;
}

static int has_client_pending_writes(server_connection_t *connection,
    zn_socket_t socket)
{
    if (!connection || !socket)
        return 0;
    return zn_has_pending_writes(socket);
}

void setup_poll_events(short *events, zn_socket_t *sockets,
    server_connection_t *connection, int count)
{
    int i;

    for (i = 0; i < count; i++) {
        events[i] = POLLIN;
        if (i > 0 && has_client_pending_writes(connection, sockets[i]))
            events[i] |= POLLOUT;
    }
}
