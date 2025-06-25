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
bool can_start_incantation(tile_t *tile, int player_level,
    const elevation_requirement_t *requirements);
void check_and_send_elevation_status(server_t *server, player_t *player,
    tile_t *current_tile, const elevation_requirement_t *requirements);
void complete_incantation_ritual(player_t *player, server_t *server);
size_t count_players_with_level(tile_t *tile, player_t *player);
int check_good_nb_players_and_resources(tile_t *tile,
    elevation_requirement_t *req, player_t *player);
bool start_incantation_immediately(player_t *player, client_t *client,
    server_connection_t *connection);

#endif // ELEVATION_H
