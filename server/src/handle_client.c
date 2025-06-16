/*
** EPITECH PROJECT, 2025
** Jetpack
** File description:
** Client connection handling
*/

#include "../include/server.h"

void accept_client(server_connection_t *connection)
{
    client_t *new_client = malloc(sizeof(client_t));

    if (!new_client)
        handle_error_connection("malloc", connection);
    memset(new_client, 0, sizeof(client_t));
    new_client->addr_len = sizeof(new_client->addr);
    new_client->fd = accept(connection->fd,
        (struct sockaddr *) &new_client->addr,
        &new_client->addr_len);
    if (new_client->fd == -1) {
        perror("accept");
        free(new_client);
        return;
    }
    connection->clients = realloc(connection->clients,
        (connection->client_count + 1) * sizeof(client_t *));
    connection->clients[connection->client_count] = new_client;
    connection->fds[connection->nfds].fd = new_client->fd;
    connection->fds[connection->nfds].events = POLLIN;
    connection->nfds++;
}

void check_read_client(server_connection_t *connection, int current_idx)
{
    int client_fd = connection->fds[current_idx].fd;

    for (int i = 0; i < connection->client_count; i++) {
        if (connection->clients[i]->fd == client_fd) {
            handle_client_read(connection, i);
            break;
        }
    }
}

void loop_clients(server_connection_t *connection, int current_idx)
{
    if (connection->fds[current_idx].revents & POLLIN) {
        if (connection->fds[current_idx].fd == connection->fd &&
            connection->client_count < MAX_CLIENTS) {
            accept_client(connection);
            connection->client_count++;
        } else
            check_read_client(connection, current_idx);
    }
}

void handle_clients(server_t *server)
{
    server->connection->clients = calloc(MAX_CLIENTS, sizeof(client_t *));
    server->connection->client_count = 0;
    server->connection->fds = malloc(sizeof(struct pollfd) *
        (MAX_CLIENTS + 1));
    server->connection->nfds = 1;
    server->connection->fds[0].fd = server->connection->fd;
    server->connection->fds[0].events = POLLIN;
    while (1) {
        if (poll(server->connection->fds, server->connection->nfds, -1) == -1)
            handle_error_connection("poll", server->connection);
        for (int current_idx = 0;
            current_idx < server->connection->nfds; current_idx++)
            loop_clients(server->connection, current_idx);
    }
}
