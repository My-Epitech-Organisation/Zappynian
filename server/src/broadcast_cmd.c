/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** broadcast_cmd
*/

#include "../include/commands.h"
#include "../include/server.h"

static int get_absolute_direction(int dx, int dy)
{
    if (dy > 0 && dx == 0)
        return 0;
    if (dy > 0 && dx > 0)
        return 1;
    if (dy == 0 && dx > 0)
        return 2;
    if (dy < 0 && dx > 0)
        return 3;
    if (dy < 0 && dx == 0)
        return 4;
    if (dy < 0 && dx < 0)
        return 5;
    if (dy == 0 && dx < 0)
        return 6;
    if (dy > 0 && dx < 0)
        return 7;
    return 0;
}

static int convert_to_tile_number(int dx, int dy, int orientation)
{
    int abs_dir = 0;
    int relative_dir = 0;

    abs_dir = get_absolute_direction(dx, dy);
    relative_dir = (abs_dir - orientation + 8) % 8;
    return relative_dir + 1;
}

int calculate_broadcast_direction(player_t *receiver, player_t *sender,
    map_t *map)
{
    int dx;
    int dy;
    int map_width = (int)map->width;
    int map_height = (int)map->height;

    if (receiver->x == sender->x && receiver->y == sender->y)
        return 0;
    dx = sender->x - receiver->x;
    dy = sender->y - receiver->y;
    if (abs(dx) > map_width / 2)
        dx = dx > 0 ? dx - map_width : dx + map_width;
    if (abs(dy) > map_height / 2)
        dy = dy > 0 ? dy - map_height : dy + map_height;
    return convert_to_tile_number(dx, dy, receiver->orientation);
}

void broadcast_to_all_players(player_t *sender, server_t *server,
    const char *message)
{
    player_t *receiver = NULL;
    int direction = 0;
    char buffer[256];

    if (server == NULL || server->players == NULL || message == NULL)
        return;
    for (size_t i = 0; i < server->player_count; i++) {
        receiver = server->players[i];
        if (receiver == NULL || receiver->dead)
            continue;
        direction = calculate_broadcast_direction(receiver, sender,
            server->map);
        snprintf(buffer, sizeof(buffer), "message %d, %s", direction, message);
        zn_send_message(server->connection->zn_server, buffer);
    }
}

char *get_broadcast_message(player_t *player)
{
    char *full_cmd = get_player_current_command(player);
    char *broadcast_text = NULL;
    char *message = NULL;

    if (full_cmd == NULL || strlen(full_cmd) < 10)
        return NULL;
    if (strncmp(full_cmd, "Broadcast ", 10) != 0)
        return NULL;
    broadcast_text += 10;
    if (strlen(broadcast_text) == 0)
        return NULL;
    message = malloc(strlen(broadcast_text) + 1);
    if (message == NULL)
        return NULL;
    strcpy(message, broadcast_text);
    message[strcspn(message, "\n")] = '\0';
    return message;
}
