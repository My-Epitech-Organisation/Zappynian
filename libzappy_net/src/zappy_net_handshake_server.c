/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Server-side handshake implementation
*/

#include "zappy_net.h"
#include "zappy_net_internal.h"

static int send_client_number(zn_socket_t sock, int client_num)
{
    char response[256];
    int ret = 0;

    ret = snprintf(response, sizeof(response), "%d", client_num);
    if (ret < 0 || (size_t)ret >= sizeof(response)) {
        return -1;
    }
    return zn_send_message(sock, response);
}

static int send_world_dimensions(zn_socket_t sock, int world_x, int world_y)
{
    char response[256];
    int ret = 0;

    ret = snprintf(response, sizeof(response), "%d %d", world_x, world_y);
    if (ret < 0 || (size_t)ret >= sizeof(response)) {
        return -1;
    }
    return zn_send_message(sock, response);
}

int zn_send_welcome(zn_socket_t sock)
{
    if (sock == NULL) {
        return -1;
    }
    return zn_send_message(sock, "WELCOME");
}

zn_role_t zn_receive_team_name(zn_socket_t sock, char *team_name,
    size_t team_name_size)
{
    char *received_line = NULL;

    if (sock == NULL || team_name == NULL || team_name_size == 0) {
        return ZN_ROLE_UNKNOWN;
    }
    received_line = zn_receive_message(sock);
    if (received_line == NULL) {
        return ZN_ROLE_UNKNOWN;
    }
    if (strlen(received_line) >= team_name_size) {
        free(received_line);
        return ZN_ROLE_UNKNOWN;
    }
    strcpy(team_name, received_line);
    printf("Received team name: %s\n", team_name);
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
    if (sock == NULL) {
        return -1;
    }
    if (send_client_number(sock, client_num) == -1) {
        return -1;
    }
    return send_world_dimensions(sock, world_x, world_y);
}
