/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** look_cmd
*/

#include "../include/commands.h"
#include "../include/world.h"

void calculate_vision_coordinates(player_t *player, int distance, int offset,
    int *pos)
{
    switch (player->orientation) {
        case 0:
            pos[0] = player->x + offset;
            pos[1] = player->y - distance;
            break;
        case 1:
            pos[0] = player->x + distance;
            pos[1] = player->y + offset;
            break;
        case 2:
            pos[0] = player->x - offset;
            pos[1] = player->y + distance;
            break;
        case 3:
            pos[0] = player->x - distance;
            pos[1] = player->y - offset;
            break;
    }
}

void add_vision_tile_players(char *result, tile_t *tile,
    player_t *self, bool *first_item)
{
    for (size_t i = 0; i < tile->player_count; i++) {
        if (tile->players[i] != self)
            add_single_player(result, first_item);
    }
}

void add_vision_tile_content(char *result, tile_t *tile, player_t *self,
    bool *first_item)
{
    add_vision_tile_players(result, tile, self, first_item);
    add_resources_to_result(result, tile, first_item);
}

void add_vision_tile(char *result, tile_t *tile, player_t *self,
    bool *first_tile)
{
    bool first_item = true;

    add_separator(result, first_tile);
    add_vision_tile_content(result, tile, self, &first_item);
}

void add_all_vision_tiles(char *result, player_t *player, map_t *map,
    bool *first_tile)
{
    int distance;
    int offset;
    int pos[2];
    tile_t *tile = NULL;

    for (distance = 1; distance <= player->level; distance++) {
        for (offset = -distance; offset <= distance; offset++) {
            calculate_vision_coordinates(player, distance, offset, pos);
            normalize_coordinates_toroidal(&pos[0], &pos[1], map->width,
                map->height);
            tile = get_tile(map, pos[0], pos[1]);
            add_vision_tile(result, tile, player, first_tile);
        }
    }
}
