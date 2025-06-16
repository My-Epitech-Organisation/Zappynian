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
    server_args_t *args = connection->args;
    const char *team_name = client->read_buffer;
    team_t *team = NULL;

    if (strcmp(client->read_buffer, "GRAPHIC") == 0) {
        client->type = CLIENT_GUI;
        client->team_name = strdup("GRAPHIC");
        printf("Client %d is a GUI client.\n", client->fd);
        return;
    }
    team = get_team_by_name(args, team_name);
    if (team && team->current_players < team->max_slots) {
        client->type = CLIENT_IA;
        client->team_name = strdup(team_name);
        team->current_players++;
        printf("Client %d is an IA client %s.\n", client->fd, team_name);
    } else
        disconnect_client(connection, idx);
}

void handle_client_read(server_connection_t *connection, int idx)
{
    client_t *client = connection->clients[idx];
    char tmp_buffer[1024] = {0};
    ssize_t bytes_read = read(client->fd, tmp_buffer, sizeof(tmp_buffer));

    if (check_correct_read(connection, idx, bytes_read, client) == 84)
        return;
    if (memcpy(client->read_buffer + client->read_index, tmp_buffer,
        bytes_read) == NULL)
        return perror("memcpy");
    client->read_index += bytes_read;
    for (int i = 0; i < client->read_index; i++) {
        if (client->read_buffer[i] == '\n') {
            client->read_buffer[i] = '\0';
            assign_client_type(client, connection, idx);
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
    close(client->fd);
    free(client->team_name);
    free(client);
    for (int i = client_idx; i < connection->client_count - 1; i++)
        connection->clients[i] = connection->clients[i + 1];
    for (int i = client_idx; i < connection->nfds; i++)
        connection->fds[i - 1] = connection->fds[i];
    connection->client_count--;
    connection->nfds--;
}
