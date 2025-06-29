/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** Movement command implementations
*/

#include "../include/commands.h"
#include "../include/server.h"
#include <stdbool.h>

static void get_next_position(player_t *player, int *new_x, int *new_y)
{
    int dx[] = {0, 1, 0, -1};
    int dy[] = {-1, 0, 1, 0};

    *new_x = player->x + dx[player->orientation];
    *new_y = player->y + dy[player->orientation];
}

static bool is_move_valid(player_t *player, client_t *client)
{
    if (client == NULL)
        return false;
    if (player->dead || player->in_elevation) {
        zn_send_message(client->zn_sock, "ko");
        return false;
    }
    return true;
}

void cmd_forward(player_t *player, server_t *server)
{
    client_t *client = find_client_by_player(server, player);
    int new_x = 0;
    int new_y = 0;

    if (!is_move_valid(player, client))
        return;
    get_next_position(player, &new_x, &new_y);
    move_player(player, new_x, new_y, server->map);
    send_ppo(server, player);
    zn_send_message(client->zn_sock, "ok");
}

void cmd_right(player_t *player, server_t *server)
{
    client_t *client = NULL;

    client = find_client_by_player(server, player);
    if (client == NULL) {
        return;
    }
    if (player->dead || player->in_elevation) {
        zn_send_message(client->zn_sock, "ko");
        return;
    }
    player->orientation = (player->orientation + 1) % 4;
    send_ppo(server, player);
    zn_send_message(client->zn_sock, "ok");
}

void cmd_left(player_t *player, server_t *server)
{
    client_t *client = NULL;

    client = find_client_by_player(server, player);
    if (client == NULL) {
        return;
    }
    if (player->dead || player->in_elevation) {
        zn_send_message(client->zn_sock, "ko");
        return;
    }
    player->orientation = (player->orientation + 3) % 4;
    send_ppo(server, player);
    zn_send_message(client->zn_sock, "ok");
}

void cmd_look(player_t *player, server_t *server)
{
    char *look_result = NULL;
    client_t *client = NULL;

    client = find_client_by_player(server, player);
    if (client == NULL) {
        return;
    }
    if (player->dead || player->in_elevation) {
        zn_send_message(client->zn_sock, "ko");
        return;
    }
    look_result = get_player_vision(player, server->map);
    if (look_result != NULL) {
        zn_send_message(client->zn_sock, look_result);
        free(look_result);
    } else
        zn_send_message(client->zn_sock, "ko");
}
