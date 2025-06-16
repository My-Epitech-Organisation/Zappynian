/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** world
*/

#ifndef WORLD_H
    #define WORLD_H
    #define RESOURCE_COUNT 7

    #include <stdbool.h>
    #include <stddef.h>

typedef struct player_s player_t;

typedef struct tile_s {
    int resources[RESOURCE_COUNT];
    player_t **players;
    size_t player_count;
} tile_t;

typedef struct map_s {
    tile_t **tiles;
    size_t width;
    size_t height;
} map_t;

map_t *create_map(size_t width, size_t height);
void destroy_map(map_t *map);
tile_t *get_tile(map_t *map, size_t x, size_t y);
void add_player_to_tile(tile_t *tile, player_t *player);
void remove_player_from_tile(tile_t *tile, player_t *player);

#endif // WORLD_H
