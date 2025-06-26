/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** send_gui_egg_info
*/

#include "../include/server.h"
#include "../include/egg.h"

static void send_egg_recursively(zn_socket_t sock, egg_t *egg)
{
    char ewn_command[256];
    int ret;

    if (egg == NULL)
        return;
    send_egg_recursively(sock, egg->next);
    ret = snprintf(ewn_command, sizeof(ewn_command),
        "ewn #%d #%d %d %d", egg->id, egg->player_id,
        egg->x, egg->y);
    if (ret < 0 || (size_t)ret >= sizeof(ewn_command))
        return;
    zn_send_message(sock, ewn_command);
}

void send_ewn(zn_socket_t sock, server_t *server)
{
    if (sock == NULL || server == NULL)
        return;
    if (server->eggs == NULL) {
        zn_send_message(sock, "ewn");
        return;
    }
    send_egg_recursively(sock, server->eggs);
}

void send_pfk(zn_socket_t sock, int player_id)
{
    char pfk_command[64];
    int ret;

    if (sock == NULL)
        return;
    ret = snprintf(pfk_command, sizeof(pfk_command), "pfk #%d", player_id);
    if (ret < 0 || (size_t)ret >= sizeof(pfk_command))
        return;
    zn_send_message(sock, pfk_command);
}
