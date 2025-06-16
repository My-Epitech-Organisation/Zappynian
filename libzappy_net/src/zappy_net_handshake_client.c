/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Client-side handshake implementation
*/

#include <limits.h>

#include "zappy_net.h"
#include "zappy_net_internal.h"

static int validate_handshake_params(zn_socket_t sock, zn_role_t role,
    const char *team_name, zn_handshake_result_t *result)
{
    if (sock == NULL || result == NULL) {
        return -1;
    }
    if (role == ZN_ROLE_AI && team_name == NULL) {
        return -1;
    }
    return 0;
}

static int process_welcome_message(zn_socket_t sock)
{
    char *welcome_msg = NULL;

    welcome_msg = zn_receive_message(sock);
    if (welcome_msg == NULL) {
        return -1;
    }
    if (strcmp(welcome_msg, "WELCOME") != 0) {
        free(welcome_msg);
        return -1;
    }
    free(welcome_msg);
    return 0;
}

static int send_team_name(zn_socket_t sock, zn_role_t role,
    const char *team_name)
{
    const char *send_name = NULL;

    send_name = (role == ZN_ROLE_GUI) ? "GRAPHIC" : team_name;
    return zn_send_message(sock, send_name);
}

static int receive_world_dimensions(zn_socket_t sock, int *world_x,
    int *world_y)
{
    char *world_size_msg = NULL;

    world_size_msg = zn_receive_message(sock);
    if (world_size_msg == NULL) {
        *world_x = 0;
        *world_y = 0;
        return 0;
    }
    if (sscanf(world_size_msg, "%d %d", world_x, world_y) != 2) {
        free(world_size_msg);
        return -1;
    }
    free(world_size_msg);
    return 0;
}

static int receive_client_number(zn_socket_t sock, int *client_num)
{
    char *client_num_msg = NULL;
    char *endptr = NULL;
    long num;

    client_num_msg = zn_receive_message(sock);
    if (client_num_msg == NULL) {
        return -1;
    }
    errno = 0;
    num = strtol(client_num_msg, &endptr, 10);
    if (errno != 0 || *endptr != '\0' || num < INT_MIN || num > INT_MAX) {
        free(client_num_msg);
        return -1;
    }
    *client_num = (int)num;
    free(client_num_msg);
    return 0;
}

static int receive_handshake_response(zn_socket_t sock,
    zn_handshake_result_t *result)
{
    if (receive_client_number(sock, &result->client_num) == -1) {
        return -1;
    }
    return receive_world_dimensions(sock, &result->world_x, &result->world_y);
}

int zn_perform_handshake(zn_socket_t sock, zn_role_t role,
    const char *team_name, zn_handshake_result_t *result)
{
    if (validate_handshake_params(sock, role, team_name, result) == -1) {
        return -1;
    }
    if (process_welcome_message(sock) == -1) {
        return -1;
    }
    if (send_team_name(sock, role, team_name) == -1) {
        return -1;
    }
    if (receive_handshake_response(sock, result) == -1) {
        return -1;
    }
    return 0;
}
