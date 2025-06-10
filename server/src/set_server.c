/*
** EPITECH PROJECT, 2025
** Jetpack
** File description:
** Server socket setup functions
*/

#include "../includes/server.h"

int set_server_socket(server_connection_t *connection)
{
    int fd = 0;
    int opt = 1;

    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
        handle_error_connection("setsockopt", connection);
    if (fd == -1)
        handle_error_connection("socket", connection);
    return fd;
}

void set_bind(server_connection_t *connection)
{
    memset(&connection->addr, 0, sizeof(connection->addr));
    if (connection == NULL) {
        fprintf(stderr, "Server connection is NULL.\n");
        exit(84);
    }
    connection->addr.sin_family = AF_INET;
    connection->addr.sin_port = htons(connection->port);
    connection->addr.sin_addr.s_addr = INADDR_ANY;
    if (bind(connection->fd, (struct sockaddr *) &connection->addr,
        sizeof(connection->addr)) == -1)
        handle_error_connection("bind", connection);
}

void set_listen(server_connection_t *connection)
{
    if (listen(connection->fd, MAX_CLIENTS) == -1)
        handle_error_connection("listen", connection);
}

void set_server(server_connection_t *connection)
{
    if (connection == NULL) {
        fprintf(stderr, "Server connection is NULL.\n");
        exit(84);
    }
    connection->fd = set_server_socket(connection);
    set_bind(connection);
    set_listen(connection);
    connection->clients = NULL;
}
