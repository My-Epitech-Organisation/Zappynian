/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** send_gui_egg_info
*/

#include "../include/server.h"
#include "../include/egg.h"

static void send_egg_recursively(graphic_client_list_t *list, egg_t *egg)
{
    char ewn_command[256];
    int ret;

    if (egg == NULL)
        return;
    send_egg_recursively(list, egg->next);
    ret = snprintf(ewn_command, sizeof(ewn_command),
        "ewn #%d #%d %d %d", egg->id, egg->player_id,
        egg->x, egg->y);
    if (ret < 0 || (size_t)ret >= sizeof(ewn_command))
        return;
    send_to_all_graphic_clients(list, ewn_command);
}

void send_enw(server_t *server)
{
    if (server->graphic_clients == NULL || server == NULL)
        return;
    send_egg_recursively(server->graphic_clients, server->eggs);
}

void send_pfk(server_t *server, int player_id)
{
    char pfk_command[64];
    int ret;

    if (server->graphic_clients == NULL)
        return;
    ret = snprintf(pfk_command, sizeof(pfk_command), "pfk #%d", player_id);
    if (ret < 0 || (size_t)ret >= sizeof(pfk_command))
        return;
    send_to_all_graphic_clients(server->graphic_clients, pfk_command);
}

void send_ebo(server_t *server, int egg_id)
{
    char ebo_command[64];
    int ret;

    if (server->graphic_clients == NULL)
        return;
    ret = snprintf(ebo_command, sizeof(ebo_command), "ebo #%d", egg_id);
    if (ret < 0 || (size_t)ret >= sizeof(ebo_command))
        return;
    send_to_all_graphic_clients(server->graphic_clients, ebo_command);
}

void send_edi(server_t *server, int egg_id)
{
    char edi_command[64];
    int ret;

    if (server->graphic_clients == NULL)
        return;
    ret = snprintf(edi_command, sizeof(edi_command), "edi #%d", egg_id);
    if (ret < 0 || (size_t)ret >= sizeof(edi_command))
        return;
    send_to_all_graphic_clients(server->graphic_clients, edi_command);
}
