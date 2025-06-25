/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** send_state_msg_to_players
*/

#include "../include/server.h"

void send_stat_to_all_players(server_t *server, tile_t *current_tile,
    size_t i, const char *stat_msg)
{
    client_t *elevated_clt;

    if (current_tile->players[i] && current_tile->players[i]->in_elevation) {
        elevated_clt = find_client_by_player(server, current_tile->players[i]);
        if (elevated_clt) {
            zn_send_message(elevated_clt->zn_sock, stat_msg);
        }
    }
}
