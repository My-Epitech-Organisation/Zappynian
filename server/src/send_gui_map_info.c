/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** send_gui_start_data
*/

#include "../include/server.h"
#include "../include/team.h"
#include "../include/world.h"
#include "../include/egg.h"

void send_msz(server_t *server, size_t width, size_t height)
{
    char msz_command[256];
    int ret;

    if (server->graphic_clients == NULL || width == 0 || height == 0)
        return;
    ret = snprintf(msz_command, sizeof(msz_command),
        "msz %zu %zu", width, height);
    if (ret < 0 || (size_t)ret >= sizeof(msz_command))
        return;
    send_to_all_graphic_clients(server->graphic_clients, msz_command);
}

void send_mct(server_t *server, map_t *map)
{
    tile_t *tile;

    if (server->graphic_clients == NULL || map == NULL)
        return;
    for (size_t i = 0; i < map->width; i++) {
        for (size_t j = 0; j < map->height; j++) {
            tile = get_tile(map, i, j);
            tile->x = i;
            tile->y = j;
            send_bct(server, tile);
        }
    }
}

void send_bct(server_t *server, tile_t *tile)
{
    char bct_command[256];
    int ret;

    if (server->graphic_clients == NULL || tile == NULL)
        return;
    ret = snprintf(bct_command, sizeof(bct_command),
        "bct %zu %zu %d %d %d %d %d %d %d",
        tile->x, tile->y, tile->resources[0], tile->resources[1],
        tile->resources[2], tile->resources[3], tile->resources[4],
        tile->resources[5], tile->resources[6]);
    if (ret < 0 || (size_t)ret >= sizeof(bct_command))
        return;
    send_to_all_graphic_clients(server->graphic_clients, bct_command);
}
