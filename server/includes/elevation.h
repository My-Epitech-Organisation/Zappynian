/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** elevation
*/

#ifndef ELEVATION_H
    #define ELEVATION_H

    #include "player.h"
    #include "world.h"

bool elevation_start(player_t **players, int player_count, map_t *map);
bool elevation_check(player_t **players, int player_count, map_t *map);
void elevation_end(player_t **players, int player_count, bool success);

#endif // ELEVATION_H
