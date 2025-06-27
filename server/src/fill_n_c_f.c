/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** fill_n_c_f
*/

#include "../include/server.h"

static int init_team_names_array(server_args_t *server, int argc)
{
    if (server->team_names == NULL) {
        server->team_names = malloc(sizeof(char *) * (argc + 1));
        if (server->team_names == NULL) {
            fprintf(stderr, "Memory allocation failed.\n");
            server->error_code = 84;
            return -1;
        }
    }
    return 0;
}

static void parse_team_names(server_args_t *server, int argc, char **argv,
    int *name_count)
{
    if (optarg != NULL) {
        server->team_names[*name_count] = strdup(optarg);
        (*name_count)++;
    }
    while (optind < argc && argv[optind][0] != '-') {
        server->team_names[*name_count] = strdup(argv[optind]);
        (*name_count)++;
        optind++;
    }
}

void fill_name(server_args_t *server, int argc, char **argv)
{
    int name_count = 0;

    if (init_team_names_array(server, argc) == -1)
        return;
    parse_team_names(server, argc, argv, &name_count);
    server->team_names[name_count] = NULL;
    server->team_count = name_count;
}

void fill_clients_nb(server_args_t *server, char *optarg)
{
    server->clients_per_team = atoi(optarg);
    if (server->clients_per_team <= 0) {
        fprintf(stderr, "Invalid number of clients.\n");
        server->error_code = 84;
    }
}

void fill_frequency(server_args_t *server, char *optarg)
{
    server->frequency = atoi(optarg);
    if (server->frequency <= 0) {
        fprintf(stderr, "Invalid frequency.\n");
        server->error_code = 84;
    }
}
