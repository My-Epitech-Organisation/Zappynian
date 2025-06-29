/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** Server player management functions
*/

#include "../include/server.h"
#include "../include/player.h"
#include "../include/team.h"
#include "../include/food_timer.h"

static int find_free_player_slot(server_t *server)
{
    for (size_t i = 0; i < server->player_count; i++) {
        if (server->players[i] == NULL)
            return i;
    }
    return -1;
}

int add_player_to_server(server_t *server, player_t *player)
{
    int slot = 0;

    if (player == NULL || server == NULL || server->players == NULL) {
        return -1;
    }
    slot = find_free_player_slot(server);
    if (slot == -1)
        return -1;
    server->players[slot] = player;
    return 0;
}

int initialize_server_players(server_t *server)
{
    size_t max_players = server->args->team_count *
        server->args->clients_per_team;

    server->players = calloc(max_players, sizeof(player_t *));
    if (server->players == NULL)
        return -1;
    server->player_count = max_players;
    return 0;
}

static void get_random_position(server_t *server, int *x, int *y)
{
    *x = rand() % (int)server->args->width;
    *y = rand() % (int)server->args->height;
}

static int assign_player_to_team_slot(team_t *team, player_t *player)
{
    int i = 0;

    for (i = 0; i < team->max_slots; i++) {
        if (team->players[i] == NULL) {
            team->players[i] = player;
            team->current_players++;
            return 1;
        }
    }
    return 0;
}

player_t *create_player_for_client(server_t *server, client_t *client,
    team_t *team, int client_id)
{
    player_t *player = NULL;
    int x = 0;
    int y = 0;

    get_random_position(server, &x, &y);
    player = create_player(client_id, client->team_name, x, y);
    if (player == NULL)
        return NULL;
    player->slot_id = client_id;
    if (add_player_to_server(server, player) == -1) {
        destroy_player(player);
        return NULL;
    }
    if (!assign_player_to_team_slot(team, player)) {
        remove_player_from_server(server, player);
        destroy_player(player);
        return NULL;
    }
    return player;
}

void remove_player_from_server(server_t *server, player_t *player)
{
    for (size_t i = 0; i < server->player_count; i++) {
        if (server->players[i] == player) {
            server->players[i] = NULL;
            break;
        }
    }
}

void remove_player_from_team(team_t *team, player_t *player)
{
    if (!team)
        return;
    for (int i = 0; i < team->max_slots; i++) {
        if (team->players[i] == player) {
            team->players[i] = NULL;
            break;
        }
    }
    if (team->current_players > 0)
        team->current_players--;
}

void process_food_consumption(server_t *server, void *food_timer)
{
    size_t i;
    food_timer_t *ft = (food_timer_t *)food_timer;

    if (!server || !ft || !food_timer_should_consume(ft))
        return;
    for (i = 0; i < server->player_count; i++) {
        if (server->players[i] && !server->players[i]->dead)
            player_decrement_food(server->players[i]);
    }
    food_timer_reset(ft);
}
