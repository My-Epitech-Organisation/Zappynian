/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** check_elevation
*/

#include "../include/elevation.h"
#include "../include/server.h"

bool complete_incantation(tile_t *tile, int player_level,
    const elevation_requirement_t *requirements)
{
    elevation_requirement_t req;

    if (player_level < 1 || player_level > MAX_LEVEL) {
        return false;
    }
    req = requirements[player_level - 1];
    if (tile->player_count < req.required_players) {
        return false;
    }
    for (int i = 0; i < NB_RESOURCE_TYPES; i++) {
        if (tile->resources[i] < req.required_resources[i]) {
            return false;
        }
    }
    return true;
}

bool can_start_incantation(tile_t *tile, int player_level,
    const elevation_requirement_t *requirements)
{
    elevation_requirement_t req;

    if (player_level < 1 || player_level > MAX_LEVEL) {
        return false;
    }
    req = requirements[player_level - 1];
    if (tile->player_count < req.required_players) {
        return false;
    }
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
    char response[256];

    if (complete_incantation(current_tile, player->level, requirements)) {
        apply_elevation(current_tile, player->level, requirements);
        snprintf(response, sizeof(response),
            "Elevation underway Current level: %d", player->level);
        for (size_t i = 0; i < current_tile->player_count; i++) {
            send_stat_to_all_players(server, current_tile, i, response);
            send_ok_to_all_players(server, current_tile, i);
        }
    } else {
        cancel_incantation(current_tile, player->level);
        for (size_t i = 0; i < current_tile->player_count; i++)
            send_ko_to_all_players(server, current_tile, i);
    }
}

size_t count_players_with_level(tile_t *tile, player_t *player)
{
    size_t count = 0;

    for (size_t i = 0; i < tile->player_count; i++) {
        if (tile->players[i] &&
            tile->players[i]->level == player->level) {
            count++;
        }
    }
    return count;
}

int check_good_nb_players_and_resources(tile_t *current_tile,
    elevation_requirement_t *req, player_t *player)
{
    size_t same_level_players = 0;

    elevation_init_requirements(req);
    same_level_players = count_players_with_level(current_tile, player);
    if (same_level_players < req[player->level - 1].required_players)
        return -1;
    for (int i = 0; i < NB_RESOURCE_TYPES; i++) {
        if (current_tile->resources[i] <
            req[player->level - 1].required_resources[i])
            return -1;
    }
    return 0;
}
