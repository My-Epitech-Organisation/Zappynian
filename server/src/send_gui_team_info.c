/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** send_gui_team_info
*/

#include "../include/server.h"
#include "../include/team.h"

void send_tna(server_t *server, team_t *teams, int team_count)
{
    char tna_command[256];
    int ret;

    if (server->graphic_clients == NULL || teams == NULL || team_count <= 0)
        return;
    for (int i = 0; i < team_count; i++) {
        ret = snprintf(tna_command, sizeof(tna_command),
            "tna %s", teams[i].name);
        if (ret < 0 || (size_t)ret >= sizeof(tna_command))
            return;
        send_to_all_graphic_clients(server->graphic_clients, tna_command);
    }
}
