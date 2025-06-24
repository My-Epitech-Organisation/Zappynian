/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** Client handshake management
*/

#include "../include/server.h"

static int send_welcome_message(client_t *client, int idx)
{
    if (zn_send_welcome(client->zn_sock) != 0) {
        return -1;
    }
    if (zn_flush(client->zn_sock) != 0) {
        return -1;
    }
    printf("[DEBUG] WELCOME sent to client %d\n", idx);
    return 0;
}

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

static int handle_welcome_phase(client_t *client, int idx, int *welcome_sent)
{
    if (!welcome_sent[idx]) {
        if (send_welcome_message(client, idx) != 0) {
            return -1;
        }
        welcome_sent[idx] = 1;
        return -1;
    }
    return 0;
}

int setup_client_handshake(client_t *client, server_connection_t *connection,
    int idx, char *team_name)
{
    int role;
    static int welcome_sent[MAX_CLIENTS] = {0};

    if (handle_welcome_phase(client, idx, welcome_sent) != 0) {
        return -1;
    }
    role = receive_team_name_async(client, team_name, 256);
    if (role == -1) {
        return -1;
    }
    if (role == -2) {
        disconnect_client(connection, idx);
        return -1;
    }
    welcome_sent[idx] = 0;
    client->type = (client_type_t)role;
    printf("[DEBUG] Client %d handshake successful, role: %d\n", idx, role);
    return 0;
}
