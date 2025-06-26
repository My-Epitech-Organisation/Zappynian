/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** send_init_gui
*/

#include "../include/server.h"
#include "../include/team.h"
#include "../include/world.h"

int send_graphic_initial_state(client_t *client, server_args_t *args)
{
    if (client == NULL || args == NULL || client->zn_sock == NULL)
        return -1;
    send_msz(client->zn_sock, args->width, args->height);
    // send_sgt(client->zn_sock, args->frequency);
    // send_mct(client->zn_sock, args->map);
    // send_tna(client->zn_sock, args->teams, args->team_count);
    // send_ewn(client->zn_sock, args->server);
    return 0;
}
