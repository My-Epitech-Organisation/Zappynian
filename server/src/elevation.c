/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** elevation
*/

#include "../include/elevation.h"
#include "../include/world.h"
#include "../include/server.h"
#include <stdio.h>

void elevation_init_requirements(elevation_requirement_t *requirements)
{
    memset(requirements, 0, sizeof(elevation_requirement_t) * MAX_LEVEL);
    requirements[0] = (elevation_requirement_t) {1, {1, 0, 0, 0, 0, 0, 0}};
    requirements[1] = (elevation_requirement_t) {2, {1, 1, 1, 0, 0, 0, 0}};
    requirements[2] = (elevation_requirement_t) {2, {2, 0, 1, 0, 2, 0, 0}};
    requirements[3] = (elevation_requirement_t) {4, {1, 1, 2, 0, 1, 0, 0}};
    requirements[4] = (elevation_requirement_t) {4, {1, 2, 1, 3, 0, 0, 0}};
    requirements[5] = (elevation_requirement_t) {6, {1, 2, 3, 0, 1, 0, 0}};
    requirements[6] = (elevation_requirement_t) {6, {2, 2, 2, 2, 2, 1, 0}};
}

bool can_start_incantation(tile_t *tile, player_t *player,
    elevation_requirement_t requirements[])
{
    elevation_requirement_t req;
    int same_level_players = 0;

    if (!tile || !player || player->level >= MAX_LEVEL)
        return false;
    req = requirements[player->level - 1];
    same_level_players = 0;
    for (size_t i = 0; i < tile->player_count; i++) {
        if (tile->players[i] && tile->players[i]->level == player->level)
            same_level_players++;
    }
    if (same_level_players < req.required_players)
        return false;
    for (int i = 0; i < RESOURCE_COUNT; i++) {
        if (tile->resources[i] < req.required_resources[i])
            return false;
    }
    return true;
}

void start_incantation(tile_t *tile)
{
    if (!tile)
        return;
    tile->is_incantation_in_progress = true;
    for (size_t i = 0; i < tile->player_count; i++) {
        if (tile->players[i]) {
            tile->players[i]->in_elevation = true;
        }
    }
}

static void reset_incantation_state(tile_t *tile)
{
    size_t i;

    tile->is_incantation_in_progress = false;
    for (i = 0; i < tile->player_count; i++) {
        if (tile->players[i])
            tile->players[i]->in_elevation = false;
    }
}

static void consume_resources(tile_t *tile, elevation_requirement_t req)
{
    int i;

    for (i = 0; i < RESOURCE_COUNT; i++) {
        tile->resources[i] -= req.required_resources[i];
        if (tile->resources[i] < 0)
            tile->resources[i] = 0;
    }
}

static void elevate_players(tile_t *tile, player_t *player, server_t *server)
{
    size_t i;

    for (i = 0; i < tile->player_count; i++) {
        if (tile->players[i] && tile->players[i]->level == player->level) {
            tile->players[i]->level++;
            tile->players[i]->in_elevation = false;
            send_plv(server, tile->players[i]);
        }
    }
}

bool complete_incantation(tile_t *tile, player_t *player,
    elevation_requirement_t requirements[], server_t *server)
{
    elevation_requirement_t req;

    if (!tile || !player || !tile->is_incantation_in_progress)
        return false;
    req = requirements[player->level - 1];
    if (!can_start_incantation(tile, player, requirements)) {
        reset_incantation_state(tile);
        return false;
    }
    consume_resources(tile, req);
    elevate_players(tile, player, server);
    tile->is_incantation_in_progress = false;
    send_bct(server, tile);
    return true;
}
