/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** resource
*/

#include "../include/resource.h"
#include <time.h>
#include <stdio.h>

static double get_resource_density(int resource_type)
{
    switch (resource_type) {
        case RESOURCE_FOOD:
            return 0.5;
        case RESOURCE_LINEMATE:
            return 0.3;
        case RESOURCE_DERAUMERE:
            return 0.15;
        case RESOURCE_SIBUR:
            return 0.1;
        case RESOURCE_MENDIANE:
            return 0.1;
        case RESOURCE_PHIRAS:
            return 0.08;
        case RESOURCE_THYSTAME:
            return 0.05;
        default:
            return 0.0;
    }
}

static int calculate_resource_quantity(size_t map_width, size_t map_height,
    int resource_type)
{
    int total_tiles = 0;
    double density = 0.0;
    int quantity = 0;

    if (resource_type < 0 || resource_type >= RESOURCE_COUNT)
        return 0;
    total_tiles = map_width * map_height;
    density = get_resource_density(resource_type);
    quantity = (int)(total_tiles * density);
    return quantity < 1 ? 1 : quantity;
}

static void add_ressource_to_tile(map_t *map, size_t y, int resource_type,
    int resources_per_tile)
{
    for (size_t x = 0; x < map->width; x++) {
        if (resources_per_tile > 0) {
            map->tiles[y][x].resources[resource_type] += resources_per_tile;
        }
    }
}

static void spawn_resource_evenly(map_t *map, int resource_type, int quantity)
{
    int total_tiles = 0;
    int resources_per_tile = 0;
    int remaining_resources = 0;
    size_t rand_x = 0;
    size_t rand_y = 0;

    if (!map || !map->tiles || resource_type < 0 ||
        resource_type >= RESOURCE_COUNT || quantity <= 0)
        return;
    total_tiles = map->width * map->height;
    resources_per_tile = quantity / total_tiles;
    remaining_resources = quantity % total_tiles;
    for (size_t y = 0; y < map->height; y++)
        add_ressource_to_tile(map, y, resource_type, resources_per_tile);
    for (int i = 0; i < remaining_resources; i++) {
        rand_x = rand() % map->width;
        rand_y = rand() % map->height;
        map->tiles[rand_y][rand_x].resources[resource_type]++;
    }
}

static int count_total_resource(map_t *map, int resource_type)
{
    int total = 0;

    if (!map || !map->tiles || resource_type < 0 ||
        resource_type >= RESOURCE_COUNT)
        return 0;
    for (size_t y = 0; y < map->height; y++) {
        for (size_t x = 0; x < map->width; x++) {
            total += map->tiles[y][x].resources[resource_type];
        }
    }
    return total;
}

static void set_resource_to_zero(map_t *map, int y, int x)
{
    for (int i = 0; i < RESOURCE_COUNT; i++)
        map->tiles[y][x].resources[i] = 0;
}

void resource_init(map_t *map)
{
    int quantity = 0;

    if (!map || !map->tiles)
        return;
    srand(time(NULL));
    for (size_t y = 0; y < map->height; y++) {
        for (size_t x = 0; x < map->width; x++) {
            set_resource_to_zero(map, x, y);
        }
    }
    for (int res_type = 0; res_type < RESOURCE_COUNT; res_type++) {
        quantity = calculate_resource_quantity(map->width, map->height,
            res_type);
        spawn_resource_evenly(map, res_type, quantity);
    }
}

void resource_spawn_periodic(map_t *map)
{
    int quantity = 0;
    int spawn_quantity = 0;

    if (!map || !map->tiles)
        return;
    for (int res_type = 0; res_type < RESOURCE_COUNT; res_type++) {
        quantity = calculate_resource_quantity(map->width, map->height,
            res_type);
        spawn_quantity = quantity / 10;
        if (spawn_quantity < 1)
            spawn_quantity = 1;
        spawn_resource_evenly(map, res_type, spawn_quantity);
    }
}

void resource_check_minimum(map_t *map)
{
    int total = 0;
    size_t rand_x = 0;
    size_t rand_y = 0;

    if (!map || !map->tiles)
        return;
    for (int res_type = 0; res_type < RESOURCE_COUNT; res_type++) {
        total = count_total_resource(map, res_type);
        if (total < 1) {
            rand_x = rand() % map->width;
            rand_y = rand() % map->height;
            map->tiles[rand_y][rand_x].resources[res_type]++;
        }
    }
}

char *get_resource_name(int resource_type)
{
    switch (resource_type) {
        case RESOURCE_FOOD:
            return "food";
        case RESOURCE_LINEMATE:
            return "linemate";
        case RESOURCE_DERAUMERE:
            return "deraumere";
        case RESOURCE_SIBUR:
            return "sibur";
        case RESOURCE_MENDIANE:
            return "mendiane";
        case RESOURCE_PHIRAS:
            return "phiras";
        case RESOURCE_THYSTAME:
            return "thystame";
        default:
            return "unknown";
    }
}
