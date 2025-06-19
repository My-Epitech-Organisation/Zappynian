/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** player
*/

#ifndef PLAYER_H
    #define PLAYER_H

    #include <stdbool.h>
    #include "world.h"
    #include "resource.h"

    #define MAX_PLAYER_COMMANDS 10
    #define NORTH 0
    #define EAST 1
    #define SOUTH 2
    #define WEST 3

typedef struct command_s command_t;
typedef struct server_s server_t;

typedef struct player_s {
    int id;
    char *team_name;
    int x;
    int y;
    int level;
    int orientation;
    int food;
    int resources[RESOURCE_COUNT];
    bool dead;
    bool in_elevation;
    int slot_id;
    const command_t *commands[MAX_PLAYER_COMMANDS];
    int command_timers[MAX_PLAYER_COMMANDS];
    int command_count;
} player_t;

player_t *create_player(int id, const char *team_name, int x, int y);
void destroy_player(player_t *player);
void move_player(player_t *player, int new_x, int new_y, map_t *map);
bool player_decrement_food(player_t *player);
bool decrement_food_for_all_players(server_t *server);

#endif // PLAYER_H
