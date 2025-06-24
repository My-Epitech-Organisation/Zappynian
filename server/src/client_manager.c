/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** client_manager
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

static int setup_client_handshake(client_t *client,
    server_connection_t *connection, int idx, char *team_name)
{
    zn_role_t role;

    if (zn_send_welcome(client->zn_sock) != 0) {
        disconnect_client(connection, idx);
        return -1;
    }
    role = zn_receive_team_name(client->zn_sock, team_name, sizeof(team_name));
    if (role == ZN_ROLE_UNKNOWN) {
        disconnect_client(connection, idx);
        return -1;
    }
    client->type = (client_type_t)role;
    return 0;
}

static int validate_and_respond(client_t *client,
    server_connection_t *connection, int idx, const char *team_name)
{
    server_args_t *args = connection->args;

    if (client->type == CLIENT_IA &&
        validate_team_assignment(args, team_name) == -1) {
        disconnect_client(connection, idx);
        return -1;
    }
    if (send_handshake_response(client, args, team_name) != 0) {
        disconnect_client(connection, idx);
        return -1;
    }
    return 0;
}

static void finalize_client_assignment(client_t *client,
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

void assign_client_type(client_t *client, server_connection_t *connection,
    int idx)
{
    char team_name[256];

    if (setup_client_handshake(client, connection, idx, team_name) == -1)
        return;
    if (validate_and_respond(client, connection, idx, team_name) == -1)
        return;
    finalize_client_assignment(client, connection, team_name);
}

void catch_command(char *line, client_t *client,
    server_connection_t *connection)
{
    player_t *player = NULL;

    if (client->type == CLIENT_IA && line[0] != '\0') {
        player = find_player_by_client(connection, client);
        if (player != NULL) {
            player_found(player, line, client);
        }
    }
}

void handle_client_read(server_connection_t *connection, int idx)
{
    client_t *client = connection->clients[idx];
    char *line = NULL;

    if (client->type == CLIENT_UNKNOWN)
        return;
    line = zn_receive_message(client->zn_sock);
    if (line == NULL) {
        if (errno != EAGAIN && errno != EWOULDBLOCK)
            disconnect_client(connection, idx);
        return;
    }
    printf("Command received: %s\n", line);
    catch_command(line, client, connection);
    free(line);
}

void disconnect_client(server_connection_t *connection, int client_idx)
{
    client_t *client = connection->clients[client_idx];
    server_args_t *args = connection->args;
    team_t *team = NULL;

    if (client->type == CLIENT_IA && client->team_name) {
        team = get_team_by_name(args, client->team_name);
        if (team && team->current_players > 0) {
            team->current_players--;
            printf("Client disconnected from team %s.\n", team->name);
        }
    }
    if (client->zn_sock != NULL) {
        zn_close(client->zn_sock);
    }
    free(client->team_name);
    free(client);
    connection->clients[client_idx] = NULL;
    if (client_idx == connection->client_count - 1) {
        connection->client_count--;
    }
}
