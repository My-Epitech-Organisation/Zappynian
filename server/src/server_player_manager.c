/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** Server player management functions
*/

#include "../include/server.h"

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

static int generate_random_position(server_t *server, int *x, int *y)
{
    *x = rand() % (int)server->args->width;
    *y = rand() % (int)server->args->height;
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

player_t *create_player_for_client(server_t *server, client_t *client)
{
    player_t *player = NULL;
    int x;
    int y;

    if (generate_random_position(server, &x, &y) == -1)
        return NULL;
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
