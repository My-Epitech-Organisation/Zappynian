/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** send_gui_egg_info
*/

#include "../include/server.h"
#include "../include/egg.h"

void send_ewn(zn_socket_t sock, server_t *server)
{
    char ewn_command[256];
    int ret;

    if (sock == NULL || server == NULL)
        return;
    for (egg_t *egg = server->eggs; egg != NULL; egg = egg->next) {
        ret = snprintf(ewn_command, sizeof(ewn_command),
            "ewn #%d #%d %d %d", egg->id, egg->id,
            egg->x, egg->y);
        if (ret < 0 || (size_t)ret >= sizeof(ewn_command))
            return;
        zn_send_message(sock, ewn_command);
    }
}
