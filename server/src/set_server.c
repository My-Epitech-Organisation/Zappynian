/*
** EPITECH PROJECT, 2025
** Jetpack
** File description:
** Server socket setup functions with libzappy_net integration
*/

#include "../include/server.h"

static int initialize_server_connection(server_connection_t *connection)
{
    connection->zn_server = zn_server_listen(connection->port);
    if (connection->zn_server == NULL) {
        handle_error_connection("zn_server_listen", connection);
        return -1;
    }
    return 0;
}

void set_server(server_connection_t *connection)
{
    if (connection == NULL) {
        fprintf(stderr, "Server connection is NULL.\n");
        exit(84);
    }
    if (initialize_server_connection(connection) == -1) {
        fprintf(stderr,
            "Failed to initialize server connection.\n");
        exit(84);
    }
    connection->clients = NULL;
}
