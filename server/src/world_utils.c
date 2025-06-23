/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** World utility functions
*/

#include "../include/world.h"
#include "../include/server.h"

int get_resource_type_from_name(const char *resource_name)
{
    const char **resource_names = get_resource_names();

    if (!resource_name)
        return -1;
    for (int i = 0; i < RESOURCE_COUNT; i++) {
        if (strcmp(resource_name, resource_names[i]) == 0) {
            return i;
        }
    }
    return -1;
}

bool take_resource_from_tile(tile_t *tile, int resource_type)
{
    if (!tile || resource_type < 0 || resource_type >= RESOURCE_COUNT)
        return false;
    if (tile->resources[resource_type] > 0) {
        tile->resources[resource_type]--;
        return true;
    }
    return false;
}
