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

static server_t *init_server(void)
{
    server_t *server = malloc(sizeof(server_t));

    if (server == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        return NULL;
    }
    if (init_network_integration() == -1) {
        fprintf(stderr, "Failed to initialize network library.\n");
        free(server);
        return NULL;
    }
    memset(server, 0, sizeof(server_t));
    return server;
}

static int handle_args_with_cleanup(int argc, char **argv, server_t *server)
{
    int args_result = handle_args(argc, argv, server);

    if (args_result == 84) {
        cleanup_network_integration();
        handle_free(server);
        return 84;
    }
    if (args_result == 1) {
        cleanup_network_integration();
        handle_free(server);
        return 1;
    }
    return 0;
}

static void run_server(server_t *server)
{
    set_server(server->connection);
    game_loop_run(server);
}

static void cleanup_server(server_t *server)
{
    cleanup_network_integration();
    handle_free(server);
}

int main(int argc, char **argv)
{
    server_t *server = init_server();
    int args_result = 0;

    if (server == NULL) {
        return 84;
    }
    args_result = handle_args_with_cleanup(argc, argv, server);
    if (args_result != 0) {
        return args_result;
    }
    run_server(server);
    cleanup_server(server);
    return 0;
}
