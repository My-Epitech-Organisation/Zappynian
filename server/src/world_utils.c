/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** World utility functions
*/

#include "../include/world.h"
#include "../include/server.h"

int get_resource_type_from_name(const char *resource_name)
{
    const char **resource_names = get_resource_names();

    if (!resource_name)
        return -1;
    for (int i = 0; i < RESOURCE_COUNT; i++) {
        if (strcmp(resource_name, resource_names[i]) == 0) {
            return i;
        }
    }
    return -1;
}

bool take_resource_from_tile(tile_t *tile, int resource_type)
{
    if (!tile || resource_type < 0 || resource_type >= RESOURCE_COUNT)
        return false;
    if (tile->resources[resource_type] > 0) {
        tile->resources[resource_type]--;
        return true;
    }
    return false;
}

bool set_resource_on_tile(tile_t *tile, int resource_type)
{
    if (!tile || resource_type < 0 || resource_type >= RESOURCE_COUNT)
        return false;
    tile->resources[resource_type]++;
    return true;
}

void normalize_coordinates_toroidal(int *x, int *y, size_t width,
    size_t height)
{
    if (!x || !y || width == 0 || height == 0)
        return;
    while (*x < 0)
        *x += width;
    while (*y < 0)
        *y += height;
    while ((size_t)*x >= width)
        *x -= width;
    while ((size_t)*y >= height)
        *y -= height;
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
