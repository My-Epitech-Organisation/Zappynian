/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** Action command implementations
*/

#include "../include/commands.h"
#include "../include/server.h"
#include "../include/world.h"
#include "../include/team.h"
#include "../include/egg.h"

void cmd_eject(player_t *player, server_t *server)
{
    tile_t *current_tile;
    size_t players_to_eject_count = 0;
    client_t *ejecting_client;

    ejecting_client = find_client_by_player(server, player);
    if (!ejecting_client)
        return;
    if (player->dead || player->in_elevation)
        return (void)zn_send_message(ejecting_client->zn_sock, "ko");
    current_tile = get_tile(server->map, player->x, player->y);
    if (!current_tile)
        return (void)zn_send_message(ejecting_client->zn_sock, "ko");
    players_to_eject_count = get_nb_player(current_tile, player);
    if (players_to_eject_count == 0) {
        destroy_eggs_at_position(player->x, player->y, server);
        return (void)zn_send_message(ejecting_client->zn_sock, "ok");
    }
    if (make_player_array(current_tile, player, server, ejecting_client) == -1)
        return (void)zn_send_message(ejecting_client->zn_sock, "ko");
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
    if (player->resources[resource_type] > 0) {
        if (set_resource_on_tile(current_tile, resource_type)) {
            player->resources[resource_type]--;
            zn_send_message(server->connection->zn_server, "ok");
        } else
            zn_send_message(server->connection->zn_server, "ko");
    } else
        zn_send_message(server->connection->zn_server, "ko");
}

void cmd_incantation(player_t *player, server_t *server)
{
    (void) server;
    printf("Player %d executed incantation command\n", player->id);
}
