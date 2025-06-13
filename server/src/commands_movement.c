/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** Movement command implementations
*/

#include <stdio.h>
#include "../includes/commands.h"

void cmd_forward(player_t *player)
{
    if (player->dead || player->in_elevation)
        return;
    switch (player->orientation) {
        case NORTH:
            move_player(player, player->x, player->y - 1, NULL);
            break;
        case EAST:
            move_player(player, player->x + 1, player->y, NULL);
            break;
        case SOUTH:
            move_player(player, player->x, player->y + 1, NULL);
            break;
        case WEST:
            move_player(player, player->x - 1, player->y, NULL);
            break;
    }
}

void cmd_right(player_t *player)
{
    if (player->dead || player->in_elevation)
        return;
    player->orientation = (player->orientation + 1) % 4;
}

void cmd_left(player_t *player)
{
    if (player->dead || player->in_elevation)
        return;
    player->orientation = (player->orientation + 3) % 4;
}

void cmd_look(player_t *player)
{
    if (player->dead || player->in_elevation)
        return;
    printf("Player %d is looking around\n", player->id);
}

void cmd_inventory(player_t *player)
{
    printf("Player %d executed inventory command\n", player->id);
}

void cmd_broadcast(player_t *player)
{
    printf("Player %d executed broadcast command\n", player->id);
}

void cmd_connect_nbr(player_t *player)
{
    printf("Player %d executed connect_nbr command\n", player->id);
}

void cmd_fork(player_t *player)
{
    printf("Player %d executed fork command\n", player->id);
}

void cmd_eject(player_t *player)
{
    printf("Player %d executed eject command\n", player->id);
}

void cmd_take(player_t *player)
{
    printf("Player %d executed take command\n", player->id);
}

void cmd_set(player_t *player)
{
    printf("Player %d executed set command\n", player->id);
}

void cmd_incantation(player_t *player)
{
    printf("Player %d executed incantation command\n", player->id);
}
