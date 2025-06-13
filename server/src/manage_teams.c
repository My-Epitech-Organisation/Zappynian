/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** manage_teams
*/

#include "../includes/server.h"

// int init_teams(server_args_t *server)
// {
//     server->teams = calloc(server->team_count, sizeof(team_t));
//     if (server->teams == NULL) {
//         fprintf(stderr, "Memory allocation failed for teams.\n");
//         for (int i = 0; i < server->team_count; i++) {
//             free(server->team_names[i]);
//         }
//         free(server->team_names);
//         return 84;
//     }
//     for (int i = 0; i < server->team_count; i++) {
//         server->teams[i].name = strdup(server->team_names[i]);
//         if (server->teams[i].name == NULL) {
//             fprintf(stderr, "Memory allocation failed for team name.\n");
//             return 84;
//         }
//         server->teams[i].max_players = server->clients_per_team;
//         server->teams[i].current_players = 0;
//     }
//     return 0;
// }

// team_t *get_team_by_name(server_args_t *server, const char *name)
// {
//     for (int i = 0; i < server->team_count; i++) {
//         if (strcmp(server->teams[i].name, name) == 0) {
//             return &server->teams[i];
//         }
//     }
//     return NULL;
// }

// bool is_team_full(server_args_t *server, const char *name)
// {
//     team_t *team = get_team_by_name(server, name);

//     if (team == NULL) {
//         fprintf(stderr, "Team '%s' not found.\n", name);
//         return true;
//     }
//     return team->current_players >= team->max_players;
// }

// bool is_valid_team(server_args_t *args, const char *name)
// {
//     if (strcmp(name, "GRAPHIC") == 0)
//         return true;
//     return get_team_by_name(args, name) != NULL;
// }
