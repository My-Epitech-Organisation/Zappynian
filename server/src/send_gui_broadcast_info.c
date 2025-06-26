/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** send_gui_broadcast_info
*/

#include "../include/server.h"
#include "../include/player.h"

void send_pbc(zn_socket_t sock, player_t *player, const char *message)
{
    char pbc_command[256];
    int ret;

    if (sock == NULL || player == NULL || message == NULL)
        return;
    ret = snprintf(pbc_command, sizeof(pbc_command),
        "pbc #%d %s", player->id, message);
    if (ret < 0 || (size_t)ret >= sizeof(pbc_command))
        return;
    zn_send_message(sock, pbc_command);
}
