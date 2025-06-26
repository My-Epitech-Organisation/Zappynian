/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** send_gui_time_info
*/

#include "../include/server.h"

void send_sgt(zn_socket_t sock, int frequency)
{
    char sgt_command[256];
    int ret;

    if (sock == NULL || frequency <= 0)
        return;
    ret = snprintf(sgt_command, sizeof(sgt_command), "sgt %d", frequency);
    if (ret < 0 || (size_t)ret >= sizeof(sgt_command))
        return;
    zn_send_message(sock, sgt_command);
}

void send_sst(zn_socket_t sock, int time)
{
    char sst_command[256];
    int ret;

    if (sock == NULL || time < 0)
        return;
    ret = snprintf(sst_command, sizeof(sst_command), "sst %d", time);
    if (ret < 0 || (size_t)ret >= sizeof(sst_command))
        return;
    zn_send_message(sock, sst_command);
}
