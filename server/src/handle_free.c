/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** handle_free
*/

#include "../include/server.h"

int handle_free_args(server_args_t *server)
{
    if (server->team_names != NULL) {
        for (int i = 0; server->team_names[i]; i++)
            free(server->team_names[i]);
        free(server->team_names);
    }
    free(server);
    return 0;
}

static void free_clients_array(server_connection_t *connection)
{
    if (connection->clients == NULL)
        return;
    for (int i = 0; i < connection->client_count; i++) {
        if (connection->clients[i] && connection->clients[i]->zn_sock) {
            zn_close(connection->clients[i]->zn_sock);
        }
        free(connection->clients[i]);
    }
    free(connection->clients);
}

int handle_free(server_t *server)
{
    if (server == NULL)
        return 0;
    if (server->args != NULL) {
        handle_free_args(server->args);
    }
    if (server->connection != NULL) {
        free_clients_array(server->connection);
    }
    free(server);
    return 0;
}
