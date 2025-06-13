/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** game_loop
*/

#ifndef GAME_LOOP_H
    #define GAME_LOOP_H

    #include "server.h"
    #include "death.h"
    #include "team.h"

void game_loop_init(server_t *server);
void game_loop_tick(server_t *server);
void game_loop_run(server_t *server);

#endif // GAME_LOOP_H
