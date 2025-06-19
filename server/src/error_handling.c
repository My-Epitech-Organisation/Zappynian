/*
** EPITECH PROJECT, 2025
** Jetpack
** File description:
** Error handling functions
*/

#include "../include/server.h"

static void cleanup_single_client(client_t *client)
{
    if (client == NULL)
        return;
    if (client->zn_sock != NULL) {
        zn_close(client->zn_sock);
    }
    free(client->team_name);
    free(client);
}

static void cleanup_client_connections(server_connection_t *connection)
{
    if (connection->clients == NULL)
        return;
    for (int i = 0; i < connection->client_count; i++) {
        cleanup_single_client(connection->clients[i]);
    }
    free(connection->clients);
}

void close_connection(server_connection_t *connection)
{
    if (connection == NULL) {
        fprintf(stderr, "Connection is NULL.\n");
        return;
    }
    if (connection->zn_server != NULL) {
        zn_close(connection->zn_server);
    }
    cleanup_client_connections(connection);
    free(connection);
}

void handle_error_connection(char *msg, server_connection_t *connection)
{
    close_connection(connection);
    perror(msg);
    exit(EXIT_FAILURE);
}
