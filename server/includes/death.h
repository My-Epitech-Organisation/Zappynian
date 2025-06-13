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

void death_check_all(player_t **players, int player_count, map_t *map);
void death_kill_player(player_t *player, map_t *map);

#endif // DEATH_H
