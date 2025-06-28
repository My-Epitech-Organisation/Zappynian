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
typedef struct egg_s egg_t;

typedef struct graphic_client_node_s {
    client_t *client;
    struct graphic_client_node_s *next;
} graphic_client_node_t;

typedef struct graphic_client_list_s {
    graphic_client_node_t *head;
    int count;
} graphic_client_list_t;

typedef enum {
    CLIENT_UNKNOWN = ZN_ROLE_UNKNOWN,
    CLIENT_IA = ZN_ROLE_AI,
    CLIENT_GUI = ZN_ROLE_GUI
} client_type_t;

typedef enum client_event_e {
    CLIENT_EVENT_NONE = 0,
    CLIENT_EVENT_ERROR = -1,
    CLIENT_EVENT_PENDING = -2,
    CLIENT_EVENT_IA_CONNECTED = 1,
    CLIENT_EVENT_GUI_CONNECTED = 2,
    CLIENT_EVENT_DISCONNECTED = 3
} client_event_t;

typedef struct client_s {
    zn_socket_t zn_sock;
    client_type_t type;
    char *team_name;
    int id;
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
    volatile bool server_running;
    egg_t *eggs;
    graphic_client_list_t *graphic_clients;
} server_t;

int handle_args(int argc, char **argv, server_t *server);

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
client_event_t handle_client_read(server_t *server, int idx);
void handle_client_write(server_t *server, int client_idx);
void disconnect_client(server_t *server, int client_idx);
client_event_t assign_client_type(client_t *client, server_t *server, int idx);
void accept_client(server_t *server, server_args_t *args);

/* Client handshake functions */
client_event_t setup_client_handshake(client_t *client,
    server_connection_t *connection, int idx, char *team_name);
int validate_and_respond(client_t *client, server_t *server, int idx,
    const char *team_name);
void finalize_client_assignment(client_t *client,
    server_connection_t *connection, const char *team_name);

/* Server loop functions */
void server_loop(server_t *server);
void stop_server_loop(server_t *server);

/* Server player management functions */
void handle_client_event(server_t *server, client_event_t event,
    int client_idx);
int initialize_server_players(server_t *server);
player_t *create_player_for_client(server_t *server, client_t *client,
    team_t *team);

/* Network integration functions */
int init_network_integration(void);
void cleanup_network_integration(void);

/* Client socket management */
void init_client_zappy_socket(client_t *client, zn_socket_t zn_sock);
void cleanup_client_zappy_socket(client_t *client);

player_t *find_player_by_client(server_connection_t *connection,
    client_t *client);
void player_found(player_t *player, const char *line, client_t *client);

const char **get_resource_names(void);

void send_stat_to_all_players(server_t *server, tile_t *current_tile,
    size_t i, const char *stat_msg);

int send_graphic_initial_state(client_t *client, server_t *server);
void send_msz(server_t *server, size_t width, size_t height);
void send_bct(server_t *server, tile_t *tile);
void send_mct(server_t *server, map_t *map);
void send_tna(server_t *server, team_t *teams, int team_count);
void send_pnw(server_t *server, player_t *player);
void send_ppo(server_t *server, player_t *player);
void send_plv(server_t *server, player_t *player);
void send_pin(server_t *server, player_t *player);
void send_pex(server_t *server, player_t *player);
void send_pbc(server_t *server, player_t *player, const char *message);
void send_pic(server_t *server, tile_t *tile, player_t *player);
void send_pie(server_t *server, tile_t *tile, bool result);
void send_pfk(server_t *server, int player_id);
void send_pdr(server_t *server, int player_id, int resource_type);
void send_pgt(server_t *server, int player_id, int resource_type);
void send_pdi(server_t *server, player_t *player);
void send_ewn(server_t *server);
void send_ebo(server_t *server, int egg_id);
void send_edi(server_t *server, int egg_id);
void send_sgt(server_t *server, int frequency);
void send_sst(server_t *server, int time);
void send_seg(server_t *server, const char *team_name);
void send_smg(server_t *server, const char *message);
void send_suc(server_t *server);
void send_sbp(server_t *server);

int init_eggs_list(server_t *server);
void make_enough_eggs_for_team(server_t *server, int team_idx);

/* Graphic clients linked list management functions */
graphic_client_list_t *create_graphic_client_list(void);
void destroy_graphic_client_list(graphic_client_list_t *list);
int add_graphic_client(graphic_client_list_t *list, client_t *client);
int remove_graphic_client(graphic_client_list_t *list, client_t *client);
graphic_client_node_t *find_graphic_client(graphic_client_list_t *list,
    int client_id);
void send_to_all_graphic_clients(graphic_client_list_t *list,
    const char *message);

void remove_player_from_server(server_t *server, player_t *player);
void remove_player_from_team(team_t *team, player_t *player);

#endif /* SERVER_H */
