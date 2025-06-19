/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** commands
*/

#ifndef COMMANDS_H
    #define COMMANDS_H

    #include <stdbool.h>
    #include "player.h"
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>

struct server_s;
typedef struct server_s server_t;

typedef struct command_s {
    char *name;
    int duration;
    void (*execute)(player_t *player, server_t *server);
} command_t;

bool commands_add(player_t *player, const char *command_name);
void process_commands(server_t *server);
char *get_player_current_command(player_t *player);

void cmd_forward(player_t *player, server_t *server);
void cmd_right(player_t *player, server_t *server);
void cmd_left(player_t *player, server_t *server);
void cmd_look(player_t *player, server_t *server);
void cmd_inventory(player_t *player, server_t *server);
void cmd_broadcast(player_t *player, server_t *server);
void cmd_connect_nbr(player_t *player, server_t *server);
void cmd_fork(player_t *player, server_t *server);
void cmd_eject(player_t *player, server_t *server);
void cmd_take(player_t *player, server_t *server);
void cmd_set(player_t *player, server_t *server);
void cmd_incantation(player_t *player, server_t *server);

const command_t *get_command_table(void);
size_t get_command_table_size(void);

char *get_player_vision(player_t *player, map_t *map);
void add_current_tile(char *result, player_t *player, map_t *map);
void add_other_tiles(char *result, player_t *player, map_t *map,
    int vision_range);
void add_resource_type(char *result, tile_t *tile, int resource_type,
    bool first);

char *get_player_inventory(player_t *player);

char *get_broadcast_message(player_t *player);
void broadcast_to_all_players(player_t *sender, server_t *server,
    const char *message);

#endif // COMMANDS_H
