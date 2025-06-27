/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** send_gui_time_info
*/

#include "../include/server.h"

void send_sgt(server_t *server, int frequency)
{
    char sgt_command[256];
    int ret;

    if (server->graphic_clients == NULL || frequency <= 0)
        return;
    ret = snprintf(sgt_command, sizeof(sgt_command), "sgt %d", frequency);
    if (ret < 0 || (size_t)ret >= sizeof(sgt_command))
        return;
    send_to_all_graphic_clients(server->graphic_clients, sgt_command);
}

void send_sst(server_t *server, int time)
{
    char sst_command[256];
    int ret;

    if (server->graphic_clients == NULL || time < 0)
        return;
    ret = snprintf(sst_command, sizeof(sst_command), "sst %d", time);
    if (ret < 0 || (size_t)ret >= sizeof(sst_command))
        return;
    send_to_all_graphic_clients(server->graphic_clients, sst_command);
}
