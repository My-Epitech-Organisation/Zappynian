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
    return team->remaining_slots;
}

static int send_handshake_response(client_t *client, server_t *server,
    const char *team_name)
{
    int client_num = 0;
    int ret = 0;

    if (client->type == CLIENT_IA) {
        client_num = get_available_slots(server->args, team_name);
        ret = zn_send_handshake_response(client->zn_sock, client_num,
        (int)server->args->width, (int)server->args->height);
    }
    if (client->type == CLIENT_GUI)
        ret = send_graphic_initial_state(client, server);
    return ret;
}

static int validate_team_assignment(server_args_t *args, const char *team_name)
{
    team_t *team = get_team_by_name(args, team_name);

    if (team == NULL || team->current_players >= team->max_slots) {
        return -1;
    }
    return 0;
}

int validate_and_respond(client_t *client, server_t *server,
    int idx, const char *team_name)
{
    server_args_t *args = server->connection->args;

    if (client->type == CLIENT_IA) {
        if (validate_team_assignment(args, team_name) == -1) {
            disconnect_client(server->connection, idx);
            return -1;
        }
    }
    if (send_handshake_response(client, server, team_name) != 0) {
        disconnect_client(server->connection, idx);
        return -1;
    }
    return 0;
}

int send_handshake_response_only(client_t *client, server_t *server,
    const char *team_name)
{
    return send_handshake_response(client, server, team_name);
}
