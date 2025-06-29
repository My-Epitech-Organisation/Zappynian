/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** commands
*/

#ifndef COMMANDS_H
    #define COMMANDS_H
    #define MAX_VISION_BUFFER 8192

    #include <stdbool.h>
    #include "player.h"
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>

struct server_s;
typedef struct server_s server_t;
struct client_s;
typedef struct client_s client_t;

typedef struct command_s {
    char *name;
    int duration;
    void (*execute)(player_t *player, server_t *server);
} command_t;

bool commands_add(player_t *player, const char *command_name);
void process_commands(server_t *server);
char *get_player_current_command(player_t *player);
char *get_command_argument(player_t *player);

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

char *get_player_vision(player_t *player, map_t *map);
void add_single_player(char *result, bool *first_item);
void add_current_tile_content(char *result, tile_t *tile, player_t *self,
    bool *first_item);
void add_vision_tile_content(char *result, tile_t *tile, player_t *self,
    bool *first_item);
void add_all_vision_tiles(char *result, player_t *player, map_t *map,
    bool *first_tile);
void add_tile(char *result, tile_t *tile, player_t *self, bool *first_tile);
void add_vision_tile(char *result, tile_t *tile, player_t *self,
    bool *first_tile);
void calculate_vision_coordinates(player_t *player, int distance,
    int offset, int *pos);
void add_current_tile_to_vision(char *result, player_t *player,
    map_t *map, bool *first_tile);
void add_all_vision_tiles(char *result, player_t *player, map_t *map,
    bool *first_tile);
void add_separator(char *result, bool *first_tile);
void add_resources_to_result(char *result, tile_t *tile,
    bool *first_item);
void add_single_player(char *result, bool *first_item);
void add_vision_tile_players(char *result, tile_t *tile,
    player_t *self, bool *first_item);

char *get_player_inventory(player_t *player);

char *get_broadcast_message(player_t *player);
void broadcast_to_all_players(player_t *sender, server_t *server,
    const char *message);

int make_player_array(tile_t *current_tile, player_t *player,
    server_t *server, client_t *ejecting_client);
client_t *find_client_by_player(server_t *server, player_t *player);
size_t get_nb_player(tile_t *tile, player_t *player);

void manage_food(server_t *server, player_t *player, client_t *client,
    int resource_type);
void manage_stones(server_t *server, player_t *player, client_t *client,
    int resource_type);
void add_resources(player_t *player, int resource_type, server_t *server,
    client_t *client);
void calculate_vision_coordinates(player_t *player, int distance, int offset,
    int *pos);
bool safe_strcat(char *dest, const char *src, size_t dest_size);

#endif // COMMANDS_H
