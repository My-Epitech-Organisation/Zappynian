/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Client-side handshake implementation
*/

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

static int receive_handshake_response(zn_socket_t sock,
    zn_handshake_result_t *result)
{
    char *client_num_msg = NULL;
    char *world_size_msg = NULL;

    client_num_msg = zn_receive_message(sock);
    if (client_num_msg == NULL) {
        return -1;
    }
    result->client_num = atoi(client_num_msg);
    free(client_num_msg);
    world_size_msg = zn_receive_message(sock);
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
