/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** fill_data
*/

#include "../includes/server.h"

void fill_port(server_config_t *server, char *optarg)
{
    server->port = atoi(optarg);
    if (server->port <= 0 || server->port > 65535) {
        fprintf(stderr, "Invalid port number.\n");
        server->error_code = 84;
    }
}

void fill_witdh(server_config_t *server, char *optarg)
{
    char *endptr;
    long width = strtol(optarg, &endptr, 10);

    if (*endptr != '\0' || width <= 0) {
        fprintf(stderr, "Invalid width.\n");
        server->error_code = 84;
        return;
    }
    server->width = (size_t)width;
}

void fill_height(server_config_t *server, char *optarg)
{
    char *endptr;
    long height = strtol(optarg, &endptr, 10);

    if (*endptr != '\0' || height <= 0) {
        fprintf(stderr, "Invalid height.\n");
        server->error_code = 84;
        return;
    }
    server->height = (size_t)height;
}
