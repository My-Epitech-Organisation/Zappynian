/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** egg
*/

#ifndef EGG_H
    #define EGG_H

    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <stdbool.h>
    #include "server.h"

typedef struct egg_s {
    int x;
    int y;
    char *team_name;
    int id;
    bool is_hatched;
    struct egg_s *next;
} egg_t;

egg_t *create_egg(int x, int y, char *team_name);
void free_egg(egg_t *egg);
void hatch_egg(egg_t *egg);
void add_egg_to_server(server_t *server, egg_t *egg);

#endif // EGG_H
