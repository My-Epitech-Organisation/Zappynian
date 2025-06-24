/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** team
*/

#include "../include/team.h"

team_t *get_team_by_name(server_args_t *server, const char *name)
{
    if (server == NULL || server->teams == NULL || name == NULL) {
        return NULL;
    }
    for (int i = 0; i < server->team_count; i++) {
        if (server->teams[i].name != NULL &&
            strcmp(server->teams[i].name, name) == 0) {
            return &server->teams[i];
        }
    }
    return NULL;
}

void team_free_slot(server_args_t *server, const char *team_name)
{
    team_t *team = get_team_by_name(server, team_name);

    if (team) {
        team->remaining_slots += 1;
        printf("Slot libéré pour l'équipe %s. Slots restants : %d\n",
            team_name, team->remaining_slots);
    }
}

int init_teams(server_args_t *server)
{
    server->teams = calloc(server->team_count, sizeof(team_t));
    if (server->teams == NULL) {
        fprintf(stderr, "Memory allocation failed for teams.\n");
        for (int i = 0; i < server->team_count; i++) {
            free(server->team_names[i]);
        }
        free(server->team_names);
        return 84;
    }
    for (int i = 0; i < server->team_count; i++) {
        server->teams[i].name = strdup(server->team_names[i]);
        if (server->teams[i].name == NULL) {
            fprintf(stderr, "Memory allocation failed for team name.\n");
            return 84;
        }
        server->teams[i].max_slots = server->clients_per_team;
        server->teams[i].remaining_slots = server->clients_per_team;
        server->teams[i].current_players = 0;
        server->teams[i].players = NULL;
    }
    return 0;
}

bool is_team_full(server_args_t *server, const char *name)
{
    team_t *team = get_team_by_name(server, name);

    if (team == NULL) {
        fprintf(stderr, "Team '%s' is not found.\n", name);
        return true;
    }
    return team->current_players >= team->max_slots;
}

void increment_team_player_slots(server_args_t *server, const char *team_name)
{
    team_t *team = get_team_by_name(server, team_name);

    if (team) {
        team->current_players += 1;
        if (team->remaining_slots > 0)
            team->remaining_slots -= 1;
    }
}
