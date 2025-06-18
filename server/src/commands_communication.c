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

    (void) server;
    if (player->dead || player->in_elevation)
        return;
    inventory_result = get_player_inventory(player);
    if (inventory_result != NULL) {
        zn_send_message(server->connection->zn_server, inventory_result);
        free(inventory_result);
    }
    zn_send_message(server->connection->zn_server, "Inventory");
}

void cmd_broadcast(player_t *player, server_t *server)
{
    char *broadcast_message = NULL;

    if (player->dead || player->in_elevation)
        return;
    broadcast_message = get_broadcast_message(player);
    if (broadcast_message == NULL || strlen(broadcast_message) == 0) {
        zn_send_message(server->connection->zn_server,
            "Broadcast message cannot be empty");
        return;
    }
    broadcast_to_all_players(player, server, broadcast_message);
    free(broadcast_message);
    printf("Sending 'ok' to player %d\n", player->id);
    zn_send_message(server->connection->zn_server, "Broadcast sent");
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
