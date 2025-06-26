/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** send_gui_resources_info
*/

#include "../include/server.h"
#include "../include/resource.h"

void send_pdr(zn_socket_t sock, int player_id, int resource_type)
{
    char pdr_command[64];
    int ret;

    if (sock == NULL)
        return;
    ret = snprintf(pdr_command, sizeof(pdr_command), "pdr #%d %d",
        player_id, resource_type);
    if (ret < 0 || (size_t)ret >= sizeof(pdr_command))
        return;
    zn_send_message(sock, pdr_command);
}

void send_pgt(zn_socket_t sock, int player_id, int resource_type)
{
    char pgt_command[64];
    int ret;

    if (sock == NULL)
        return;
    ret = snprintf(pgt_command, sizeof(pgt_command), "pgt #%d %d",
        player_id, resource_type);
    if (ret < 0 || (size_t)ret >= sizeof(pgt_command))
        return;
    zn_send_message(sock, pgt_command);
}
