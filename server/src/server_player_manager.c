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

static int add_player_to_server(server_t *server, player_t *player)
{
    int slot = find_free_player_slot(server);

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

static void assign_player_to_team(team_t *team, player_t *player)
{
    int i;

    for (i = 0; i < team->max_slots; i++) {
        if (team->players[i] == NULL) {
            team->players[i] = player;
            break;
        }
    }
    team->current_players++;
    team->remaining_slots--;
}

static player_t *setup_player_for_client(server_t *server, client_t *client)
{
    player_t *player = NULL;
    int x = rand() % (int)server->args->width;
    int y = rand() % (int)server->args->height;

    player = create_player(client->id, client->team_name, x, y);
    if (player == NULL)
        return NULL;
    player->slot_id = client->id;
    if (add_player_to_server(server, player) == -1) {
        destroy_player(player);
        return NULL;
    }
    return player;
}

player_t *create_player_for_client(server_t *server, client_t *client,
    team_t *team)
{
    player_t *player = NULL;

    if (team->remaining_slots <= 0)
        return NULL;
    player = setup_player_for_client(server, client);
    if (player == NULL)
        return NULL;
    assign_player_to_team(team, player);
    return player;
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
