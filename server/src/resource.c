/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** resource
*/

#include "../include/resource.h"

void fill_resource(tile_t *tile, bool random)
{
    if (!random) {
        for (int i = 0; i < RESOURCE_COUNT; i++)
            tile->resources[i] = 0;
        return;
    }
    for (int i = 0; i < RESOURCE_COUNT; i++) {
        if (rand() % 4 == 0)
            tile->resources[i] += 1;
    }
}

void resource_init(map_t *map)
{
    tile_t *tile = NULL;

    if (!map || !map->tiles)
        return;
    for (size_t i = 0; i < map->width; i++) {
        for (size_t j = 0; j < map->height; j++) {
            tile = &map->tiles[i][j];
            fill_resource(tile, false);
        }
    }
}

void resource_respawn(map_t *map)
{
    tile_t *tile = NULL;

    if (!map || !map->tiles)
        return;
    for (size_t i = 0; i < map->width; i++) {
        for (size_t j = 0; j < map->height; j++) {
            tile = &map->tiles[i][j];
            fill_resource(tile, false);
        }
    }
}

void resource_add(tile_t *tile, int resource_type, int amount)
{
    if (!tile || resource_type < 0 || resource_type >= RESOURCE_COUNT ||
        amount <= 0) {
        return;
    }
    tile->resources[resource_type] += amount;
}

bool resource_remove(tile_t *tile, int resource_type, int amount)
{
    if (!tile || resource_type < 0 || resource_type >= RESOURCE_COUNT ||
        amount <= 0 || tile->resources[resource_type] < amount) {
        return false;
    }
    tile->resources[resource_type] -= amount;
    return true;
}
