/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** fill_data
*/

#include "../includes/server.h"

void fill_port(char **argv, int *i, server_config_t *server)
{
    int port = atoi(argv[++*i]);

    if (port <= 0 || port > 65535) {
        fprintf(stderr, "Invalid port number.\n");
        server->error_code = 84;
    } else
        server->port = port;
}

void fill_witdh(char **argv, int *i, server_config_t *server)
{
    size_t width = atoi(argv[++*i]);

    if (width == 0) {
        fprintf(stderr, "Invalid width.\n");
        server->error_code = 84;
    } else
        server->width = width;
    *i += 1;
}

void fill_height(char **argv, int *i, server_config_t *server)
{
    size_t height = atoi(argv[++*i]);

    if (height == 0) {
        fprintf(stderr, "Invalid height.\n");
        server->error_code = 84;
    } else
        server->height = height;
    *i += 1;
}

void fill_name(char **argv, int argc, int *i, server_config_t *server)
{
    int name_count = 0;

    while (++*i < argc && argv[*i][0] != '-') {
        if (server->team_names == NULL) {
            server->team_names = malloc(sizeof(char *) * (argc - *i + 1));
        }
        if (server->team_names == NULL) {
            fprintf(stderr, "Memory allocation failed.\n");
            server->error_code = 84;
        }
        server->team_names[name_count] = strdup(argv[*i]);
        if (server->team_names[*i - 1] == NULL) {
            fprintf(stderr, "Memory allocation failed for team name.\n");
            server->error_code = 84;
        }
        name_count++;
    }
    *i -= 1;
}

void fill_clientsNb(char **argv, int *i, server_config_t *server)
{
    int clientsNb = atoi(argv[++*i]);

    if (clientsNb <= 0) {
        fprintf(stderr, "Invalid number of clients.\n");
        server->error_code = 84;
    } else
        server->team_count = clientsNb;
    *i += 1;
}

void fill_frequency(char **argv, int *i, server_config_t *server)
{
    int freq = atoi(argv[++*i]);

    if (freq <= 0) {
        fprintf(stderr, "Invalid frequency.\n");
        server->error_code = 84;
    } else
        server->frequency = freq;
    *i += 1;
}
