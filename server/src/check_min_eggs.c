/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** check_min_eggs
*/

#include "../include/server.h"
#include "../include/egg.h"
#include "../include/team.h"

static int get_team_egg_count(egg_t *eggs, const char *team_name)
{
    int count = 0;
    egg_t *current = eggs;

    while (current != NULL) {
        if (strcmp(current->team_name, team_name) == 0)
            count++;
        current = current->next;
    }
    return count;
}

static int get_team_player_count(server_t *server, const char *team_name)
{
    int count = 0;

    for (size_t i = 0; i < server->player_count; i++) {
        if (server->players[i] != NULL &&
            strcmp(server->players[i]->team_name, team_name) == 0) {
            count++;
        }
    }
    return count;
}

void make_enough_eggs_for_team(server_t *server, int team_idx)
{
    team_t *team = &server->args->teams[team_idx];
    int current_players = get_team_player_count(server, team->name);
    int current_eggs = get_team_egg_count(server->eggs, team->name);
    int total_slots_filled = current_players + current_eggs;
    int needed_eggs = team->max_slots - total_slots_filled;
    int x = 0;
    int y = 0;
    egg_t *new_egg;

    for (int i = 0; i < needed_eggs && team->remaining_slots > 0; i++) {
        x = rand() % server->args->width;
        y = rand() % server->args->height;
        new_egg = create_egg(x, y, team->name);
        if (new_egg != NULL) {
            add_egg_to_server(server, new_egg);
            team->remaining_slots--;
        } else
            break;
    }
}
