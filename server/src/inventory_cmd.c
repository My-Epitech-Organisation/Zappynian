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

void add_inventory_items(char *result, player_t *player)
{
    char buffer[256];
    const char **resource_names = get_resource_names();

    sprintf(result, "[ food %d", player->food);
    for (int i = 1; i < RESOURCE_COUNT; i++) {
        sprintf(buffer, ", %s %d", resource_names[i], player->resources[i]);
        strcat(result, buffer);
    }
}

char *get_player_inventory(player_t *player)
{
    char *inventory_result = malloc(4096);

    if (inventory_result == NULL)
        return NULL;
    inventory_result[0] = '\0';
    add_inventory_items(inventory_result, player);
    strcat(inventory_result, " ]");
    return inventory_result;
}
