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
    send_msz(server, server->args->width, server->args->height);
    send_sgt(server, server->args->frequency);
    send_mct(server, server->map);
    send_tna(server, server->args->teams, server->args->team_count);
    send_ewn(server);
    return 0;
}
