/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** Server player management functions
*/

#include "../include/server.h"
#include "../include/player.h"
#include "../include/team.h"

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

player_t *create_player_for_client(server_t *server, client_t *client,
    team_t *team, int client_id)
{
    player_t *player = NULL;
    int x = rand() % (int)server->args->width;
    int y = rand() % (int)server->args->height;
    int slot_assigned = 0;

    player = create_player(client_id, client->team_name, x, y);
    if (player == NULL)
        return NULL;
    player->slot_id = client_id;
    if (add_player_to_server(server, player) == -1) {
        destroy_player(player);
        return NULL;
    }
    for (int i = 0; i < team->max_slots; i++) {
        if (team->players[i] == NULL) {
            team->players[i] = player;
            slot_assigned = 1;
            break;
        }
    }
    if (!slot_assigned) {
        remove_player_from_server(server, player);
        destroy_player(player);
        return NULL;
    }
    team->current_players++;
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
