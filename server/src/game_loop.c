/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** game_loop
*/

#include "../includes/game_loop.h"

void game_loop_init(server_t *server)
{
    if (!server) {
        return;
    }
    server->tick_count = 0;
    server->game_running = true;
}

void game_loop_tick(server_t *server)
{
    if (!server || !server->game_running)
        return;
    server->tick_count++;
}

void game_loop_run(server_t *server)
{
    if (!server) {
        return;
    }
    game_loop_init(server);
    if (!server->game_running)
        return;

    // The new server_loop function now handles the main loop with polling
    // as well as calling the game logic functions
    server_loop(server);
}
