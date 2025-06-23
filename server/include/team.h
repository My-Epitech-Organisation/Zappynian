/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** team
*/

#ifndef TEAM_H
    #define TEAM_H
    #define MAX_TEAMS 16

    #include <stdio.h>
    #include <string.h>
    #include <stdlib.h>
    #include <stddef.h>
    #include <stdbool.h>
    #include "server.h"
    #include "player.h"

typedef struct team_s {
    char *name;
    int max_slots;
    int remaining_slots;
    int current_players;
    player_t **players;
} team_t;

team_t *get_team_by_name(server_args_t *server, const char *name);
void team_free_slot(server_args_t *server, const char *team_name);
int init_teams(server_args_t *server);
bool is_team_full(server_args_t *server, const char *name);
void increment_team_player_slots(server_args_t *server, const char *team_name);

#endif // TEAM_H
