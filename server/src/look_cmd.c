/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** look_cmd
*/

#include "../include/commands.h"
#include "../include/world.h"

void add_tile(char *result, tile_t *tile, player_t *self, bool *first_tile,
    bool is_current) {
    bool first_item = true;
    player_t *p = NULL;

    if (!(*first_tile)) {
        strcat(result, ",");
    }
    *first_tile = false;
    for (size_t i = 0; i < tile->player_count; i++) {
        p = tile->players[i];
        if (p == self && !is_current)
            continue;
        if (!first_item) strcat(result, " ");
        strcat(result, "player");
        first_item = false;
    }
    if (is_current) {
        if (!first_item) strcat(result, " ");
        strcat(result, "player");
        first_item = false;
    }
    for (int i = 0; i < RESOURCE_COUNT; i++) {
        for (int j = 0; j < tile->resources[i]; j++) {
            if (!first_item) strcat(result, " ");
            strcat(result, get_resource_name(i));
            first_item = false;
        }
    }
}

char *get_player_vision(player_t *player, map_t *map)
{
    char *result = malloc(4096);
    bool first_tile = true;
    int pos[2];
    tile_t *current = NULL;
    tile_t *tile = NULL;

    if (!result)
        return NULL;
    strcpy(result, "[ ");
    current = get_tile(map, player->x, player->y);
    add_tile(result, current, player, &first_tile, true);
    for (int distance = 1; distance <= player->level; distance++) {
        for (int offset = -distance; offset <= distance; offset++) {
            calculate_vision_coordinates(player, distance, offset, pos);
            normalize_coordinates_toroidal(&pos[0], &pos[1], map->width,
                map->height);
            tile = get_tile(map, pos[0], pos[1]);
            add_tile(result, tile, player, &first_tile, false);
        }
    }
    strcat(result, " ]");
    return result;
}
