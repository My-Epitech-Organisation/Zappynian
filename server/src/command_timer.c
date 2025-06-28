/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** Command timer implementation
*/

#include "../include/command_timer.h"
#include "../include/player.h"
#include "../include/server.h"

void command_timer_init(player_t *player)
{
    int i;

    if (!player)
        return;
    player->command_count = 0;
    for (i = 0; i < 10; i++) {
        player->commands[i] = NULL;
        player->command_timers[i] = 0;
    }
}

bool command_timer_add(player_t *player, int duration)
{
    if (!player || player->command_count >= 10)
        return false;
    player->command_timers[player->command_count] = duration;
    player->command_count++;
    return true;
}

void command_timer_tick(player_t *player)
{
    if (!player || player->command_count == 0)
        return;
    if (player->command_timers[0] > 0)
        player->command_timers[0]--;
}

bool command_timer_is_ready(player_t *player)
{
    if (!player || player->command_count == 0)
        return false;
    return player->command_timers[0] == 0;
}

void command_timer_execute_ready(player_t *player, server_t *server)
{
    int i;

    if (!command_timer_is_ready(player))
        return;
    if (player->commands[0])
        player->commands[0]->execute(player, server);
    for (i = 1; i < player->command_count; i++) {
        player->commands[i - 1] = player->commands[i];
        player->command_timers[i - 1] = player->command_timers[i];
    }
    player->command_count--;
}

bool has_pending_commands(player_t *player)
{
    if (!player)
        return false;
    return player->command_count > 0;
}
