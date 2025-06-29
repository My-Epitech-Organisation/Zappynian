/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** look_cmd
*/

#include "../include/commands.h"
#include "../include/world.h"

void add_separator(char *result, bool *first_tile)
{
    if (!(*first_tile))
        strcat(result, ",");
    *first_tile = false;
}

void add_single_player(char *result, bool *first_item)
{
    if (!*first_item)
        strcat(result, " ");
    strcat(result, "player");
    *first_item = false;
}

static void add_single_resource(char *result, int resource_type,
    bool *first_item)
{
    if (!*first_item)
        strcat(result, " ");
    strcat(result, get_resource_name(resource_type));
    *first_item = false;
}

void add_resources_to_result(char *result, tile_t *tile,
    bool *first_item)
{
    for (int i = 0; i < RESOURCE_COUNT; i++) {
        for (int j = 0; j < tile->resources[i]; j++) {
            add_single_resource(result, i, first_item);
        }
    }
}

char *get_player_vision(player_t *player, map_t *map)
{
    char *result = malloc(4096);
    bool first_tile = true;

    if (!result)
        return NULL;
    strcpy(result, "[");
    add_current_tile_to_vision(result, player, map, &first_tile);
    add_all_vision_tiles(result, player, map, &first_tile);
    strcat(result, "]");
    return result;
}
