/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** player
*/

#include "../includes/player.h"
#include "../includes/server.h"
#include "../includes/commands.h"

player_t *create_player(int id, const char *team_name, int x, int y)
{
    player_t *player = malloc(sizeof(player_t));

    if (player == NULL) {
        fprintf(stderr, "Memory allocation failed for player.\n");
        return NULL;
    }
    player->id = id;
    player->team_name = strdup(team_name);
    player->x = x;
    player->y = y;
    player->level = 1;
    player->orientation = 0;
    player->food = 10;
    player->dead = false;
    player->in_elevation = false;
    player->slot_id = -1;
    return player;
}

void destroy_player(player_t *player)
{
    if (player == NULL)
        return;
    free(player->team_name);
    free(player);
}

void move_player(player_t *player, int x, int y, map_t *map)
{
    if (player == NULL || map == NULL)
        return;
    if (x < 0 || (size_t)x >= map->width || y < 0 ||
        (size_t)y >= map->height) {
        fprintf(stderr, "Move out of bounds: (%d, %d)\n", x, y);
        return;
    }
    player->x = x;
    player->y = y;
}

bool player_decrement_food(player_t *player)
{
    if (player == NULL)
        return false;
    if (player->food > 0) {
        player->food--;
        return true;
    } else {
        player->dead = true;
        return false;
    }
}

void decrement_food_for_all_players(void)
{
    server_t *server = get_server_instance();

    if (!server)
        return;
    for (size_t i = 0; i < server->player_count; i++) {
        if (server->players[i]) {
            player_decrement_food(server->players[i]);
        }
    }
}

void check_player_deaths(void)
{
    server_t *server = get_server_instance();
    player_t *player;

    if (!server)
        return;
    for (size_t i = 0; i < server->player_count; i++) {
        player = server->players[i];
        if (player && !player->dead && player->food <= 0) {
            player->dead = true;
        }
    }
}

void execute_ready_commands(void)
{
    server_t *server = get_server_instance();

    if (!server)
        return;
    for (size_t i = 0; i < server->player_count; i++) {
        if (server->players[i] && !server->players[i]->dead) {
            commands_execute_next(server->players[i]);
        }
    }
}
