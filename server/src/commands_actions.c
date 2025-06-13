/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** Action command implementations
*/

#include <stdio.h>
#include "../includes/commands.h"

void cmd_eject(player_t *player, server_t *server)
{
    (void) server;
    printf("Player %d executed eject command\n", player->id);
}

void cmd_take(player_t *player, server_t *server)
{
    (void) server;
    printf("Player %d executed take command\n", player->id);
}

void cmd_set(player_t *player, server_t *server)
{
    (void) server;
    printf("Player %d executed set command\n", player->id);
}

void cmd_incantation(player_t *player, server_t *server)
{
    (void) server;
    printf("Player %d executed incantation command\n", player->id);
}
