/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** client_manager
*/

#include "../include/server.h"
#include "../include/team.h"

client_event_t assign_client_type(client_t *client,
    server_t *server, int idx)
{
    char team_name[256];
    client_event_t event;

    event = setup_client_handshake(client, server->connection, idx, team_name);
    if (event == CLIENT_EVENT_PENDING || event == CLIENT_EVENT_ERROR)
        return event;
    if (validate_and_respond(client, server, idx, team_name) == -1)
        return CLIENT_EVENT_ERROR;
    return event;
}

void catch_command(char *line, client_t *client,
    server_connection_t *connection)
{
    player_t *player = NULL;

    if (client->type == CLIENT_IA && line[0] != '\0') {
        player = find_player_by_client(connection, client);
        if (player != NULL) {
            printf("[DEBUG] Command from player %d: %s\n", player->id, line);
            player_found(player, line, client);
        }
    }
}

client_event_t handle_client_read(server_t *server, int idx)
{
    client_t *client = server->connection->clients[idx];
    char *line = NULL;
    client_event_t event;

    if (client->type == CLIENT_UNKNOWN) {
        event = assign_client_type(client, server, idx);
        return event;
    }
    line = zn_receive_message(client->zn_sock);
    if (line == NULL) {
        if (errno != EAGAIN && errno != EWOULDBLOCK) {
            disconnect_client(server->connection, idx);
            return CLIENT_EVENT_DISCONNECTED;
        }
        return CLIENT_EVENT_NONE;
    }
    catch_command(line, client, server->connection);
    free(line);
    return CLIENT_EVENT_NONE;
}

void disconnect(client_t *client, server_args_t *args)
{
    team_t *team = NULL;

    team = get_team_by_name(args, client->team_name);
    if (team && team->current_players > 0) {
        team->current_players--;
        printf("Client disconnected from team %s.\n", team->name);
    }
}

void disconnect_client(server_connection_t *connection, int client_idx)
{
    client_t *client = NULL;
    server_args_t *args = NULL;

    if (connection == NULL)
        return;
    client = connection->clients[client_idx];
    if (client == NULL)
        return;
    args = connection->args;
    if (args == NULL)
        return;
    if (client->type == CLIENT_IA && client->team_name)
        disconnect(client, args);
    if (client->zn_sock != NULL)
        zn_close(client->zn_sock);
    free(client->team_name);
    free(client);
    connection->clients[client_idx] = NULL;
    if (client_idx == connection->client_count - 1)
        connection->client_count--;
}
