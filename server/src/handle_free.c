/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** handle_free
*/

#include "../includes/server.h"

int handle_free(server_config_t *server)
{
    if (server->team_names != NULL) {
        for (int i = 0; server->team_names[i]; i++)
            free(server->team_names[i]);
        free(server->team_names);
    }
    free(server);
    return 0;
}
