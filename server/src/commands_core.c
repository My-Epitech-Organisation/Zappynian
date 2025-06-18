/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** Core command management functions
*/

#include <stdio.h>
#include <string.h>
#include "../includes/commands.h"
#include "../includes/server.h"

static const command_t command_table[] = {
    {"forward", 7, cmd_forward},
    {"right", 7, cmd_right},
    {"left", 7, cmd_left},
    {"look", 7, cmd_look},
    {"inventory", 1, cmd_inventory},
    {"broadcast", 7, cmd_broadcast},
    {"connect_nbr", 0, cmd_connect_nbr},
    {"fork", 42, cmd_fork},
    {"eject", 7, cmd_eject},
    {"take", 7, cmd_take},
    {"set", 7, cmd_set},
    {"incantation", 300, cmd_incantation}
};

const command_t *get_command_table(void)
{
    return command_table;
}

size_t get_command_table_size(void)
{
    return sizeof(command_table) / sizeof(command_t);
}

bool commands_add(player_t *player, const char *command_name)
{
    const command_t *table = get_command_table();
    size_t table_size = get_command_table_size();
    size_t i;

    if (player->command_count >= MAX_PLAYER_COMMANDS)
        return false;
    for (i = 0; i < table_size; i++) {
        if (strcmp(table[i].name, command_name) == 0) {
            player->commands[player->command_count] = &table[i];
            player->command_timers[player->command_count] = table[i].duration;
            player->command_count++;
            return true;
        }
    }
    return false;
}

void commands_execute_next(player_t *player, server_t *server)
{
    if (player->command_count == 0)
        return;
    player->command_timers[0]--;
    if (player->command_timers[0] <= 0) {
        player->commands[0]->execute(player, server);
        for (int i = 1; i < player->command_count; i++) {
            player->commands[i - 1] = player->commands[i];
            player->command_timers[i - 1] = player->command_timers[i];
        }
        player->command_count--;
    }
}

void process_commands(server_t *server)
{
    player_t *player;
    size_t i;

    for (i = 0; i < server->player_count; i++) {
        player = server->players[i];
        if (player != NULL && !player->dead && player->command_count > 0) {
            commands_execute_next(player, server);
        }
    }
}
