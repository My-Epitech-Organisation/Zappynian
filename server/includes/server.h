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

void fill_port(char **argv, int *i, server_config_t *server);
void fill_witdh(char **argv, int *i, server_config_t *server);
void fill_height(char **argv, int *i, server_config_t *server);
void fill_name(char **argv, int argc, int *i, server_config_t *server);
void fill_clientsNb(char **argv, int *i, server_config_t *server);
void fill_frequency(char **argv, int *i, server_config_t *server);

#endif /* SERVER_H */
