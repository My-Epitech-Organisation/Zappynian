/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** handle_free
*/

#include "../includes/server.h"

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

int handle_free(server_t *server)
{
    if (server == NULL)
        return 0;
    if (server->args != NULL) {
        handle_free_args(server->args);
    }
    if (server->connection->clients != NULL) {
        for (int i = 0; i < server->connection->client_count; i++)
            close(server->connection->clients[i]->fd);
        free(server->connection->clients);
    }
    free(server);
    return 0;
}
