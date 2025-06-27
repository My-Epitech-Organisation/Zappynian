/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** resource
*/

#ifndef RESOURCE_H
    #define RESOURCE_H

    #include <stdbool.h>
    #include <stddef.h>
    #include <stdlib.h>
    #include "world.h"

enum resource_type {
    RESOURCE_FOOD = 0,
    RESOURCE_LINEMATE,
    RESOURCE_DERAUMERE,
    RESOURCE_SIBUR,
    RESOURCE_MENDIANE,
    RESOURCE_PHIRAS,
    RESOURCE_THYSTAME
};

void resource_init(map_t *map);
void resource_spawn_periodic(map_t *map);
void resource_check_minimum(map_t *map);
char *get_resource_name(int resource_type);

#endif // RESOURCE_H
