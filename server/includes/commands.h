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

typedef struct command_s {
    char *name;
    int duration;
    void (*execute)(player_t *player);
} command_t;

bool commands_add(player_t *player, const char *command_name);
void commands_execute_next(player_t *player);
void cmd_forward(player_t *player);
void cmd_right(player_t *player);
void cmd_left(player_t *player);
void cmd_look(player_t *player);
void cmd_inventory(player_t *player);
void cmd_broadcast(player_t *player);
void cmd_connect_nbr(player_t *player);
void cmd_fork(player_t *player);
void cmd_eject(player_t *player);
void cmd_take(player_t *player);
void cmd_set(player_t *player);
void cmd_incantation(player_t *player);

const command_t *get_command_table(void);
size_t get_command_table_size(void);

#endif // COMMANDS_H
