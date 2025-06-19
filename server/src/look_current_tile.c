/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** look_cmd
*/

#include "../include/commands.h"
#include "../include/world.h"

void add_resource_type(char *result, tile_t *tile, int resource_type,
    bool first)
{
    for (int i = 0; i < tile->resources[resource_type]; i++) {
        if (!first) {
            strcat(result, ",");
        }
        first = false;
        strcat(result, get_resource_name(resource_type));
    }
}

void add_current_tile(char *result, player_t *player, map_t *map)
{
    tile_t *tile = get_tile(map, player->x, player->y);

    if (!tile)
        return;
    for (int i = 0; i < RESOURCE_COUNT; i++) {
        add_resource_type(result, tile, i, false);
    }
}
