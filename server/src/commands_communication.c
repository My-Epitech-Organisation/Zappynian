/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** Communication command implementations
*/

#include <stdio.h>
#include "../include/commands.h"
#include "../include/server.h"

void cmd_inventory(player_t *player, server_t *server)
{
    char *inventory_result = NULL;

    if (player->dead || player->in_elevation) {
        zn_send_message(server->connection->zn_server, "ko");
        return;
    }
    inventory_result = get_player_inventory(player);
    if (inventory_result != NULL) {
        zn_send_message(server->connection->zn_server, inventory_result);
        free(inventory_result);
    } else
        zn_send_message(server->connection->zn_server, "ko");
}

void cmd_broadcast(player_t *player, server_t *server)
{
    char *broadcast_message = NULL;

    if (player->dead || player->in_elevation) {
        zn_send_message(server->connection->zn_server, "ko");
        return;
    }
    broadcast_message = get_broadcast_message(player);
    if (broadcast_message == NULL || strlen(broadcast_message) == 0) {
        zn_send_message(server->connection->zn_server, "ko");
        return;
    }
    broadcast_to_all_players(player, server, broadcast_message);
    free(broadcast_message);
    zn_send_message(server->connection->zn_server, "ok");
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
