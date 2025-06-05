/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** main
*/

#include "../includes/server.h"

int main(int argc, char **argv)
{
    server_config_t *server = malloc(sizeof(server_config_t));
    int args_result = 0;

    if (server == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        return 84;
    }
    memset(server, 0, sizeof(server_config_t));
    args_result = handle_args(argc, argv, server);
    if (args_result == 84) {
        handle_free(server);
        return 84;
    }
    if (args_result == 1) {
        return handle_free(server);
    }
    display_infos(server);
    while (1) {
        sleep(1);
    }
}
