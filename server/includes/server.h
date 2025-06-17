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
    #include <stdbool.h>
    #include "world.h"
    #include "player.h"
    #include "resource.h"
    #include "commands.h"
    #include "../../libzappy_net/include/zappy_net.h"

typedef struct team_s team_t;

typedef enum {
    CLIENT_UNKNOWN = ZN_ROLE_UNKNOWN,
    CLIENT_IA = ZN_ROLE_AI,
    CLIENT_GUI = ZN_ROLE_GUI
} client_type_t;

typedef struct client_s {
    zn_socket_t zn_sock;
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
    zn_socket_t zn_server;
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

// Simplified network setup function
void set_server(server_connection_t *connection);

// Client handling functions
void handle_client_read(server_connection_t *connection, int client_idx);
void handle_client_write(server_connection_t *connection, int client_idx);
void disconnect_client(server_connection_t *connection, int client_idx);
void assign_client_type(client_t *client, server_connection_t *connection,
    int idx);
void accept_client(server_connection_t *connection, server_args_t *args);

/* Server loop functions */
void server_loop(server_t *server);
void stop_server_loop(void);

// String utilities
void send_string_to_client(client_t *client, char *str);
int strlen_custom(char *str);

/* Network integration functions */
int init_network_integration(void);
void cleanup_network_integration(void);
zn_socket_t setup_zappy_server_socket(int port);

/* Client socket management */
void init_client_zappy_socket(client_t *client, zn_socket_t zn_sock);
void cleanup_client_zappy_socket(client_t *client);

#endif /* SERVER_H */
