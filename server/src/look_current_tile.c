/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** look_cmd
*/

#include "../include/commands.h"
#include "../include/world.h"

void add_current_tile_players(char *result, bool *first_item)
{
    if (!*first_item)
        strcat(result, " ");
    strcat(result, "player");
    *first_item = false;
}

void add_current_tile_content(char *result, tile_t *tile, player_t *self,
    bool *first_item)
{
    add_current_tile_players(result, first_item);
    add_vision_tile_players(result, tile, self, first_item);
    add_resources_to_result(result, tile, first_item);
}

void add_tile(char *result, tile_t *tile, player_t *self, bool *first_tile)
{
    bool first_item = true;

    add_separator(result, first_tile);
    add_current_tile_content(result, tile, self, &first_item);
}

void add_current_tile_to_vision(char *result, player_t *player,
    map_t *map, bool *first_tile)
{
    tile_t *current = get_tile(map, player->x, player->y);

    add_tile(result, current, player, first_tile);
}
