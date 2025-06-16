/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** Movement command implementations
*/

#include <stdio.h>
#include "../include/commands.h"
#include "../include/server.h"
#include "../../libzappy_net/include/zappy_net.h"

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
    printf("Forward\n");
}

void cmd_right(player_t *player, server_t *server)
{
    if (player->dead || player->in_elevation)
        return;
    player->orientation = (player->orientation + 1) % 4;
    zn_send_message(server->connection->fd, "Right");
}

void cmd_left(player_t *player, server_t *server)
{
    (void) server;
    if (player->dead || player->in_elevation)
        return;
    player->orientation = (player->orientation + 3) % 4;
    zn_send_message(server->connection->fd, "Left");
}

void cmd_look(player_t *player, server_t *server)
{
    char *look_result = NULL;

    if (player->dead || player->in_elevation)
        return;
    look_result = get_player_vision(player, server->map);
    if (look_result != NULL) {
        zn_send_message(server->connection->fd, look_result);
        free(look_result);
    }
}
