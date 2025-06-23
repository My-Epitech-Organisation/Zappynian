/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** Core command management functions
*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "../include/commands.h"
#include "../include/server.h"

const command_t *get_command_table(void)
{
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

    return command_table;
}

size_t get_command_table_size(void)
{
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

    return sizeof(command_table) / sizeof(command_t);
}

static char *put_good_format(const char *command_name)
{
    char *formatted_command_name = strdup(command_name);
    char *start = NULL;
    char *end = NULL;
    char *trimmed_command_name = NULL;

    if (formatted_command_name == NULL)
        return NULL;
    for (size_t i = 0; formatted_command_name[i]; i++)
        formatted_command_name[i] = tolower(formatted_command_name[i]);
    start = formatted_command_name;
    while (*start == ' ')
        start++;
    end = formatted_command_name + strlen(formatted_command_name) - 1;
    while (end > start && *end == ' ')
        end--;
    *(end + 1) = '\0';
    trimmed_command_name = strdup(start);
    free(formatted_command_name);
    return trimmed_command_name;
}

bool commands_add(player_t *player, const char *command_name)
{
    const command_t *table = get_command_table();
    size_t i;
    char *formatted_command_name;

    if (player->command_count >= MAX_PLAYER_COMMANDS)
        return false;
    formatted_command_name = put_good_format(command_name);
    if (formatted_command_name == NULL)
        return false;
    for (i = 0; i < get_command_table_size(); i++) {
        if (strcmp(table[i].name, formatted_command_name) == 0) {
            player->commands[player->command_count] = &table[i];
            player->command_timers[player->command_count] = table[i].duration;
            player->command_count++;
            free(formatted_command_name);
            return true;
        }
    }
    free(formatted_command_name);
    return false;
}

static void commands_execute_next(player_t *player, server_t *server)
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

char *get_player_current_command(player_t *player)
{
    if (player->command_count == 0)
        return NULL;
    return player->commands[0]->name;
}
