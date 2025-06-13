/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Zappy Network Library - Handshake implementation
*/

#include "zappy_net.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

static int send_message(zn_socket_t sock, const char *message);
static char *receive_line(zn_socket_t sock);

int zn_perform_handshake(zn_socket_t sock, zn_role_t role,
    const char *team_name, zn_handshake_result_t *result)
{
    char *welcome_msg = NULL;
    char *client_num_msg = NULL;
    char *world_size_msg = NULL;
    const char *send_name = NULL;

    if (sock == NULL || result == NULL) {
        return -1;
    }
    if (role == ZN_ROLE_AI && team_name == NULL) {
        return -1;
    }
    welcome_msg = receive_line(sock);
    if (welcome_msg == NULL) {
        return -1;
    }
    if (strcmp(welcome_msg, "WELCOME") != 0) {
        free(welcome_msg);
        return -1;
    }
    free(welcome_msg);
    send_name = (role == ZN_ROLE_GUI) ? "GRAPHIC" : team_name;
    if (send_message(sock, send_name) == -1) {
        return -1;
    }
    client_num_msg = receive_line(sock);
    if (client_num_msg == NULL) {
        return -1;
    }
    result->client_num = atoi(client_num_msg);
    free(client_num_msg);
    world_size_msg = receive_line(sock);
    if (world_size_msg == NULL) {
        result->world_x = 0;
        result->world_y = 0;
        return 0;
    }
    if (sscanf(world_size_msg, "%d %d", &result->world_x,
        &result->world_y) != 2) {
        free(world_size_msg);
        return -1;
    }
    free(world_size_msg);
    return 0;
}

int zn_send_welcome(zn_socket_t sock)
{
    if (sock == NULL) {
        return -1;
    }
    return send_message(sock, "WELCOME");
}

zn_role_t zn_receive_team_name(zn_socket_t sock, char *team_name,
    size_t team_name_size)
{
    char *received_line = NULL;

    if (sock == NULL || team_name == NULL || team_name_size == 0) {
        return ZN_ROLE_UNKNOWN;
    }
    received_line = receive_line(sock);
    if (received_line == NULL) {
        return ZN_ROLE_UNKNOWN;
    }
    if (strlen(received_line) >= team_name_size) {
        free(received_line);
        return ZN_ROLE_UNKNOWN;
    }
    strcpy(team_name, received_line);
    if (strcmp(received_line, "GRAPHIC") == 0) {
        free(received_line);
        return ZN_ROLE_GUI;
    }
    free(received_line);
    return ZN_ROLE_AI;
}

int zn_send_handshake_response(zn_socket_t sock, int client_num,
    int world_x, int world_y)
{
    char response[256];
    int ret = 0;

    if (sock == NULL) {
        return -1;
    }
    ret = snprintf(response, sizeof(response), "%d", client_num);
    if (ret < 0 || (size_t)ret >= sizeof(response)) {
        return -1;
    }
    if (send_message(sock, response) == -1) {
        return -1;
    }
    ret = snprintf(response, sizeof(response), "%d %d", world_x, world_y);
    if (ret < 0 || (size_t)ret >= sizeof(response)) {
        return -1;
    }
    return send_message(sock, response);
}

static int send_message(zn_socket_t sock, const char *message)
{
    size_t message_len = 0;
    ssize_t written = 0;

    if (sock == NULL || message == NULL) {
        return -1;
    }
    message_len = strlen(message);
    written = zn_write(sock, message, message_len);
    if (written != (ssize_t)message_len) {
        return -1;
    }
    written = zn_write(sock, "\n", 1);
    if (written != 1) {
        return -1;
    }
    if (zn_flush(sock) == -1) {
        return -1;
    }
    return 0;
}

static char *receive_line(zn_socket_t sock)
{
    char *line = NULL;

    if (sock == NULL) {
        return NULL;
    }
    line = zn_readline(sock);
    return line;
}
