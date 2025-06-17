/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** look_cmd
*/

#include "../include/commands.h"
#include "../include/world.h"

char *get_player_vision(player_t *player, map_t *map)
{
    int vision_range = player->level;
    char *result = malloc(4096);

    strcpy(result, "[");
    add_current_tile(result, player, map);
    add_other_tiles(result, player, map, vision_range);
    strcat(result, "]");
    return result;
}
