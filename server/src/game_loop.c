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
    printf("Game loop initialized.\n");
}

void game_loop_tick(server_t *server)
{
    player_t *player = NULL;

    if (!server || !server->game_running)
        return;
    server->tick_count++;
    printf("Game tick: %d\n", server->tick_count);
    for (size_t i = 0; i < server->player_count; i++) {
        player = server->players[i];
        if (player && !player->dead)
            printf("Processing commands for player %d\n", player->id);
    }
    death_check(server->players, server->player_count, server->map);
    for (int i = 0; i < team_count; i++) {
        if (win_check(teams[i]->name, server->players, server->player_count)) {
            server->game_running = false;
            break;
        }
    }
}
