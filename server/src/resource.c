/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** resource
*/

#include "../includes/resource.h"
#include "../includes/world.h"
#include "../includes/server.h"
#include <stdlib.h>
#include <time.h>

void resource_respawn(map_t *map)
{
    if (!map)
        return;
    
    for (size_t y = 0; y < map->height; y++) {
        for (size_t x = 0; x < map->width; x++) {
            if (rand() % 100 < 10) { // 10% chance
                map->tiles[y][x].resources.food++;
            }
            if (rand() % 100 < 5) { // 5% chance
                map->tiles[y][x].resources.linemate++;
            }
        }
    }
}

void init_resource_spawn_rates(void)
{
    srand(time(NULL));
}

int get_resource_spawn_rate(const char *resource_name)
{
    if (!resource_name)
        return 0;
    return 10; // Default spawn rate
}

void set_resource(tile_t *tile, bool random_spawn)
{
    if (!tile)
        return;
    
    if (random_spawn) {
        // Random resource spawning
        if (rand() % 100 < 15) {
            tile->resources.food = rand() % 3 + 1;
        }
        if (rand() % 100 < 10) {
            tile->resources.linemate = rand() % 2 + 1;
        }
        if (rand() % 100 < 8) {
            tile->resources.deraumere = rand() % 2 + 1;
        }
        if (rand() % 100 < 6) {
            tile->resources.sibur = rand() % 2 + 1;
        }
        if (rand() % 100 < 4) {
            tile->resources.mendiane = rand() % 2 + 1;
        }
        if (rand() % 100 < 3) {
            tile->resources.phiras = rand() % 2 + 1;
        }
        if (rand() % 100 < 2) {
            tile->resources.thystame = rand() % 2 + 1;
        }
    } else {
        // Initialize to default values
        tile->resources.food = 1;
        tile->resources.linemate = 0;
        tile->resources.deraumere = 0;
        tile->resources.sibur = 0;
        tile->resources.mendiane = 0;
        tile->resources.phiras = 0;
        tile->resources.thystame = 0;
    }
}