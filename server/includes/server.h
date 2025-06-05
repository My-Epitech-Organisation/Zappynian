/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** server
*/

#ifndef SERVER_H
    #define SERVER_H

    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <unistd.h>
    #include <getopt.h>

typedef struct server_config_s {
    int port;
    size_t width;
    size_t height;
    char **team_names;
    int team_count;
    int clients_per_team;
    int frequency;
    int error_code;
} server_config_t;

int check_args(int argc, char **argv, server_config_t *server);
int handle_args(int argc, char **argv, server_config_t *server);

void fill_port(server_config_t *server, char *optarg);
void fill_witdh(server_config_t *server, char *optarg);
void fill_height(server_config_t *server, char *optarg);
void fill_name(server_config_t *server, int argc, char **argv);
void fill_clients_nb(server_config_t *server, char *optarg);
void fill_frequency(server_config_t *server, char *optarg);

int display_help(void);
void display_infos(server_config_t *server);

void handle_free(server_config_t *server);

#endif /* SERVER_H */
