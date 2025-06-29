/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** Communication command implementations
*/

#include <stdio.h>
#include "../include/commands.h"
#include "../include/server.h"
#include "../include/egg.h"

void cmd_inventory(player_t *player, server_t *server)
{
    char *inventory_result = NULL;
    client_t *client = NULL;

    client = find_client_by_player(server, player);
    if (client == NULL) {
        return;
    }
    if (player->dead || player->in_elevation) {
        zn_send_message(client->zn_sock, "ko");
        return;
    }
    inventory_result = get_player_inventory(player);
    if (inventory_result != NULL) {
        send_pin(server, player);
        printf("Inventory result: %s\n", inventory_result);
        zn_send_message(client->zn_sock, inventory_result);
        free(inventory_result);
    } else
        zn_send_message(client->zn_sock, "ko");
}

void cmd_broadcast(player_t *player, server_t *server)
{
    char *broadcast_message = NULL;
    client_t *client = NULL;

    client = find_client_by_player(server, player);
    if (client == NULL) {
        return;
    }
    if (player->dead || player->in_elevation) {
        zn_send_message(client->zn_sock, "ko");
        return;
    }
    broadcast_message = get_broadcast_message(player);
    if (broadcast_message == NULL || strlen(broadcast_message) == 0) {
        zn_send_message(client->zn_sock, "ko");
        return;
    }
    broadcast_to_all_players(player, server, broadcast_message);
    send_pbc(server, player, broadcast_message);
    free(broadcast_message);
    zn_send_message(client->zn_sock, "ok");
}

void cmd_connect_nbr(player_t *player, server_t *server)
{
    char response[20];
    client_t *client = NULL;

    client = find_client_by_player(server, player);
    if (client == NULL) {
        return;
    }
    if (player->dead || player->in_elevation) {
        zn_send_message(client->zn_sock, "ko");
        return;
    }
    snprintf(response, sizeof(response), "%d",
        server->args->clients_per_team - player->slot_id);
    zn_send_message(client->zn_sock, response);
}

void cmd_fork(player_t *player, server_t *server)
{
    egg_t *new_egg = NULL;
    client_t *client = NULL;

    client = find_client_by_player(server, player);
    if (client == NULL) {
        return;
    }
    if (player->dead || player->in_elevation) {
        zn_send_message(client->zn_sock, "ko");
        return;
    }
    send_pfk(server, player->id);
    new_egg = create_egg(player->x, player->y, player->team_name);
    if (new_egg == NULL) {
        zn_send_message(client->zn_sock, "ko");
        return;
    }
    add_egg_to_server(server, new_egg);
    send_enw(server);
    zn_send_message(client->zn_sock, "ok");
}
