/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** send_gui_broadcast_info
*/

#include "../include/server.h"
#include "../include/player.h"

void send_pbc(server_t *server, player_t *player, const char *message)
{
    char pbc_command[256];
    int ret;

    if (server->graphic_clients == NULL || player == NULL || message == NULL)
        return;
    ret = snprintf(pbc_command, sizeof(pbc_command),
        "pbc #%d %s", player->id, message);
    if (ret < 0 || (size_t)ret >= sizeof(pbc_command))
        return;
    send_to_all_graphic_clients(server->graphic_clients, pbc_command);
}
