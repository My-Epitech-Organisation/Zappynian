/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** world
*/

#ifndef WORLD_H
    #define WORLD_H

    #include <stdlib.h>
    #include <stdio.h>
    #include <stdbool.h>

typedef struct resource_s {
    int food;
    int linemate;
    int deraumere;
    int sibur;
    int mendiane;
    int phiras;
    int thystame;
} resource_t;

typedef struct tile_s {
    resource_t resources;
    struct player_s **players;
    size_t player_count;
} tile_t;

typedef struct map_s {
    tile_t **tiles;
    size_t width;
    size_t height;
} map_t;

map_t *create_map(size_t width, size_t height);
void destroy_map(map_t *map);
void set_resource(tile_t *tile, bool random_spawn);
tile_t *get_tile(map_t *map, size_t x, size_t y);
void add_player_to_tile(tile_t *tile, struct player_s *player);
void remove_player_from_tile(tile_t *tile, struct player_s *player);

#endif // WORLD_H
