/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** send_init_gui
*/

#include "../include/server.h"
#include "../include/team.h"
#include "../include/world.h"
#include "../include/egg.h"

static void send_msz_to_client(client_t *client, size_t width, size_t height)
{
    char msz_command[256];
    int ret;

    if (client == NULL || client->zn_sock == NULL || width == 0 || height == 0)
        return;
    ret = snprintf(msz_command, sizeof(msz_command),
        "msz %zu %zu", width, height);
    if (ret < 0 || (size_t)ret >= sizeof(msz_command))
        return;
    zn_send_message(client->zn_sock, msz_command);
}

static void send_sgt_to_client(client_t *client, size_t frequency)
{
    char sgt_command[256];
    int ret;

    if (client == NULL || client->zn_sock == NULL || frequency == 0)
        return;
    ret = snprintf(sgt_command, sizeof(sgt_command), "sgt %zu", frequency);
    if (ret < 0 || (size_t)ret >= sizeof(sgt_command))
        return;
    zn_send_message(client->zn_sock, sgt_command);
}

static void send_bct_to_client(client_t *client, tile_t *tile)
{
    char bct_command[256];
    int ret;

    if (client == NULL || client->zn_sock == NULL || tile == NULL)
        return;
    ret = snprintf(bct_command, sizeof(bct_command),
        "bct %zu %zu %d %d %d %d %d %d %d",
        tile->x, tile->y, tile->resources[0], tile->resources[1],
        tile->resources[2], tile->resources[3], tile->resources[4],
        tile->resources[5], tile->resources[6]);
    if (ret < 0 || (size_t)ret >= sizeof(bct_command))
        return;
    zn_send_message(client->zn_sock, bct_command);
}

static void send_mct_to_client(client_t *client, map_t *map)
{
    tile_t *tile;

    if (client == NULL || client->zn_sock == NULL || map == NULL)
        return;
    for (size_t i = 0; i < map->width; i++) {
        for (size_t j = 0; j < map->height; j++) {
            tile = get_tile(map, i, j);
            tile->x = i;
            tile->y = j;
            send_bct_to_client(client, tile);
        }
    }
}

static void send_tna_to_client(client_t *client, team_t *teams, int team_count)
{
    char tna_command[256];
    int ret;

    if (client == NULL || client->zn_sock == NULL || teams == NULL)
        return;
    for (int i = 0; i < team_count; i++) {
        ret = snprintf(tna_command, sizeof(tna_command),
            "tna %s", teams[i].name);
        if (ret < 0 || (size_t)ret >= sizeof(tna_command))
            continue;
        zn_send_message(client->zn_sock, tna_command);
    }
}

static void send_ewn_to_client_recursive(client_t *client, egg_t *egg)
{
    char ewn_command[256];
    int ret;

    if (egg == NULL)
        return;
    send_ewn_to_client_recursive(client, egg->next);
    ret = snprintf(ewn_command, sizeof(ewn_command),
        "ewn #%d #%d %d %d", egg->id, egg->player_id,
        egg->x, egg->y);
    if (ret < 0 || (size_t)ret >= sizeof(ewn_command))
        return;
    zn_send_message(client->zn_sock, ewn_command);
}

static void send_ewn_to_client(client_t *client, server_t *server)
{
    if (client == NULL || client->zn_sock == NULL || server == NULL)
        return;
    send_ewn_to_client_recursive(client, server->eggs);
}

int send_graphic_initial_state(client_t *client, server_t *server)
{
    if (client == NULL || server == NULL || client->zn_sock == NULL ||
        server->args == NULL)
        return -1;
    send_msz_to_client(client, server->args->width, server->args->height);
    send_sgt_to_client(client, server->args->frequency);
    send_mct_to_client(client, server->map);
    send_tna_to_client(client, server->args->teams, server->args->team_count);
    send_ewn_to_client(client, server);
    return 0;
}
