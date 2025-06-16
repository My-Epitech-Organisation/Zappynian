/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** main
*/

#include "../include/server.h"
#include "../include/player.h"
#include "../include/death.h"
#include "../include/game_loop.h"

int main(int argc, char **argv)
{
    server_t *server = malloc(sizeof(server_t));
    int args_result = 0;

    if (server == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        return 84;
    }
    memset(server, 0, sizeof(server_t));
    args_result = handle_args(argc, argv, server);
    if (args_result == 84) {
        handle_free(server);
        return 84;
    }
    if (args_result == 1)
        return handle_free(server);
    set_server(server->connection);
    game_loop_run(server);
    handle_free(server);
    return 0;
}
