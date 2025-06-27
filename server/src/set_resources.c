/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** set_resources
*/

#include "../include/commands.h"
#include "../include/server.h"

void manage_food(server_t *server, player_t *player, client_t *client,
    int resource_type)
{
    tile_t *current_tile;

    current_tile = get_tile(server->map, player->x, player->y);
    if (player->food > 0) {
        if (set_resource_on_tile(current_tile, resource_type)) {
            player->food--;
            player->resources[resource_type]--;
            send_pdr(server, player->id, resource_type);
            zn_send_message(client->zn_sock, "ok");
        } else
            zn_send_message(client->zn_sock, "ko");
    } else
        zn_send_message(client->zn_sock, "ko");
}

void manage_stones(server_t *server, player_t *player, client_t *client,
    int resource_type)
{
    tile_t *current_tile;

    current_tile = get_tile(server->map, player->x, player->y);
    if (player->resources[resource_type] > 0) {
        if (set_resource_on_tile(current_tile, resource_type)) {
            player->resources[resource_type]--;
            send_pdr(server, player->id, resource_type);
            zn_send_message(client->zn_sock, "ok");
        } else
            zn_send_message(client->zn_sock, "ko");
    } else
        zn_send_message(client->zn_sock, "ko");
}

void add_resources(player_t *player, int resource_type, server_t *server,
    client_t *client)
{
    if (resource_type == 0)
        player->food++;
    player->resources[resource_type]++;
    send_pgt(server, player->id, resource_type);
    zn_send_message(client->zn_sock, "ok");
}
