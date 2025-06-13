/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** death
*/

#ifndef DEATH_H
    #define DEATH_H

    #include "player.h"
    #include "world.h"

void death_check(player_t **players, int player_count, map_t *map);
void death_handle(player_t *player, map_t *map);

#endif // DEATH_H
