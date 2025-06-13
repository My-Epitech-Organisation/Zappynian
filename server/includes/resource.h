/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** resource
*/

#ifndef RESOURCE_H
    #define RESOURCE_H
    #define RESOURCE_COUNT 7

    #include <stdbool.h>
    #include <stddef.h>

typedef struct tile_s tile_t;
typedef struct map_s map_t;

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
void resource_respawn(map_t *map);
void resource_add(tile_t *tile, int resource_type, int amount);
bool resource_remove(tile_t *tile, int resource_type, int amount);

#endif // RESOURCE_H
