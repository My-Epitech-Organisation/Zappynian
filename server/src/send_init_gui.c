/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** send_init_gui
*/

#include "../include/server.h"
#include "../include/team.h"
#include "../include/world.h"

int send_graphic_initial_state(client_t *client, server_t *server)
{
    if (client == NULL || server == NULL || client->zn_sock == NULL ||
        server->args == NULL)
        return -1;
    send_msz(client->zn_sock, server->args->width, server->args->height);
    send_sgt(client->zn_sock, server->args->frequency);
    send_mct(client->zn_sock, server->map);
    send_tna(client->zn_sock, server->args->teams, server->args->team_count);
    send_ewn(client->zn_sock, server);
    return 0;
}
