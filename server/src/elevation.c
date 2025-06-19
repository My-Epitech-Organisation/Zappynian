/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** elevation
*/

#include "../include/elevation.h"
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

static bool can_start_incantation(tile_t *tile, int player_level,
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

void start_incantation(tile_t *tile, int player_level,
    const elevation_requirement_t *requirements)
{
    if (!can_start_incantation(tile, player_level, requirements)) {
        fprintf(stderr, "Cannot start incantation: requirements not met.\n");
        return;
    }
    for (size_t i = 0; i < tile->player_count; i++) {
        tile->players[i]->in_elevation = true;
    }
}

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

void apply_elevation(tile_t *tile, int player_level,
    const elevation_requirement_t *requirements)
{
    if (!complete_incantation(tile, player_level, requirements))
        return;
    for (size_t i = 0; i < tile->player_count; i++) {
        tile->players[i]->level++;
        tile->players[i]->in_elevation = false;
    }
    for (int i = 0; i < NB_RESOURCE_TYPES; i++) {
        tile->resources[i] -=
            requirements[player_level - 1].required_resources[i];
    }
}

void cancel_incantation(tile_t *tile, int player_level)
{
    if (player_level < 1 || player_level >= MAX_LEVEL) {
        return;
    }
    for (size_t i = 0; i < tile->player_count; i++) {
        tile->players[i]->in_elevation = false;
    }
}
