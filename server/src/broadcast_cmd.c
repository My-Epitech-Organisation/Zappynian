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

static int get_shortest_distance(int pos1, int pos2, size_t map_size)
{
    int d1 = pos1 - pos2;
    int d2 = d1 > 0 ? d1 - map_size : d1 + map_size;

    return (abs(d1) <= abs(d2)) ? d1 : d2;
}

static double normalize_angle(double angle)
{
    while (angle < 0)
        angle += 360;
    while (angle >= 360)
        angle -= 360;
    return angle;
}

static int calculate_direction(player_t *sender, player_t *receiver,
    size_t map_width, size_t map_height)
{
    int dx = 0;
    int dy = 0;
    int direction = 0;
    double angle;

    if (sender->x == receiver->x && sender->y == receiver->y)
        return 0;
    dx = get_shortest_distance(sender->x, receiver->x, map_width);
    dy = get_shortest_distance(sender->y, receiver->y, map_height);
    angle = atan2(dy, dx) * 180.0 / M_PI - receiver->orientation * 90.0;
    angle = normalize_angle(angle);
    direction = (int)((angle + 22.5) / 45.0) + 1;
    return (direction > 8) ? 1 : direction;
}

void broadcast_to_all_players(player_t *sender, server_t *server,
    const char *message)
{
    char broadcast_msg[512];
    player_t *receiver = NULL;
    client_t *client = NULL;

    if (!sender || !server || !message)
        return;
    for (size_t i = 0; i < server->player_count; i++) {
        receiver = server->players[i];
        if (!receiver || receiver->dead || receiver == sender)
            continue;
        client = find_client_by_player(server, receiver);
        if (!client)
            continue;
        snprintf(broadcast_msg, sizeof(broadcast_msg),
            "message %d, %s", calculate_direction(sender, receiver,
            server->map->width, server->map->height), message);
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
