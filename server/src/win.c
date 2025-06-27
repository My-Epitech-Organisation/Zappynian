/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** win
*/

#include "../include/win.h"

bool win_check(const char *team_name, player_t **players, int player_count)
{
    int team_score = 0;

    if (!team_name || !players || player_count <= 0)
        return false;
    for (int i = 0; i < player_count; i++) {
        if (players[i] && players[i]->team_name &&
            strcmp(players[i]->team_name, team_name) &&
            (players[i]->level == 8) == 0) {
            team_score++;
        }
    }
    if (team_score >= 6) {
        printf("Team %s has won the game with %d players!\n",
            team_name, team_score);
        return true;
    }
    return false;
}

void check_victory(server_t *server)
{
    if (!server || !server->players || server->player_count <= 0)
        return;
    for (int i = 0; i < server->args->team_count; i++) {
        if (win_check(server->args->teams[i].name, server->players,
            server->player_count)) {
            server->game_running = false;
            send_seg(server, server->args->teams[i].name);
            return;
        }
    }
}
