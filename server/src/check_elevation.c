/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** check_elevation
*/

#include "../include/elevation.h"
#include "../include/server.h"

size_t count_players_with_level(tile_t *tile, player_t *player)
{
    size_t count = 0;

    for (size_t i = 0; i < tile->player_count; i++) {
        if (tile->players[i] && !tile->players[i]->dead &&
            tile->players[i]->level == player->level) {
            count++;
        }
    }
    return count;
}

bool can_start_incantation(tile_t *tile, player_t *player,
    const elevation_requirement_t *requirements)
{
    elevation_requirement_t req;
    size_t same_level_players = 0;

    if (!tile || !tile->players || tile->player_count == 0 || !player ||
        player->dead || player->level < 1 || player->level > MAX_LEVEL)
        return false;
    req = requirements[player->level - 1];
    same_level_players = count_players_with_level(tile, player);
    if (same_level_players < requirements[player->level - 1].required_players)
        return false;
    for (int i = 0; i < NB_RESOURCE_TYPES; i++) {
        if (tile->resources[i] < req.required_resources[i]) {
            return false;
        }
    }
    return true;
}

void check_and_send_elevation_status(server_t *server, player_t *player,
    tile_t *current_tile, const elevation_requirement_t *requirements)
{
    char msg[256];

    send_pic(server, current_tile, player);
    if (can_start_incantation(current_tile, player, requirements)) {
        apply_elevation(current_tile, player->level, requirements);
        send_pie(server, current_tile, player);
        snprintf(msg, sizeof(msg), "Current level: %d", player->level);
        for (size_t i = 0; i < current_tile->player_count; i++)
            send_stat_to_all_players(server, current_tile, i, msg);
    } else {
        cancel_incantation(current_tile, player->level);
        zn_send_message(server->connection->zn_server, "ko");
    }
}
