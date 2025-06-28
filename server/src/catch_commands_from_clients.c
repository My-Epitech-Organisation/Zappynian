/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** catch_commands_from_clients
*/

#include "../include/server.h"
#include "../include/team.h"

static int validate_client_params(server_connection_t *connection,
    client_t *client)
{
    server_args_t *args = connection->args;

    if (connection == NULL || client == NULL || args == NULL ||
        args->clients_per_team <= 0) {
        fprintf(stderr, "Invalid connection or client parameters.\n");
        return -1;
    }
    return 0;
}

static player_t *search_player_in_team(team_t *team, client_t *client,
    server_args_t *args)
{
    player_t *player = NULL;

    if (args == NULL || team == NULL || client == NULL)
        return NULL;
    for (int i = 0; i < args->clients_per_team; i++) {
        if (team->players[i] == NULL)
            continue;
        if (team->players[i]->slot_id == client->id) {
            player = team->players[i];
            break;
        }
    }
    if (player == NULL)
        fprintf(stderr, "Player not found for client.\n");
    return player;
}

player_t *find_player_by_client(server_connection_t *connection,
    client_t *client)
{
    server_args_t *args = connection->args;
    team_t *team = NULL;

    if (validate_client_params(connection, client) == -1)
        return NULL;
    team = get_team_by_name(args, client->team_name);
    if (team == NULL)
        return NULL;
    return search_player_in_team(team, client, args);
}

void player_found(player_t *player, const char *line, client_t *client)
{
    char *newline = NULL;

    newline = strchr(line, '\n');
    if (newline != NULL)
        *newline = '\0';
    if (!commands_add(player, line))
        zn_send_message(client->zn_sock, "ko");
    else
        printf("[DEBUG] Command received: %s from player %d of team %s\n",
            line, player->id, player->team_name);
    free(newline);
}
