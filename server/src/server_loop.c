/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** Server main loop with libzappy_net integration
*/

#include "../include/server.h"
#include "../include/game_loop.h"
#include "../include/egg.h"
#include "../include/timing_integration.h"

static void handle_socket_events(server_t *server,
    zn_poll_result_t *result, int i, int client_idx)
{
    client_event_t event = CLIENT_EVENT_NONE;

    if (result->readable & (1ULL << i)) {
        event = handle_client_read(server, client_idx);
        handle_client_event(server, event, client_idx);
    }
    if (result->writable & (1ULL << i))
        handle_client_write(server->connection, client_idx);
    if (result->error & (1ULL << i)) {
        disconnect_client(server->connection, client_idx);
        handle_client_event(server, CLIENT_EVENT_DISCONNECTED, client_idx);
    }
}

static void handle_ready_sockets(server_t *server,
    zn_poll_result_t *result, zn_socket_t *sockets, int count)
{
    int client_idx;

    if (result->readable & 1)
        accept_client(server->connection, NULL);
    for (int i = 1; i < count; i++) {
        client_idx = find_client_by_socket(server->connection, sockets[i]);
        if (client_idx == -1)
            continue;
        handle_socket_events(server, result, i, client_idx);
    }
}

static int init_client_array(server_t *server)
{
    if (server->connection->clients == NULL) {
        server->connection->clients = calloc(MAX_CLIENTS, sizeof(client_t *));
        if (server->connection->clients == NULL) {
            return -1;
        }
        server->connection->client_count = 0;
    }
    return 0;
}

void check_min_eggs(server_t *server)
{
    for (int i = 0; i < server->args->team_count; i++) {
        make_enough_eggs_for_team(server, i);
    }
}

static void process_game_tick(server_t *server)
{
    game_loop_tick(server);
    process_commands(server);
    server_timing_tick(server);
    death_check(server->players, server->player_count, server->map, server);
    check_victory(server);
    check_min_eggs(server);
}

void stop_server_loop(server_t *server)
{
    server->server_running = false;
}

void server_loop(server_t *server)
{
    zn_socket_t sockets[ZN_POLL_MAX_SOCKETS];
    short events[ZN_POLL_MAX_SOCKETS];
    int count = 0;
    zn_poll_result_t poll_result;
    int timeout;

    if (init_client_array(server) == -1)
        return;
    if (initialize_server_players(server) == -1)
        return;
    if (server_timing_init(server) == -1)
        return;
    while (server->server_running && server->game_running) {
        setup_socket_array(server->connection, sockets, &count);
        setup_poll_events(events, sockets, server->connection, count);
        timeout = server_get_poll_timeout(server);
        poll_result = zn_poll(sockets, events, count, timeout);
        if (poll_result.ready_count > 0) {
            handle_ready_sockets(server, &poll_result, sockets, count);
        }
        if (server_should_tick(server))
            process_game_tick(server);
    }
    server_timing_destroy(server);
}
