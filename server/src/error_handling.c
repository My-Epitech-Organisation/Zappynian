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
    if (connection->fd != -1) {
        close(connection->fd);
    }
    free(connection->clients);
    free(connection->fds);
    free(connection);
}

void handle_error_connection(char *msg, server_connection_t *connection)
{
    close_connection(connection);
    perror(msg);
    exit(EXIT_FAILURE);
}
