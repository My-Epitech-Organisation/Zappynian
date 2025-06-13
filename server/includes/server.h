/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** server
*/

#ifndef SERVER_H
    #define SERVER_H
    #define MAX_CLIENTS 100
    #define BUFFER_SIZE 4096

    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <unistd.h>
    #include <getopt.h>
    #include <arpa/inet.h>
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <errno.h>
    #include <poll.h>
    #include <stdbool.h>
    #include "world.h"
    #include "player.h"
    #include "resource.h"
    #include "commands.h"

typedef struct team_s team_t;

typedef enum {
    CLIENT_UNKNOWN,
    CLIENT_IA,
    CLIENT_GUI
} client_type_t;

typedef struct client_s {
    int fd;
    struct sockaddr_in addr;
    socklen_t addr_len;
    char read_buffer[BUFFER_SIZE];
    int read_index;
    char write_buffer[BUFFER_SIZE];
    int write_index;
    int write_total;
    client_type_t type;
    char *team_name;
} client_t;

typedef struct server_args_s {
    int port;
    size_t width;
    size_t height;
    char **team_names;
    int team_count;
    int clients_per_team;
    int frequency;
    int error_code;
    team_t *teams;
} server_args_t;

typedef struct server_connection_s {
    int fd;
    int nfds;
    struct sockaddr_in addr;
    struct pollfd *fds;
    client_t **clients;
    int client_count;
    int port;
    server_args_t *args;
} server_connection_t;

typedef struct server_s {
    server_args_t *args;
    server_connection_t *connection;
    map_t *map;
    player_t **players;
    size_t player_count;
    int tick_count;
    bool game_running;
} server_t;

int check_args(int argc, char **argv, server_args_t *server);
int handle_args(int argc, char **argv, server_t *server);
int handle_options(int opt, char **argv, int argc, server_args_t *server);
int parse_options(int argc, char **argv, server_args_t *server);

void fill_port(server_args_t *server, char *optarg);
void fill_witdh(server_args_t *server, char *optarg);
void fill_height(server_args_t *server, char *optarg);
void fill_name(server_args_t *server, int argc, char **argv);
void fill_clients_nb(server_args_t *server, char *optarg);
void fill_frequency(server_args_t *server, char *optarg);

int display_help(void);
void display_infos(server_args_t *server);

int handle_free(server_t *server);
void handle_error_connection(char *msg, server_connection_t *connection);

void set_server(server_connection_t *connection);
void set_bind(server_connection_t *connection);
void set_listen(server_connection_t *connection);
int set_server_socket(server_connection_t *connection);

void handle_clients(server_t *server);
void handle_client_read(server_connection_t *connection, int client_idx);
void disconnect_client(server_connection_t *connection, int client_idx);
int check_correct_read(server_connection_t *connection, int idx,
    ssize_t bytes_read, client_t *client);

server_t *get_server_instance(void);
void set_server_instance(server_t *server);

void put_str_fd(int fd, char *str);
int strlen_fd(char *str);

#endif /* SERVER_H */
