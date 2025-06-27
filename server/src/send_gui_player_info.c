/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** send_gui_player_info
*/

#include "../include/server.h"
#include "../include/player.h"

void send_pnw(server_t *server, player_t *player)
{
    char pnw_command[256];
    int ret;

    if (server->graphic_clients == NULL || player == NULL)
        return;
    ret = snprintf(pnw_command, sizeof(pnw_command),
        "pnw #%d %d %d %d %d %s",
        player->id, player->x, player->y, player->orientation,
        player->level, player->team_name);
    if (ret < 0 || (size_t)ret >= sizeof(pnw_command))
        return;
    send_to_all_graphic_clients(server->graphic_clients, pnw_command);
}

void send_ppo(server_t *server, player_t *player)
{
    char ppo_command[256];
    int ret;

    if (server->graphic_clients == NULL || player == NULL)
        return;
    ret = snprintf(ppo_command, sizeof(ppo_command),
        "ppo #%d %d %d %d",
        player->id, player->x, player->y, player->orientation);
    if (ret < 0 || (size_t)ret >= sizeof(ppo_command))
        return;
    send_to_all_graphic_clients(server->graphic_clients, ppo_command);
}

void send_plv(server_t *server, player_t *player)
{
    char plv_command[256];
    int ret;

    if (server->graphic_clients == NULL || player == NULL)
        return;
    ret = snprintf(plv_command, sizeof(plv_command),
        "plv #%d %d", player->id, player->level);
    if (ret < 0 || (size_t)ret >= sizeof(plv_command))
        return;
    send_to_all_graphic_clients(server->graphic_clients, plv_command);
}

void send_pin(server_t *server, player_t *player)
{
    char pin_command[256];
    int ret;

    if (server->graphic_clients == NULL || player == NULL)
        return;
    ret = snprintf(pin_command, sizeof(pin_command),
        "pin #%d %d %d %d %d %d %d %d %d %d",
        player->id, player->x, player->y, player->resources[0],
        player->resources[1], player->resources[2], player->resources[3],
        player->resources[4], player->resources[5], player->resources[6]);
    if (ret < 0 || (size_t)ret >= sizeof(pin_command))
        return;
    send_to_all_graphic_clients(server->graphic_clients, pin_command);
}

void send_pex(server_t *server, player_t *player)
{
    char pex_command[256];
    int ret;

    if (server->graphic_clients == NULL || player == NULL)
        return;
    ret = snprintf(pex_command, sizeof(pex_command),
        "pex #%d", player->id);
    if (ret < 0 || (size_t)ret >= sizeof(pex_command))
        return;
    send_to_all_graphic_clients(server->graphic_clients, pex_command);
}
