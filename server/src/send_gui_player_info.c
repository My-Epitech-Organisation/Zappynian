/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** send_gui_player_info
*/

#include "../include/server.h"
#include "../include/player.h"

void send_pnw(zn_socket_t sock, player_t *player)
{
    char pnw_command[256];
    int ret;

    if (sock == NULL || player == NULL)
        return;
    ret = snprintf(pnw_command, sizeof(pnw_command),
        "pnw #%d %d %d %d %d %s",
        player->id, player->x, player->y, player->orientation,
        player->level, player->team_name);
    if (ret < 0 || (size_t)ret >= sizeof(pnw_command))
        return;
    zn_send_message(sock, pnw_command);
}

void send_ppo(zn_socket_t sock, player_t *player)
{
    char ppo_command[256];
    int ret;

    if (sock == NULL || player == NULL)
        return;
    ret = snprintf(ppo_command, sizeof(ppo_command),
        "ppo #%d %d %d %d",
        player->id, player->x, player->y, player->orientation);
    if (ret < 0 || (size_t)ret >= sizeof(ppo_command))
        return;
    zn_send_message(sock, ppo_command);
}

void send_plv(zn_socket_t sock, player_t *player)
{
    char plv_command[256];
    int ret;

    if (sock == NULL || player == NULL)
        return;
    ret = snprintf(plv_command, sizeof(plv_command),
        "plv #%d %d", player->id, player->level);
    if (ret < 0 || (size_t)ret >= sizeof(plv_command))
        return;
    zn_send_message(sock, plv_command);
}

void send_pin(zn_socket_t sock, player_t *player)
{
    char pin_command[256];
    int ret;

    if (sock == NULL || player == NULL)
        return;
    ret = snprintf(pin_command, sizeof(pin_command),
        "pin #%d %d %d %d %d %d %d %d %d %d %d %d",
        player->id, player->x, player->y, player->level,
        player->orientation, player->food,
        player->resources[0], player->resources[1],
        player->resources[2], player->resources[3],
        player->resources[4], player->resources[5]);
    if (ret < 0 || (size_t)ret >= sizeof(pin_command))
        return;
    zn_send_message(sock, pin_command);
}
