/*
** EPITECH PROJECT, 2025
** Jetpack
** File description:
** Server socket setup functions with libzappy_net integration
*/

#include "../includes/server.h"

static int validate_zappy_server(zn_socket_t zn_server)
{
    if (zn_server == NULL) {
        return -1;
    }
    return 0;
}

static int setup_server_fd(server_connection_t *connection)
{
    connection->fd = zn_get_fd(connection->zn_server);
    if (connection->fd == -1) {
        return -1;
    }
    return 0;
}

int set_server_socket(server_connection_t *connection)
{
    connection->zn_server = zn_server_listen(connection->port);
    if (validate_zappy_server(connection->zn_server) == -1) {
        return -1;
    }
    if (setup_server_fd(connection) == -1) {
        return -1;
    }
    return 0;
}

static int initialize_server_connection(server_connection_t *connection)
{
    if (set_server_socket(connection) == -1) {
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
    initialize_server_connection(connection);
    connection->clients = NULL;
}
