/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** init_eggs
*/

#include "../include/server.h"
#include "../include/egg.h"
#include "../include/team.h"

int init_by_team(server_t *server, team_t *team)
{
    int x = 0;
    int y = 0;
    egg_t *egg;

    for (int j = 0; j < team->max_slots; j++) {
        if (team->remaining_slots <= 0)
            continue;
        if (is_team_full(server->args, team->name))
            continue;
        x = rand() % server->args->width;
        y = rand() % server->args->height;
        egg = create_egg(x, y, team->name);
        if (egg == NULL) {
            fprintf(stderr, "Failed to create egg.\n");
            return 84;
        }
        add_egg_to_server(server, egg);
    }
    return 0;
}

int init_eggs_list(server_t *server)
{
    int nb_teams = server->args->team_count;

    if (server == NULL || server->eggs != NULL) {
        fprintf(stderr, "Invalid server or eggs list already initialized.\n");
        return 84;
    }
    server->eggs = NULL;
    for (int i = 0; i < nb_teams; i++) {
            init_by_team(server, &server->args->teams[i]);
    }
    if (server->eggs == NULL) {
        fprintf(stderr, "Failed to initialize eggs list.\n");
        return 84;
    }
    return 0;
}
