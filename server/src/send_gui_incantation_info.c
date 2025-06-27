/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** send_gui_incantation_info
*/

#include "../include/server.h"
#include "../include/player.h"
#include "../include/elevation.h"

static char *resize_teams_buffer(char *teams, size_t new_size)
{
    char *new_teams = realloc(teams, new_size);

    if (new_teams == NULL) {
        free(teams);
        return NULL;
    }
    return new_teams;
}

static char *process_player_team(char *teams, player_t *player,
    size_t *total_len)
{
    size_t team_name_len;
    size_t prefix_len;
    size_t new_total;
    int written;
    char *prefix;

    if (player == NULL || player->team_name == NULL)
        return teams;
    team_name_len = strlen(player->team_name);
    prefix_len = (*total_len > 0) ? 2 : 1;
    new_total = *total_len + team_name_len + prefix_len + 1;
    teams = resize_teams_buffer(teams, new_total);
    if (teams == NULL)
        return NULL;
    prefix = (*total_len == 0) ? "#" : " #";
    written = snprintf(teams + *total_len, new_total - *total_len,
        "%s%s", prefix, player->team_name);
    if (written > 0)
        *total_len += written;
    return teams;
}

static char *get_teams_in_tile(tile_t *tile)
{
    char *teams = malloc(1);
    size_t total_len = 0;

    if (teams == NULL)
        return NULL;
    teams[0] = '\0';
    for (size_t i = 0; i < tile->player_count; i++) {
        teams = process_player_team(teams, tile->players[i], &total_len);
        if (teams == NULL)
            return NULL;
    }
    return teams;
}

void send_pic(server_t *server, tile_t *tile, player_t *player)
{
    char pic_command[256];
    int ret;
    char *teams = NULL;

    if (server->graphic_clients == NULL || tile == NULL || player == NULL)
        return;
    teams = get_teams_in_tile(tile);
    ret = snprintf(pic_command, sizeof(pic_command),
        "pic %zu %zu %d %s",
        tile->x, tile->y, player->level, teams);
    if (ret < 0 || (size_t)ret >= sizeof(pic_command))
        return;
    send_to_all_graphic_clients(server->graphic_clients, pic_command);
}

void send_pie(server_t *server, tile_t *tile, bool result)
{
    char pie_command[256];
    int ret;
    char *str_result = result ? "1" : "0";

    if (server->graphic_clients == NULL || tile == NULL)
        return;
    ret = snprintf(pie_command, sizeof(pie_command),
        "pie %zu %zu %s", tile->x, tile->y, str_result);
    if (ret < 0 || (size_t)ret >= sizeof(pie_command))
        return;
    send_to_all_graphic_clients(server->graphic_clients, pie_command);
}
