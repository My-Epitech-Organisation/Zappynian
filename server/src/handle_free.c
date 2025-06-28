/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** handle_free
*/

#include "../include/server.h"
#include "../include/team.h"

static void free_team_names(server_args_t *server)
{
    for (int i = 0; server->team_names[i]; i++)
        free(server->team_names[i]);
    free(server->team_names);
}

static void free_teams(server_args_t *server)
{
    for (int i = 0; i < server->team_count; i++) {
        if (server->teams[i].name != NULL)
            free(server->teams[i].name);
    }
    free(server->teams);
}

int handle_free_args(server_args_t *server)
{
    if (server->team_names != NULL)
        free_team_names(server);
    if (server->teams != NULL)
        free_teams(server);
    return 0;
}

static void free_single_client(client_t *client)
{
    if (client->zn_sock)
        zn_close(client->zn_sock);
    if (client->team_name)
        free(client->team_name);
}

static void free_clients_array(server_connection_t *connection)
{
    if (connection->clients == NULL)
        return;
    for (int i = 0; i < connection->client_count; i++) {
        if (connection->clients[i])
            free_single_client(connection->clients[i]);
    }
}

int handle_free(server_t *server)
{
    if (server == NULL)
        return 0;
    if (server->args != NULL)
        handle_free_args(server->args);
    if (server->connection != NULL)
        free_clients_array(server->connection);
    if (server->graphic_clients != NULL)
        destroy_graphic_client_list(server->graphic_clients);
    if (server->map != NULL)
        destroy_map(server->map);
    free(server);
    return 0;
}
