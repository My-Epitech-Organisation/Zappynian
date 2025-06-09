/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** client_manager
*/

#include "../includes/server.h"

void assign_client_type(client_t *client)
{
    if (strcmp(client->read_buffer, "GRAPHIC") == 0) {
        client->type = CLIENT_GUI;
        client->team_name = strdup("GRAPHIC TEAM");
        printf("Client %d is a GUI client.\n", client->fd);
    } else if (strcmp(client->read_buffer, "IA") == 0) {
        client->type = CLIENT_IA;
        client->team_name = strdup("IA TEAM");
        printf("Client %d is an IA client.\n", client->fd);
    } else {
        client->type = CLIENT_UNKNOWN;
    }
}

void handle_client_read(server_connection_t *connection, int idx)
{
    client_t *client = connection->clients[idx];
    char tmp_buffer[1024] = {0};
    ssize_t bytes_read = read(client->fd, tmp_buffer, sizeof(tmp_buffer));

    if (check_correct_read(connection, idx, bytes_read, client) == 84)
        return;
    memcpy(client->read_buffer + client->read_index, tmp_buffer, bytes_read);
    client->read_index += bytes_read;
    for (int i = 0; i < client->read_index; i++) {
        if (client->read_buffer[i] == '\n') {
            client->read_buffer[i] = '\0';
            assign_client_type(client);
            client->read_index = 0;
            break;
        }
    }
}

void disconnect_client(server_connection_t *connection, int client_idx)
{
    close(connection->clients[client_idx]->fd);
    free(connection->clients[client_idx]);
    for (int i = client_idx; i < connection->client_count - 1; i++)
        connection->clients[i] = connection->clients[i + 1];
    for (int i = client_idx; i < connection->nfds; i++)
        connection->fds[i - 1] = connection->fds[i];
    connection->client_count--;
    connection->nfds--;
}
