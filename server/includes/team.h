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

typedef struct team_s {
    char *name;
    int max_slots;
    int remaining_slots;
} team_t;

extern team_t *teams[MAX_TEAMS];
extern int team_count;

team_t *get_team_by_name(const char *name);
void team_free_slot(const char *team_name);

#endif // TEAM_H
