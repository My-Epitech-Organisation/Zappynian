/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** main
*/

#include "../includes/server.h"
#include "../includes/player.h"
#include "../includes/death.h"
#include "../includes/game_loop.h"

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
    display_infos(server->args);
    set_server(server->connection);
    game_loop_run(server);
    handle_free(server);
    return 0;
}
