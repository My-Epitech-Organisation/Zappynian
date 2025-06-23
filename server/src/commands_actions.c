/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** Action command implementations
*/

#include "../include/commands.h"
#include "../include/server.h"
#include "../include/world.h"

void cmd_eject(player_t *player, server_t *server)
{
    (void) server;
    printf("Player %d executed eject command\n", player->id);
}

void cmd_take(player_t *player, server_t *server)
{
    int resource_type;
    tile_t *current_tile;

    if (player->dead || player->in_elevation)
        return (void)zn_send_message(server->connection->zn_server, "ko");
    resource_type = get_resource_type_from_name(get_command_argument(player));
    if (resource_type == -1) {
        zn_send_message(server->connection->zn_server, "ko");
        return;
    }
    current_tile = get_tile(server->map, player->x, player->y);
    if (take_resource_from_tile(current_tile, resource_type)) {
        if (resource_type == 0)
            player->food++;
        else
            player->resources[resource_type]++;
        zn_send_message(server->connection->zn_server, "ok");
    } else
        zn_send_message(server->connection->zn_server, "ko");
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
