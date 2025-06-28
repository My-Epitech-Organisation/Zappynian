/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** elevation
*/

#ifndef ELEVATION_H
    #define ELEVATION_H
    #define MAX_LEVEL 8
    #define NB_RESOURCE_TYPES 7

    #include "player.h"
    #include "world.h"
    #include <stdbool.h>
    #include <string.h>

typedef struct elevation_requirement_s {
    size_t required_players;
    int required_resources[NB_RESOURCE_TYPES];
} elevation_requirement_t;

void elevation_init_requirements(elevation_requirement_t *requirements);
void start_incantation(tile_t *tile);
void apply_elevation(tile_t *tile, int player_level, server_t *server);
void cancel_incantation(tile_t *tile, server_t *server);
bool can_start_incantation(tile_t *tile, player_t *player,
    elevation_requirement_t *requirements);
void check_and_send_elevation_status(server_t *server, player_t *player,
    tile_t *current_tile, elevation_requirement_t *requirements);
size_t count_players_with_level(tile_t *tile, player_t *player);

#endif // ELEVATION_H
