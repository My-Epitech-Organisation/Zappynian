/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** Broadcast command with proper sound direction calculation
*/

#include "../include/commands.h"
#include "../include/server.h"
#include "../include/world.h"
#include <math.h>

static int calculate_direction(player_t *sender, player_t *receiver, 
    size_t map_width, size_t map_height)
{
    int direction = 0;
    int dx1 = 0;
    int dx2 = 0;
    int dx = 0;
    int dy1 = 0;
    int dy2 = 0;
    int dy = 0;
    double angle = 0;

    if (sender->x == receiver->x && sender->y == receiver->y)
        return 0;
    dx1 = sender->x - receiver->x;
    dx2 = dx1 > 0 ? dx1 - map_width : dx1 + map_width;
    dx = (abs(dx1) <= abs(dx2)) ? dx1 : dx2;
    dy1 = sender->y - receiver->y;
    dy2 = dy1 > 0 ? dy1 - map_height : dy1 + map_height;
    dy = (abs(dy1) <= abs(dy2)) ? dy1 : dy2;
    angle = atan2(dy, dx) * 180.0 / M_PI;
    angle -= receiver->orientation * 90.0;
    while (angle < 0)
        angle += 360;
    while (angle >= 360)
        angle -= 360;
    direction = (int)((angle + 22.5) / 45.0) + 1;
    if (direction > 8)
        direction = 1;
    return direction;
}

void broadcast_to_all_players(player_t *sender, server_t *server,
    const char *message)
{
    char broadcast_msg[512];
    player_t *receiver = NULL;
    client_t *client = NULL;
    int direction = 0;

    if (!sender || !server || !message)
        return;
    for (size_t i = 0; i < server->player_count; i++) {
        receiver = server->players[i];
        if (!receiver || receiver->dead || receiver == sender)
            continue;
        client = find_client_by_player(server, receiver);
        if (!client)
            continue;
        direction = calculate_direction(sender, receiver, 
            server->map->width, server->map->height);
        snprintf(broadcast_msg, sizeof(broadcast_msg), 
            "message %d, %s", direction, message);
        zn_send_message(client->zn_sock, broadcast_msg);
    }
    send_pbc(server, sender, message);
}

char *get_broadcast_message(player_t *player)
{
    char *full_cmd = NULL;
    char *broadcast_text = NULL;
    char *message = NULL;

    if (!player || !player->current_command_line)
        return NULL;
    full_cmd = player->current_command_line;
    if (full_cmd == NULL || strlen(full_cmd) < 10)
        return NULL;
    if (strncmp(full_cmd, "Broadcast ", 10) != 0)
        return NULL;
    broadcast_text = full_cmd + 10;
    if (strlen(broadcast_text) == 0)
        return NULL;
    message = malloc(strlen(broadcast_text) + 1);
    if (message == NULL)
        return NULL;
    strcpy(message, broadcast_text);
    message[strcspn(message, "\n")] = '\0';
    return message;
}
