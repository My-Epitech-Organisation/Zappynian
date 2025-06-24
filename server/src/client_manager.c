/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** client_manager
*/

#include "../include/server.h"
#include "../include/team.h"

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

    if (client->type == CLIENT_UNKNOWN) {
        assign_client_type(client, connection, idx);
        return;
    }
    line = zn_receive_message(client->zn_sock);
    if (line == NULL) {
        if (errno != EAGAIN && errno != EWOULDBLOCK) {
            disconnect_client(connection, idx);
        }
        return;
    }
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
