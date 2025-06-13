/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** Communication command implementations
*/

#include <stdio.h>
#include "../includes/commands.h"

void cmd_inventory(player_t *player, server_t *server)
{
    (void) server;
    printf("Player %d executed inventory command\n", player->id);
}

void cmd_broadcast(player_t *player, server_t *server)
{
    (void) server;
    printf("Player %d executed broadcast command\n", player->id);
}

void cmd_connect_nbr(player_t *player, server_t *server)
{
    (void) server;
    printf("Player %d executed connect_nbr command\n", player->id);
}

void cmd_fork(player_t *player, server_t *server)
{
    (void) server;
    printf("Player %d executed fork command\n", player->id);
}
