/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** Client handshake response management
*/

#include "../include/server.h"
#include "../include/team.h"

static int get_available_slots(server_args_t *args, const char *team_name)
{
    team_t *team = get_team_by_name(args, team_name);

    if (team == NULL) {
        return 0;
    }
    return team->max_slots - team->current_players;
}

static int send_handshake_response(client_t *client, server_args_t *args,
    const char *team_name)
{
    int client_num = 0;

    if (client->type == CLIENT_IA) {
        client_num = get_available_slots(args, team_name);
    }
    return zn_send_handshake_response(client->zn_sock, client_num,
        (int)args->width, (int)args->height);
}

static int validate_team_assignment(server_args_t *args, const char *team_name)
{
    team_t *team = get_team_by_name(args, team_name);

    if (team == NULL || team->current_players >= team->max_slots) {
        return -1;
    }
    return 0;
}

int validate_and_respond(client_t *client, server_connection_t *connection,
    int idx, const char *team_name)
{
    server_args_t *args = connection->args;

    if (client->type == CLIENT_IA) {
        if (validate_team_assignment(args, team_name) == -1) {
            disconnect_client(connection, idx);
            return -1;
        }
    }
    if (send_handshake_response(client, args, team_name) != 0) {
        disconnect_client(connection, idx);
        return -1;
    }
    return 0;
}

void finalize_client_assignment(client_t *client,
    server_connection_t *connection, const char *team_name)
{
    server_args_t *args = connection->args;
    team_t *team;

    if (client->type == CLIENT_IA) {
        team = get_team_by_name(args, team_name);
        team->current_players++;
        client->team_name = strdup(team_name);
        printf("Client is an IA client: %s\n", team_name);
    } else {
        client->team_name = strdup("GRAPHIC");
        printf("Client is a GUI client.\n");
    }
}
