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
    size_t x;
    size_t y;
    bool is_incantation_in_progress;
} tile_t;

typedef struct map_s {
    tile_t **tiles;
    size_t width;
    size_t height;
} map_t;

map_t *create_map(size_t width, size_t height);
void destroy_map(map_t *map);
tile_t *get_tile(map_t *map, size_t x, size_t y);
tile_t *get_tile_toroidal(map_t *map, int x, int y);
void add_player_to_tile(tile_t *tile, player_t *player);
void remove_player_from_tile(tile_t *tile, player_t *player);
bool take_resource_from_tile(tile_t *tile, int resource_type);
bool set_resource_on_tile(tile_t *tile, int resource_type);
int get_resource_type_from_name(const char *resource_name);
void normalize_coordinates_toroidal(int *x, int *y, size_t width,
    size_t height);
int calculate_toroidal_distance(int x1, int y1, int x2, int y2,
    size_t width, size_t height);

#endif // WORLD_H
