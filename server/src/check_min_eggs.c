/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** Ensure minimum eggs for teams
*/

#include "../include/server.h"
#include "../include/egg.h"
#include "../include/team.h"

static int count_team_eggs(server_t *server, const char *team_name)
{
    int count = 0;
    egg_t *current = server->eggs;

    while (current) {
        if (current->team_name && strcmp(current->team_name, team_name) == 0
            && !current->is_hatched)
            count++;
        current = current->next;
    }
    return count;
}

void make_enough_eggs_for_team(server_t *server, int team_idx)
{
    team_t *team = &server->args->teams[team_idx];
    int current_eggs = count_team_eggs(server, team->name);
    int connected_players = team->current_players;
    int available_slots = team->remaining_slots;
    int needed_eggs = available_slots - current_eggs;
    int x = 0;
    int y = 0;
    egg_t *egg;

    for (int i = 0; i < needed_eggs; i++) {
        x = rand() % server->args->width;
        y = rand() % server->args->height;
        egg = create_egg(x, y, team->name);
        if (egg) {
            add_egg_to_server(server, egg);
            send_enw(server, egg);
        }
    }
}
