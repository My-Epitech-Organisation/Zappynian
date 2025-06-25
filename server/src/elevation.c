/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** elevation
*/

#include "../include/elevation.h"
#include "../include/server.h"
#include "../include/world.h"
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

void apply_elevation(tile_t *tile, int player_level,
    const elevation_requirement_t *requirements, server_t *server)
{
    if (!complete_incantation(tile, player_level, requirements, server))
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
    if (player_level < 1 || player_level >= MAX_LEVEL)
        return;
    for (size_t i = 0; i < tile->player_count; i++) {
        tile->players[i]->in_elevation = false;
    }
}

void complete_incantation_ritual(player_t *player, server_t *server)
{
    tile_t *current_tile;
    client_t *player_client;
    elevation_requirement_t requirements[MAX_LEVEL];

    player_client = find_client_by_player(server, player);
    if (!player_client)
        return;
    current_tile = get_tile(server->map, player->x, player->y);
    if (!current_tile) {
        zn_send_message(player_client->zn_sock, "ko");
        cancel_incantation(current_tile, player->level);
        return;
    }
    elevation_init_requirements(requirements);
    check_and_send_elevation_status(server, player, current_tile,
        requirements);
}
