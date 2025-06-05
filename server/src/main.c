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

    if (server == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        return 84;
    }
    memset(server, 0, sizeof(server_config_t));
    if (handle_args(argc, argv, server) == 84) {
        handle_free(server);
        return 84;
    }
    display_infos(server);
    while (1);
}
