/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** catch_commands_from_clients
*/

#include "../include/server.h"
#include "../include/team.h"

player_t *find_player_by_client(server_connection_t *connection,
    client_t *client)
{
    server_args_t *args = connection->args;
    team_t *team = get_team_by_name(args, client->team_name);
    player_t *player = NULL;

    if (team == NULL)
        return NULL;
    for (int i = 0; i < args->clients_per_team; i++) {
        if (team->players[i] != NULL) {
            player = team->players[i];
            break;
        }
    }
    if (player == NULL)
        fprintf(stderr, "Player not found for client.\n");
    return player;
}

void player_found(player_t *player, const char *line, client_t *client)
{
    char *newline = NULL;

    newline = strchr(line, '\n');
    if (newline != NULL)
        *newline = '\0';
    if (!commands_add(player, line))
        zn_send_message(client->zn_sock, "ko");
}
