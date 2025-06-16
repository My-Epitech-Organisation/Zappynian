/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** look_cmd
*/

#include "../include/commands.h"
#include "../include/world.h"

void add_tile_contents(char *result, int x, int y, map_t *map)
{
    tile_t *tile = get_tile(map, x, y);

    if (!tile)
        return;
    for (int i = 0; i < RESOURCE_COUNT; i++) {
        add_resource_type(result, tile, i, true);
    }
}

void calculate_vision_coordinates(player_t *player, int distance, int offset,
    int *pos)
{
    switch (player->orientation) {
        case NORTH:
            pos[0] = player->x + offset;
            pos[1] = player->y - distance;
            break;
        case EAST:
            pos[0] = player->x + distance;
            pos[1] = player->y + offset;
            break;
        case SOUTH:
            pos[0] = player->x - offset;
            pos[1] = player->y + distance;
            break;
        case WEST:
            pos[0] = player->x - distance;
            pos[1] = player->y - offset;
            break;
    }
}

void add_other_tiles(char *result, player_t *player, map_t *map,
    int vision_range)
{
    int pos[2] = {0, 0};

    for (int distance = 1; distance <= vision_range; distance++) {
        for (int offset = -distance; offset <= distance; offset++) {
            calculate_vision_coordinates(player, distance, offset, pos);
            pos[0] = (pos[0] + map->width) % map->width;
            pos[1] = (pos[1] + map->height) % map->height;
            strcat(result, ",");
            add_tile_contents(result, pos[0], pos[1], map);
        }
    }
}
