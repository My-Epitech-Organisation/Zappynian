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
void start_incantation(tile_t *tile, int player_level,
    const elevation_requirement_t *requirements);
bool complete_incantation(tile_t *tile, int player_level,
    const elevation_requirement_t *requirements);
void apply_elevation(tile_t *tile, int player_level,
    const elevation_requirement_t *requirements);
void cancel_incantation(tile_t *tile, int player_level);

#endif // ELEVATION_H
