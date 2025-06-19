/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** inventory_cmd
*/

#include "../include/commands.h"
#include "../include/server.h"
#include "../include/player.h"

void add_inventory_items(char *result, player_t *player)
{
    char buffer[256];
    bool first = true;
    const char *resource_names[RESOURCE_COUNT] = {
        "food", "linemate", "deraumere", "sibur", "mendiane", "phiras",
        "thystame"
    };

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
