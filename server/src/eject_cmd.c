/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** eject_cmd
*/

#include "../include/commands.h"
#include "../include/server.h"
#include "../include/world.h"
#include "../include/team.h"
#include "../include/egg.h"

client_t *find_client_by_player(server_t *server,
    player_t *target_player)
{
    client_t *client;
    player_t *player;

    for (int i = 0; i < server->connection->client_count; i++) {
        client = server->connection->clients[i];
        if (!client || client->type != CLIENT_IA || !client->team_name)
            continue;
        player = find_player_by_client(server->connection, client);
        if (player == target_player)
            return client;
    }
    return NULL;
}

static void move_player_in_direction(player_t *player, int direction,
    map_t *map)
{
    int new_x = player->x;
    int new_y = player->y;

    switch (direction) {
        case NORTH:
            new_y = (new_y - 1 + (int)map->height) % (int)map->height;
            break;
        case EAST:
            new_x = (new_x + 1) % (int)map->width;
            break;
        case SOUTH:
            new_y = (new_y + 1) % (int)map->height;
            break;
        case WEST:
            new_x = (new_x - 1 + (int)map->width) % (int)map->width;
            break;
    }
    move_player(player, new_x, new_y, map);
}

static int get_dir(player_t *ejected_player,
    player_t *ejector_player)
{
    int dx = ejected_player->x - ejector_player->x;
    int dy = ejected_player->y - ejector_player->y;

    if (dx == 0 && dy == 0)
        return 0;
    if (dy > 0)
        return 3;
    if (dy < 0)
        return 7;
    if (dx > 0)
        return 1;
    if (dx < 0)
        return 5;
    return 0;
}

size_t get_nb_player(tile_t *tile, player_t *player)
{
    size_t count = 0;

    for (size_t i = 0; i < tile->player_count; i++) {
        if (tile->players[i] != player)
            count++;
    }
    return count;
}

static void ejection(player_t *player, server_t *server,
    player_t **players_to_eject, tile_t *current_tile)
{
    char msg[64];
    tile_t *new_tile;
    player_t *ejected;
    client_t *ejected_client;
    int direction;

    for (size_t i = 0; i < get_nb_player(current_tile, player); i++) {
        ejected = players_to_eject[i];
        ejected_client = find_client_by_player(server, ejected);
        direction = get_dir(player, ejected);
        remove_player_from_tile(current_tile, ejected);
        move_player_in_direction(ejected, player->orientation, server->map);
        new_tile = get_tile(server->map, ejected->x, ejected->y);
        if (new_tile)
            add_player_to_tile(new_tile, ejected);
        send_ppo(server, ejected);
        if (ejected_client) {
            snprintf(msg, sizeof(msg), "eject: %d", direction);
            zn_send_message(ejected_client->zn_sock, msg);
        }
    }
}

int make_player_array(tile_t *current_tile, player_t *player, server_t *server,
    client_t *ejecting_client)
{
    size_t idx = 0;
    player_t **players_to_eject;
    size_t count = get_nb_player(current_tile, player);

    players_to_eject = malloc(sizeof(player_t *) * count);
    if (!players_to_eject)
        return -1;
    for (size_t i = 0; i < current_tile->player_count; i++) {
        if (current_tile->players[i] != player) {
            players_to_eject[idx] = current_tile->players[i];
            idx++;
        }
    }
    ejection(player, server, players_to_eject, current_tile);
    destroy_eggs_at_position(player->x, player->y, server);
    zn_send_message(ejecting_client->zn_sock, "ok");
    send_pex(server, player);
    free(players_to_eject);
    return 0;
}
