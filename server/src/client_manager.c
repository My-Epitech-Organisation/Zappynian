/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** client_manager
*/

#include "../includes/server.h"
#include "../includes/team.h"

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

void assign_client_type(client_t *client, server_connection_t *connection,
    int idx)
{
    server_args_t *args = connection->args;
    char team_name[256];
    zn_role_t role;

    if (zn_send_welcome(client->zn_sock) != 0) {
        disconnect_client(connection, idx);
        return;
    }
    role = zn_receive_team_name(client->zn_sock, team_name, sizeof(team_name));
    if (role == ZN_ROLE_UNKNOWN) {
        disconnect_client(connection, idx);
        return;
    }
    client->type = (client_type_t)role;
    if (client->type == CLIENT_IA &&
        validate_team_assignment(args, team_name) == -1) {
        disconnect_client(connection, idx);
        return;
    }
    if (send_handshake_response(client, args, team_name) != 0) {
        disconnect_client(connection, idx);
        return;
    }
    if (client->type == CLIENT_IA) {
        team_t *team = get_team_by_name(args, team_name);
        team->current_players++;
        client->team_name = strdup(team_name);
        printf("Client %d is an IA client %s.\n", client->fd, team_name);
    } else {
        client->team_name = strdup("GRAPHIC");
        printf("Client %d is a GUI client.\n", client->fd);
    }
}

void handle_client_read(server_connection_t *connection, int idx)
{
    client_t *client = connection->clients[idx];
    char tmp_buffer[1024] = {0};
    ssize_t bytes_read = read(client->fd, tmp_buffer, sizeof(tmp_buffer));

    if (check_correct_read(connection, idx, bytes_read, client) == 84)
        return;
    if (client->type == CLIENT_UNKNOWN) {
        return;
    }
    if (memcpy(client->read_buffer + client->read_index, tmp_buffer,
        bytes_read) == NULL)
        return perror("memcpy");
    client->read_index += bytes_read;
    for (int i = 0; i < client->read_index; i++) {
        if (client->read_buffer[i] == '\n') {
            client->read_buffer[i] = '\0';
            printf("Command from client %d: %s\n", client->fd,
                client->read_buffer);
            client->read_index = 0;
            break;
        }
    }
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
            printf("Client %d disco from team %s.\n", client->fd, team->name);
        }
    }
    if (client->zn_sock != NULL) {
        zn_close(client->zn_sock);
    } else if (client->fd != -1) {
        close(client->fd);
    }
    free(client->team_name);
    free(client);
    connection->clients[client_idx] = NULL;
    if (client_idx == connection->client_count - 1) {
        connection->client_count--;
    }
}
