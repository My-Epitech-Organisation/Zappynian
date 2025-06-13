/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** team
*/

#include "../includes/team.h"

team_t *teams[MAX_TEAMS];
int team_count = 0;

team_t *get_team_by_name(const char *name)
{
    for (int i = 0; i < team_count; i++) {
        if (strcmp(teams[i]->name, name) == 0)
            return teams[i];
    }
    return NULL;
}

void team_free_slot(const char *team_name)
{
    team_t *team = get_team_by_name(team_name);

    if (team) {
        team->remaining_slots += 1;
        printf("✅ Slot libéré pour l'équipe %s. Slots restants : %d\n",
            team_name, team->remaining_slots);
    }
}
