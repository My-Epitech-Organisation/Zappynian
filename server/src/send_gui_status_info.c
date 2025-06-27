/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** send_gui_status_info
*/

#include "../include/server.h"
#include "../include/death.h"

void send_pdi(server_t *server, player_t *player)
{
    char command[128];
    int ret;

    if (server->graphic_clients == NULL || player == NULL)
        return;
    ret = snprintf(command, sizeof(command), "pdi #%d", player->id);
    if (ret < 0 || (size_t)ret >= sizeof(command))
        return;
    send_to_all_graphic_clients(server->graphic_clients, command);
}

void send_seg(server_t *server, const char *team_name)
{
    char command[128];
    int ret;

    if (server->graphic_clients == NULL)
        return;
    ret = snprintf(command, sizeof(command), "seg %s", team_name);
    if (ret < 0 || (size_t)ret >= sizeof(command))
        return;
    send_to_all_graphic_clients(server->graphic_clients, command);
}

void send_smg(server_t *server, const char *message)
{
    char command[256];
    int ret;

    if (server->graphic_clients == NULL || message == NULL)
        return;
    ret = snprintf(command, sizeof(command), "smg %s", message);
    if (ret < 0 || (size_t)ret >= sizeof(command))
        return;
    send_to_all_graphic_clients(server->graphic_clients, command);
}

void send_suc(server_t *server)
{
    char command[256];
    int ret;

    if (server->graphic_clients == NULL)
        return;
    ret = snprintf(command, sizeof(command), "suc");
    if (ret < 0 || (size_t)ret >= sizeof(command))
        return;
    send_to_all_graphic_clients(server->graphic_clients, command);
}

void send_sbp(server_t *server)
{
    char command[128];
    int ret;

    if (server->graphic_clients == NULL)
        return;
    ret = snprintf(command, sizeof(command), "sbp");
    if (ret < 0 || (size_t)ret >= sizeof(command))
        return;
    send_to_all_graphic_clients(server->graphic_clients, command);
}
