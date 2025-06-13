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
    player_t *player = NULL;

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
    while (server->game_running != false) {
        game_loop_tick(server);
        handle_clients(server);
        process_commands(server);
        decrement_food_for_all_players(server);
        death_check(server->players, server->player_count, server->map,
            server);
        check_victory(server);
        usleep(10000);
    }
}
