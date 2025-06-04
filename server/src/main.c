/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** main
*/

#include "../includes/server.h"

int check_args(int argc, char **argv, server_config_t *server)
{
    if (argc < 13) {
        fprintf(stderr, "Not enough arguments.\n");
        return 84;
    }
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-p") == 0 && i + 1 < argc) {
            printf("The arg number: %d is %s\n", i, argv[i]);
            fill_port(argv, &i, server);
        } else if (strcmp(argv[i], "-x") == 0 && i + 1 < argc)
            fill_witdh(argv, &i, server);
        else if (strcmp(argv[i], "-y") == 0 && i + 1 < argc)
            fill_height(argv, &i, server);
        else if (strcmp(argv[i], "-n") == 0)
            fill_name(argv, argc, &i, server);
        else if (strcmp(argv[i], "-c") == 0 && i + 1 < argc)
            fill_clientsNb(argv, &i, server);
        else if (strcmp(argv[i], "-f") == 0 && i + 1 < argc)
            fill_frequency(argv, &i, server);
        else {
            printf("Argument: %s\n", argv[i]);
            fprintf(stderr, "Unknown argument: %s\n", argv[i]);
            return 84;
        }
    }
    return 0;
}

int display_help(void)
{
    printf("Usage: ./zappy_server -p port -x width -y height");
    printf(" -n name1 name2 ...-c clientsNb -f freq\n");
    return 0;
}

int handle_args(int argc, char **argv, server_config_t *server)
{
    if (argc < 2) {
        fprintf(stderr, "For the usage, check: %s -help\n", argv[0]);
        return 84;
    }
    if (argc == 2 && strcmp(argv[1], "-help") == 0)
        return display_help();
    if (check_args(argc, argv, server) == 84) {
        fprintf(stderr, "Invalid arguments.\n");
        // return 84;
    }
    return 0;
}

int main(int argc, char **argv)
{
    server_config_t *server = malloc(sizeof(server_config_t));
    if (handle_args(argc, argv, server) == 84)
        return 84;
    printf("\nZappynian Server started with %d arguments.\n", argc);
    printf("Server initialized with : \nport: %d\nwidth: %zu\nheight: %zu\n",
           server->port, server->width, server->height);
    printf("Team names: ");
    for (int i = 0; server->team_names && server->team_names[i]; i++) {
        printf("%s ", server->team_names[i]);
    }
    printf("\nAuthorized clients: %d\nFrequency: %d\n",
           server->team_count, server->frequency);
    while (1);
}
