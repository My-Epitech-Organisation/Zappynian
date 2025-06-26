/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** send_gui_status_info
*/

#include "../include/server.h"
#include "../include/death.h"

void send_pdi(zn_socket_t sock, player_t *player)
{
    char command[128];
    int ret;

    if (sock == NULL || player == NULL)
        return;
    ret = snprintf(command, sizeof(command), "pdi #%d", player->id);
    if (ret < 0 || (size_t)ret >= sizeof(command))
        return;
    zn_send_message(sock, command);
}

void send_seg(zn_socket_t sock, const char *team_name)
{
    char command[128];
    int ret;

    if (sock == NULL)
        return;
    ret = snprintf(command, sizeof(command), "seg %s", team_name);
    if (ret < 0 || (size_t)ret >= sizeof(command))
        return;
    zn_send_message(sock, command);
}

void send_smg(zn_socket_t sock, const char *message)
{
    char command[256];
    int ret;

    if (sock == NULL || message == NULL)
        return;
    ret = snprintf(command, sizeof(command), "smt %s", message);
    if (ret < 0 || (size_t)ret >= sizeof(command))
        return;
    zn_send_message(sock, command);
}

void send_suc(zn_socket_t sock)
{
    char command[256];
    int ret;

    if (sock == NULL)
        return;
    ret = snprintf(command, sizeof(command), "suc");
    if (ret < 0 || (size_t)ret >= sizeof(command))
        return;
    zn_send_message(sock, command);
}

void send_sbp(zn_socket_t sock)
{
    char command[128];
    int ret;

    if (sock == NULL)
        return;
    ret = snprintf(command, sizeof(command), "sbp");
    if (ret < 0 || (size_t)ret >= sizeof(command))
        return;
    zn_send_message(sock, command);
}
