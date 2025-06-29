/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** Movement command implementations
*/

#include "../include/commands.h"
#include "../include/server.h"

void cmd_forward(player_t *player, server_t *server)
{
    client_t *client;

    client = find_client_by_player(server, player);
    if (!client)
        return;
    if (player->dead || player->in_elevation) {
        zn_send_message(client->zn_sock, "ko");
        return;
    }
    switch (player->orientation) {
        case NORTH:
            move_player(player, player->x, player->y - 1, server->map);
            break;
        case EAST:
            move_player(player, player->x + 1, player->y, server->map);
            break;
        case SOUTH:
            move_player(player, player->x, player->y + 1, server->map);
            break;
        case WEST:
            move_player(player, player->x - 1, player->y, server->map);
            break;
    }
    send_ppo(server, player);
    zn_send_message(client->zn_sock, "ok");
}

void cmd_right(player_t *player, server_t *server)
{
    client_t *client;

    client = find_client_by_player(server, player);
    if (!client)
        return;
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
    client_t *client;

    client = find_client_by_player(server, player);
    if (!client)
        return;
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
    client_t *client;

    client = find_client_by_player(server, player);
    if (!client)
        return;
    if (player->dead || player->in_elevation) {
        zn_send_message(client->zn_sock, "ko");
        return;
    }
    look_result = get_player_vision(player, server->map);
    if (look_result != NULL) {
        printf("Look result: %s\n", look_result);
        zn_send_message(client->zn_sock, look_result);
        free(look_result);
    } else
        zn_send_message(client->zn_sock, "ko");
}
