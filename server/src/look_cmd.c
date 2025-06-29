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

static char *allocate_vision_buffer(void)
{
    char *result = malloc(MAX_VISION_BUFFER);

    if (!result)
        return NULL;
    result[0] = '[';
    result[1] = '\0';
    return result;
}

static bool finalize_vision_buffer(char *result)
{
    size_t current_len = strlen(result);

    if (current_len >= MAX_VISION_BUFFER - 2)
        return false;
    result[current_len] = ']';
    result[current_len + 1] = '\0';
    return true;
}

static bool build_vision_content(char *result, player_t *player, map_t *map)
{
    bool first_tile = true;

    add_current_tile_to_vision(result, player, map, &first_tile);
    add_all_vision_tiles(result, player, map, &first_tile);
    return finalize_vision_buffer(result);
}

char *get_player_vision(player_t *player, map_t *map)
{
    char *result;

    if (!player || !map)
        return NULL;
    result = allocate_vision_buffer();
    if (!result)
        return NULL;
    if (!build_vision_content(result, player, map)) {
        free(result);
        return NULL;
    }
    return result;
}
