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
#include "../include/elevation.h"

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
        send_pex(server, player);
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
        send_pgt(server, player->id, resource_type);
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
            send_pdr(server, player->id, resource_type);
            zn_send_message(server->connection->zn_server, "ok");
        } else
            zn_send_message(server->connection->zn_server, "ko");
    } else
        zn_send_message(server->connection->zn_server, "ko");
}

void cmd_incantation(player_t *player, server_t *server)
{
    tile_t *current_tile;
    client_t *player_client;
    elevation_requirement_t req[MAX_LEVEL];

    if (!player || !server)
        return;
    if (player->dead || player->in_elevation)
        return (void)zn_send_message(server->connection->zn_server, "ko");
    player_client = find_client_by_player(server, player);
    if (!player_client)
        return;
    current_tile = get_tile(server->map, player->x, player->y);
    if (!current_tile)
        return (void)zn_send_message(player_client->zn_sock, "ko");
    elevation_init_requirements(req);
    if (!can_start_incantation(current_tile, player, req))
        return (void)zn_send_message(player_client->zn_sock, "ko");
    start_incantation(current_tile);
    zn_send_message(player_client->zn_sock, "Elevation underway");
    check_and_send_elevation_status(server, player, current_tile, req);
}
