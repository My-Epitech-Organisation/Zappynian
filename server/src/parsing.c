/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** parsing
*/

#include "../includes/server.h"
#include "../includes/team.h"

static int process_option_p_x_y(int opt, server_args_t *server)
{
    switch (opt) {
        case 'p':
            fill_port(server, optarg);
            return 0;
        case 'x':
            fill_witdh(server, optarg);
            return 0;
        case 'y':
            fill_height(server, optarg);
            return 0;
        default:
            return -1;
    }
}

static int process_option_n_c_f(int opt, char **argv, int argc,
    server_args_t *server)
{
    switch (opt) {
        case 'n':
            fill_name(server, argc, argv);
            return 0;
        case 'c':
            fill_clients_nb(server, optarg);
            return 0;
        case 'f':
            fill_frequency(server, optarg);
            return 0;
        default:
            return -1;
    }
}

static int handle_options(int opt, char **argv, int argc,
    server_args_t *server)
{
    if (process_option_p_x_y(opt, server) == 0)
        return (server->error_code == 84) ? 84 : 0;
    if (process_option_n_c_f(opt, argv, argc, server) == 0)
        return (server->error_code == 84) ? 84 : 0;
    fprintf(stderr, "Unknown option: %c\n", opt);
    return 84;
}

static int parse_options(int argc, char **argv, server_args_t *server)
{
    int opt = 0;
    char *opt_string = "p:x:y:n:c:f:";

    opt = getopt(argc, argv, opt_string);
    while (opt != -1) {
        if (handle_options(opt, argv, argc, server) == 84) {
            fprintf(stderr, "Error processing options.\n");
            return 84;
        }
        if (server->error_code == 84) {
            fprintf(stderr, "Error in argument processing.\n");
            return 84;
        }
        opt = getopt(argc, argv, opt_string);
    }
    return 0;
}

static int verify_config(server_args_t *server)
{
    if (server->port == 0 || server->width == 0 || server->height == 0 ||
        server->team_count == 0 || server->clients_per_team == 0 ||
        server->frequency == 0) {
        fprintf(stderr, "Missing required parameters.\n");
        return 84;
    }
    return 0;
}

static int check_args(int argc, char **argv, server_args_t *server)
{
    server->team_names = NULL;
    server->team_count = 0;
    if (parse_options(argc, argv, server) == 84)
        return 84;
    if (verify_config(server) == 84)
        return 84;
    return init_teams(server);
}

static int init_server_memory(server_t *server)
{
    server->connection = malloc(sizeof(server_connection_t));
    server->args = malloc(sizeof(server_args_t));
    if (server->args == NULL) {
        fprintf(stderr, "Memory allocation failed for server arguments.\n");
        return 84;
    }
    memset(server->args, 0, sizeof(server_args_t));
    return 0;
}

static int validate_arguments(int argc, char **argv, server_t *server)
{
    if (argc < 2) {
        fprintf(stderr, "For the usage, check: %s -help\n", argv[0]);
        return 84;
    }
    if (argc == 2 && strcmp(argv[1], "-help") == 0)
        return display_help();
    if (check_args(argc, argv, server->args) == 84) {
        fprintf(stderr, "Invalid arguments.\n");
        return 84;
    }
    return 0;
}

int handle_args(int argc, char **argv, server_t *server)
{
    int result;

    if (init_server_memory(server) == 84)
        return 84;
    result = validate_arguments(argc, argv, server);
    if (result != 0)
        return result;
    server->connection->port = server->args->port;
    return 0;
}
