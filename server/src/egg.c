/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** egg
*/

#include "../include/egg.h"
#include "../include/world.h"
#include "../include/server.h"
#include "../include/team.h"

static int get_id(void)
{
    static int id = 0;
    static bool first = true;

    if (first) {
        id = 0;
        first = false;
        return id;
    }
    id++;
    return id;
}

egg_t *create_egg(int x, int y, char *team_name)
{
    egg_t *new_egg = malloc(sizeof(egg_t));

    if (new_egg == NULL)
        return NULL;
    new_egg->x = x;
    new_egg->y = y;
    new_egg->player_id = -1;
    new_egg->team_name = strdup(team_name);
    if (new_egg->team_name == NULL) {
        free(new_egg);
        return NULL;
    }
    new_egg->id = get_id();
    new_egg->is_hatched = false;
    new_egg->next = NULL;
    return new_egg;
}

void free_egg(egg_t *egg)
{
    if (egg == NULL)
        return;
    free(egg->team_name);
    free(egg);
}

void hatch_egg(egg_t *egg)
{
    if (egg == NULL)
        return;
    egg->is_hatched = true;
}

void add_egg_to_server(server_t *server, egg_t *egg)
{
    if (server == NULL || egg == NULL)
        return;
    if (server->eggs == NULL) {
        server->eggs = egg;
    } else {
        egg->next = server->eggs;
        server->eggs = egg;
    }
}

void destroy_eggs_at_position(int x, int y, server_t *server)
{
    egg_t **current;
    egg_t *to_delete;

    if (server == NULL)
        return;
    current = &server->eggs;
    while (*current != NULL) {
        if ((*current)->x == x && (*current)->y == y) {
            to_delete = *current;
            send_edi(server, to_delete->id);
            *current = (*current)->next;
            free_egg(to_delete);
        } else
            current = &(*current)->next;
    }
}
