/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** Movement command implementations
*/

#include <stdio.h>
#include "../includes/commands.h"
#include "../includes/server.h"

void cmd_forward(player_t *player, server_t *server)
{
    if (player->dead || player->in_elevation)
        return;
    switch (player->orientation) {
        case NORTH:
            move_player(player, player->x, player->y - 1, server->map);
            break;
        case EAST:
            move_player(player, player->x + 1, player->y, server->map);
            break;
        case SOUTH:
            move_player(player, player->x, player->y + 1, server->map);
            break;
        case WEST:
            move_player(player, player->x - 1, player->y, server->map);
            break;
    }
}

void cmd_right(player_t *player, server_t *server)
{
    (void) server;
    if (player->dead || player->in_elevation)
        return;
    player->orientation = (player->orientation + 1) % 4;
}

void cmd_left(player_t *player, server_t *server)
{
    (void) server;
    if (player->dead || player->in_elevation)
        return;
    player->orientation = (player->orientation + 3) % 4;
}

void cmd_look(player_t *player, server_t *server)
{
    (void) server;
    if (player->dead || player->in_elevation)
        return;
    printf("Player %d is looking around\n", player->id);
}
