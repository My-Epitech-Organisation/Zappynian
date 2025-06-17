/*
** EPITECH PROJECT, 2025
** Jetpack
** File description:
** Client connection handling with libzappy_net integration
*/

#include "../includes/server.h"

static int setup_client_socket(client_t *client, zn_socket_t zn_sock)
{
    client->zn_sock = zn_sock;
    client->fd = zn_get_fd(zn_sock);
    if (client->fd == -1) {
        return -1;
    }
    return 0;
}

static int configure_client_properties(client_t *client)
{
    if (zn_set_nonblocking(client->zn_sock, 1) == -1) {
        return -1;
    }
    client->type = CLIENT_UNKNOWN;
    return 0;
}

static void cleanup_failed_client(client_t *client)
{
    if (client->zn_sock != NULL) {
        zn_close(client->zn_sock);
        client->zn_sock = NULL;
    }
    client->fd = -1;
}

static int find_free_client_slot(client_t **clients, int max_clients)
{
    for (int i = 0; i < max_clients; i++) {
        if (clients[i] == NULL || clients[i]->fd == -1) {
            return i;
        }
    }
    return -1;
}

static client_t *create_new_client(zn_socket_t zn_sock)
{
    client_t *client = malloc(sizeof(client_t));

    if (client == NULL) {
        return NULL;
    }
    memset(client, 0, sizeof(client_t));
    if (setup_client_socket(client, zn_sock) == -1) {
        free(client);
        return NULL;
    }
    if (configure_client_properties(client) == -1) {
        cleanup_failed_client(client);
        free(client);
        return NULL;
    }
    return client;
}

void accept_client(server_connection_t *connection, server_args_t *unused)
{
    int slot = find_free_client_slot(connection->clients, MAX_CLIENTS);
    zn_socket_t new_sock = NULL;
    client_t *new_client = NULL;

    (void)unused;
    if (slot == -1) {
        return;
    }
    new_sock = zn_accept(connection->zn_server, NULL, NULL);
    if (new_sock == NULL) {
        return;
    }
    new_client = create_new_client(new_sock);
    if (new_client == NULL) {
        zn_close(new_sock);
        return;
    }
    connection->clients[slot] = new_client;
    if (slot >= connection->client_count) {
        connection->client_count = slot + 1;
    }
    assign_client_type(new_client, connection, slot);
}

static void process_server_events(server_connection_t *connection)
    __attribute__((unused));
    
static void process_server_events(server_connection_t *connection)
{
    // This function is now a no-op since polling is done in server_loop.c
    (void)connection; // Avoid unused parameter warning
}

// Removed setup_poll_events - this functionality is now in server_loop.c

// Removed process_poll_results - this functionality is now in server_loop.c

static void process_client_events(server_connection_t *connection)
    __attribute__((unused));
    
static void process_client_events(server_connection_t *connection)
{
    // This function is now a no-op since polling is done in server_loop.c
    (void)connection; // Avoid unused parameter warning
}

static void initialize_client_array(server_connection_t *connection)
    __attribute__((unused));
    
static void initialize_client_array(server_connection_t *connection)
{
    connection->clients = calloc(MAX_CLIENTS, sizeof(client_t *));
    if (connection->clients == NULL) {
        handle_error_connection("calloc", connection);
        return;
    }
    connection->client_count = 0;
}

void handle_clients(server_t *server)
{
    // This function is kept for backward compatibility but is now a no-op
    // The actual network handling is done in server_loop.c
    (void)server; // Avoid unused parameter warning
    
    // Note: We've kept this function in case there are external calls to it
    // that we don't want to break, but it no longer does anything since
    // the functionality has been moved to server_loop.c
}
