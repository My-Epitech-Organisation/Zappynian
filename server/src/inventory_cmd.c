/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** inventory_cmd
*/

#include "../include/commands.h"
#include "../include/server.h"
#include "../include/player.h"

const char **get_resource_names(void)
{
    static const char *resource_names[RESOURCE_COUNT] = {
        "food", "linemate", "deraumere", "sibur", "mendiane", "phiras",
        "thystame"
    };

    return resource_names;
}

int get_resource_type_from_name(const char *resource_name)
{
    const char **resource_names = get_resource_names();

    if (!resource_name)
        return -1;
    for (int i = 0; i < RESOURCE_COUNT; i++) {
        if (strcmp(resource_name, resource_names[i]) == 0)
            return i;
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

void add_inventory_items(char *result, player_t *player)
{
    char buffer[256];
    bool first = true;
    const char **resource_names = get_resource_names();

    sprintf(result, "food %d", player->food);
    strcat(result, buffer);
    first = false;
    for (int i = 1; i < RESOURCE_COUNT; i++) {
        if (!first)
            strcat(result, ", ");
        sprintf(buffer, "%s %d", resource_names[i], player->resources[i]);
        strcat(result, buffer);
        first = false;
    }
}

char *get_player_inventory(player_t *player)
{
    char *inventory_result = malloc(4096);

    strcpy(inventory_result, "[");
    add_inventory_items(inventory_result, player);
    strcat(inventory_result, "]");
    return inventory_result;
}
