/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** world
*/

#include "../include/world.h"
#include <time.h>
#include <stdbool.h>
#include "../include/server.h"
#include "../include/player.h"
#include "../include/resource.h"

static int set_tiles(map_t *map, size_t width, size_t height, size_t y)
{
    if (y >= height) {
        fprintf(stderr, "Invalid tile row index: %zu\n", y);
        return 84;
    }
    if (!map->tiles[y]) {
        perror("Failed to allocate memory for tile row");
        return 84;
    }
    for (size_t x = 0; x < width; x++) {
        for (int i = 0; i < RESOURCE_COUNT; i++)
            map->tiles[y][x].resources[i] = 0;
        map->tiles[y][x].players = NULL;
        map->tiles[y][x].player_count = 0;
    }
    return 0;
}

static int allocate_and_set_tile_row(map_t *map, size_t width,
    size_t height, size_t y)
{
    map->tiles[y] = malloc(width * sizeof(tile_t));
    if (set_tiles(map, width, height, y) == 84) {
        for (size_t i = 0; i < y; i++)
            free(map->tiles[i]);
        free(map->tiles);
        free(map);
        return 84;
    }
    return 0;
}

map_t *create_map(size_t width, size_t height)
{
    map_t *map = malloc(sizeof(map_t));

    if (!map) {
        perror("Failed to allocate memory for map");
        return NULL;
    }
    map->width = width;
    map->height = height;
    map->tiles = malloc(height * sizeof(tile_t *));
    if (!map->tiles) {
        perror("Failed to allocate memory for map tiles");
        free(map);
        return NULL;
    }
    for (size_t y = 0; y < height; y++) {
        if (allocate_and_set_tile_row(map, width, height, y) == 84)
            return NULL;
    }
    resource_init(map);
    return map;
}

tile_t *get_tile(map_t *map, size_t x, size_t y)
{
    if (!map || x >= map->width || y >= map->height) {
        fprintf(stderr, "Invalid tile coordinates: (%zu, %zu)\n", x, y);
        return NULL;
    }
    return &map->tiles[y][x];
}

void add_player_to_tile(tile_t *tile, player_t *player)
{
    if (!tile || !player) {
        fprintf(stderr, "Invalid tile or player\n");
        return;
    }
    tile->players = realloc(tile->players, (tile->player_count + 1) *
        sizeof(player_t *));
    if (!tile->players) {
        perror("Failed to allocate memory for players in tile");
        return;
    }
    tile->players[tile->player_count] = player;
    tile->player_count++;
}

static void remove_player(tile_t *tile, size_t i)
{
    for (size_t j = i; j < tile->player_count - 1; j++)
        tile->players[j] = tile->players[j + 1];
    tile->player_count--;
    tile->players = realloc(tile->players, tile->player_count *
        sizeof(player_t *));
    if (!tile->players && tile->player_count > 0) {
        perror("Failed to reallocate memory for players in tile");
    }
    return;
}

void remove_player_from_tile(tile_t *tile, player_t *player)
{
    if (!tile || !player || tile->player_count == 0) {
        fprintf(stderr, "Invalid tile or player, or no players in tile\n");
        return;
    }
    for (size_t i = 0; i < tile->player_count; i++) {
        if (tile->players[i] == player)
            return remove_player(tile, i);
    }
    fprintf(stderr, "Player not found in tile\n");
}

static void normalize_coordinates(int *x, int *y, map_t *map)
{
    if (!map || !x || !y)
        return;
    while (*x < 0)
        *x += map->width;
    while (*y < 0)
        *y += map->height;
    while ((size_t)*x >= map->width)
        *x -= map->width;
    while ((size_t)*y >= map->height)
        *y -= map->height;
}

tile_t *get_tile_toroidal(map_t *map, int x, int y)
{
    if (!map)
        return NULL;
    normalize_coordinates(&x, &y, map);
    return &map->tiles[y][x];
}
