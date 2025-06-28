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

    event = setup_client_handshake(client, server, idx, team_name);
    if (event == CLIENT_EVENT_PENDING || event == CLIENT_EVENT_ERROR)
        return event;
    if (validate_and_respond(client, server, idx, team_name) == -1)
        return CLIENT_EVENT_ERROR;
    return event;
}

static void process_ia_command(char *line, client_t *client,
    server_connection_t *connection)
{
    player_t *player = find_player_by_client(connection, client);

    if (player == NULL || player->command_count >= MAX_PLAYER_COMMANDS)
        return;
    player_found(player, line, client);
}

void catch_command(char *line, client_t *client,
    server_connection_t *connection)
{
    if (client->type == CLIENT_IA && line[0] != '\0') {
        process_ia_command(line, client, connection);
        return;
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
            disconnect_client(server, idx);
            return CLIENT_EVENT_DISCONNECTED;
        }
        return CLIENT_EVENT_NONE;
    }
    catch_command(line, client, server->connection);
    free(line);
    return CLIENT_EVENT_NONE;
}

static void cleanup_ia_player(server_t *server, client_t *client)
{
    player_t *player = find_player_by_client(server->connection, client);
    tile_t *tile;
    team_t *team;

    if (!player)
        return;
    tile = get_tile_toroidal(server->map, player->x, player->y);
    if (tile)
        remove_player_from_tile(tile, player);
    remove_player_from_server(server, player);
    team = get_team_by_name(server->connection->args, client->team_name);
    remove_player_from_team(team, player);
    send_pdi(server, player);
    destroy_player(player);
}

static void cleanup_client_resources(server_connection_t *connection,
    int client_idx)
{
    client_t *client = connection->clients[client_idx];

    if (client->zn_sock != NULL)
        zn_close(client->zn_sock);
    free(client->team_name);
    free(client);
    connection->clients[client_idx] = NULL;
    if (client_idx == connection->client_count - 1)
        connection->client_count--;
}

void disconnect_client(server_t *server, int client_idx)
{
    client_t *client = server->connection->clients[client_idx];

    if (!client)
        return;
    if (client->type == CLIENT_IA)
        cleanup_ia_player(server, client);
    cleanup_client_resources(server->connection, client_idx);
}
