/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** resource
*/

#ifndef RESOURCE_H
    #define RESOURCE_H

    #include "world.h"

void resource_respawn(map_t *map);
void init_resource_spawn_rates(void);
int get_resource_spawn_rate(const char *resource_name);

#endif // RESOURCE_H
