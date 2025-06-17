/*
** EPITECH PROJECT, 2025
** Jetpack
** File description:
** Error handling functions
*/

#include "../includes/server.h"

void close_connection(server_connection_t *connection)
{
    if (connection == NULL) {
        fprintf(stderr, "Connection is NULL.\n");
        return;
    }

    if (connection->zn_server != NULL) {
        zn_close(connection->zn_server);
    }

    if (connection->clients != NULL) {
        for (int i = 0; i < connection->client_count; i++) {
            if (connection->clients[i] != NULL) {
                if (connection->clients[i]->zn_sock != NULL) {
                    zn_close(connection->clients[i]->zn_sock);
                }
                free(connection->clients[i]->team_name);
                free(connection->clients[i]);
            }
        }
        free(connection->clients);
    }

    free(connection);
}

void handle_error_connection(char *msg, server_connection_t *connection)
{
    close_connection(connection);
    perror(msg);
    exit(EXIT_FAILURE);
}
