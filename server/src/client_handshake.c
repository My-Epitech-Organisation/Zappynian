/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** Client handshake management
*/

#include "../include/server.h"

static int receive_team_name_async(client_t *client, char *team_name,
    size_t team_name_size)
{
    char *received_line = NULL;

    received_line = zn_receive_message(client->zn_sock);
    if (received_line == NULL) {
        return -1;
    }
    if (strlen(received_line) >= team_name_size) {
        printf("[ERROR] Team name too long: %s\n", received_line);
        free(received_line);
        return -2;
    }
    strcpy(team_name, received_line);
    printf("[DEBUG] Received team name: %s\n", received_line);
    if (strcmp(received_line, "GRAPHIC") == 0) {
        free(received_line);
        return ZN_ROLE_GUI;
    }
    free(received_line);
    return ZN_ROLE_AI;
}

int setup_client_handshake(client_t *client, server_connection_t *connection,
    int idx, char *team_name)
{
    int role;

    // The WELCOME message has already been sent in accept_client()
    // Now we just wait for the team name
    role = receive_team_name_async(client, team_name, 256);
    if (role == -1) {
        return -1;  // No message available yet, try again later
    }
    if (role == -2) {
        disconnect_client(connection, idx);
        return -1;
    }

    client->type = (client_type_t)role;
    printf("[DEBUG] Client %d handshake successful, role: %d\n", idx, role);
    return 0;
}
