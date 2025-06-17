/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** Server main loop with libzappy_net integration
*/

#include "../includes/server.h"
#include "../includes/game_loop.h"

static volatile bool running = true;

static void setup_socket_array(server_connection_t *connection,
                              zn_socket_t *sockets, int *count)
{
    *count = 0;

    // Add server socket first
    sockets[0] = connection->zn_server;
    *count = 1;

    // Add all active client sockets
    for (int i = 0; i < connection->client_count && *count < ZN_POLL_MAX_SOCKETS; i++) {
        if (connection->clients[i] != NULL && connection->clients[i]->zn_sock != NULL) {
            sockets[*count] = connection->clients[i]->zn_sock;
            (*count)++;
        }
    }
}

static void handle_ready_sockets(server_connection_t *connection,
                               zn_poll_result_t *result,
                               zn_socket_t *sockets, int count)
{
    // Check if server socket is readable (new connection)
    if (result->readable & 1) {
        accept_client(connection, NULL);
    }

    // Check client sockets for activity
    for (int i = 1; i < count; i++) {
        // Find the client that matches this socket
        int client_idx = -1;
        for (int j = 0; j < connection->client_count; j++) {
            if (connection->clients[j] != NULL &&
                connection->clients[j]->zn_sock == sockets[i]) {
                client_idx = j;
                break;
            }
        }

        if (client_idx == -1) {
            continue; // Couldn't find client for this socket
        }

        // Handle readable sockets
        if (result->readable & (1ULL << i)) {
            handle_client_read(connection, client_idx);
        }

        // Handle writable sockets
        if (result->writable & (1ULL << i)) {
            handle_client_write(connection, client_idx);
        }

        // Handle socket errors
        if (result->error & (1ULL << i)) {
            disconnect_client(connection, client_idx);
        }
    }
}

void server_loop(server_t *server)
{
    zn_socket_t sockets[ZN_POLL_MAX_SOCKETS];
    short events[ZN_POLL_MAX_SOCKETS];
    int count = 0;
    zn_poll_result_t poll_result;

    // Initialize client array if needed
    if (server->connection->clients == NULL) {
        server->connection->clients = calloc(MAX_CLIENTS, sizeof(client_t *));
        if (server->connection->clients == NULL) {
            return;
        }
        server->connection->client_count = 0;
    }

    while (running && server->game_running) {
        // Setup sockets array for polling
        setup_socket_array(server->connection, sockets, &count);

        // Set all events to check for read, write, and errors
        for (int i = 0; i < count; i++) {
            events[i] = POLLIN;

            // For client sockets, also check for write readiness if needed
            if (i > 0) {
                for (int j = 0; j < server->connection->client_count; j++) {
                    if (server->connection->clients[j] != NULL &&
                        server->connection->clients[j]->zn_sock == sockets[i] &&
                        server->connection->clients[j]->write_total > 0) {
                        events[i] |= POLLOUT;
                        break;
                    }
                }
            }
        }

        // Poll with timeout (100ms)
        poll_result = zn_poll(sockets, events, count, 100);

        // Process network events if any sockets are ready
        if (poll_result.ready_count > 0) {
            handle_ready_sockets(server->connection, &poll_result, sockets, count);
        }

        // Process game logic
        game_loop_tick(server);
        process_commands(server);
        decrement_food_for_all_players(server);
        death_check(server->players, server->player_count, server->map, server);
        check_victory(server);
    }
}

void stop_server_loop(void)
{
    running = false;
}
