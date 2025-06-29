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
        safe_strcat(result, ",", MAX_VISION_BUFFER);
    *first_tile = false;
}

void add_single_player(char *result, bool *first_item)
{
    if (!*first_item)
        safe_strcat(result, " ", MAX_VISION_BUFFER);
    safe_strcat(result, "player", MAX_VISION_BUFFER);
    *first_item = false;
}

static void add_single_resource(char *result, int resource_type,
    bool *first_item)
{
    if (!*first_item)
        safe_strcat(result, " ", MAX_VISION_BUFFER);
    safe_strcat(result, get_resource_name(resource_type), MAX_VISION_BUFFER);
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
    char *result = malloc(MAX_VISION_BUFFER);
    bool first_tile = true;
    size_t current_len = 0;

    if (!result)
        return NULL;
    
    // Check parameters
    if (!player || !map) {
        free(result);
        return NULL;
    }
    
    // Initialize with opening bracket
    result[0] = '[';
    result[1] = '\0';
    current_len = 1;
    
    // Check if we have enough space before calling helper functions
    if (current_len >= MAX_VISION_BUFFER - 2) { // Reserve space for ']' and '\0'
        free(result);
        return NULL;
    }
    
    add_current_tile_to_vision(result, player, map, &first_tile);
    add_all_vision_tiles(result, player, map, &first_tile);
    
    // Add closing bracket with bounds checking
    current_len = strlen(result);
    if (current_len >= MAX_VISION_BUFFER - 2) {
        free(result);
        return NULL;
    }
    
    result[current_len] = ']';
    result[current_len + 1] = '\0';
    
    return result;
}
