/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** Command timer management
*/

#ifndef COMMAND_TIMER_H
    #define COMMAND_TIMER_H

    #include <stdbool.h>

typedef struct player_s player_t;
typedef struct server_s server_t;

/* Command timer functions */
void command_timer_init(player_t *player);
bool command_timer_add(player_t *player, int duration);
void command_timer_tick(player_t *player);
bool command_timer_is_ready(player_t *player);
void command_timer_execute_ready(player_t *player, server_t *server);

/* Command timer utilities */
int get_command_duration(const char *command_name);
bool has_pending_commands(player_t *player);

#endif /* COMMAND_TIMER_H */
