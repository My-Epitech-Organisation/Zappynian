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
    if (init_network_integration() == -1) {
        fprintf(stderr, "Failed to initialize network library.\n");
        free(server);
        return 84;
    }
    memset(server, 0, sizeof(server_t));
    args_result = handle_args(argc, argv, server);
    if (args_result == 84) {
        cleanup_network_integration();
        handle_free(server);
        return 84;
    }
    if (args_result == 1) {
        cleanup_network_integration();
        return handle_free(server);
    }
    set_server(server->connection);
    game_loop_run(server);
    cleanup_network_integration();
    handle_free(server);
    return 0;
}
