/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** display_infos
*/

#include "../include/server.h"

void display_infos(server_args_t *server)
{
    printf("Server configuration:\n");
    printf("Port: %d\n", server->port);
    printf("Width: %zu\n", server->width);
    printf("Height: %zu\n", server->height);
    printf("Teams:\n");
    for (int i = 0; i < server->team_count; i++)
        printf(" - %s\n", server->team_names[i]);
    printf("Clients per team: %d\n", server->clients_per_team);
    printf("Frequency: %d\n", server->frequency);
}
