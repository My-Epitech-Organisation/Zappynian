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

void accept_client(server_connection_t *connection, client_t *unused)
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
{
    if (connection->client_count < MAX_CLIENTS) {
        accept_client(connection, NULL);
    }
}

static void setup_poll_events(server_connection_t *connection,
                             zn_socket_t *sockets, short *events, int *count)
{
    *count = 0;

    // Add server socket for accepting new clients
    if (connection->client_count < MAX_CLIENTS) {
        sockets[*count] = connection->zn_server;
        events[*count] = POLLIN;
        (*count)++;
    }

    // Add client sockets for reading and writing
    for (int i = 0; i < connection->client_count && *count < ZN_POLL_MAX_SOCKETS; i++) {
        if (connection->clients[i] != NULL && connection->clients[i]->fd != -1) {
            sockets[*count] = connection->clients[i]->zn_sock;
            events[*count] = POLLIN;

            // If client has pending data to write, also check for write readiness
            if (connection->clients[i]->write_total > 0) {
                events[*count] |= POLLOUT;
            }

            (*count)++;
        }
    }
}

static void process_poll_results(server_connection_t *connection,
                               zn_socket_t *sockets, zn_poll_result_t *result, int count)
{
    // Check server socket for new connections
    if (count > 0 && (result->readable & 1) && connection->client_count < MAX_CLIENTS) {
        accept_client(connection, NULL);
    }

    // Check client sockets for read/write events
    int client_offset = (connection->client_count < MAX_CLIENTS) ? 1 : 0;
    int client_idx = 0;

    for (int i = client_offset; i < count; i++) {
        // Find the client index for this socket
        for (client_idx = 0; client_idx < connection->client_count; client_idx++) {
            if (connection->clients[client_idx] != NULL &&
                connection->clients[client_idx]->zn_sock == sockets[i]) {
                break;
            }
        }

        if (client_idx >= connection->client_count) {
            continue; // Couldn't find client for this socket
        }

        // Check for readable sockets
        if (result->readable & (1ULL << i)) {
            handle_client_read(connection, client_idx);
        }

        // Check for writable sockets
        if (result->writable & (1ULL << i)) {
            handle_client_write(connection, client_idx);
        }

        // Check for error condition
        if (result->error & (1ULL << i)) {
            disconnect_client(connection, client_idx);
        }
    }
}

static void process_client_events(server_connection_t *connection)
{
    zn_socket_t sockets[ZN_POLL_MAX_SOCKETS];
    short events[ZN_POLL_MAX_SOCKETS];
    int count = 0;
    zn_poll_result_t poll_result;

    // Setup the poll arrays
    setup_poll_events(connection, sockets, events, &count);

    if (count == 0) {
        return; // Nothing to poll
    }

    // Poll with a small timeout (10ms)
    poll_result = zn_poll(sockets, events, count, 10);

    // Process poll results
    if (poll_result.ready_count > 0) {
        process_poll_results(connection, sockets, &poll_result, count);
    }
}

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
    static bool initialized = false;
    if (!initialized) {
        initialize_client_array(server->connection);
        initialized = true;
    }
    process_server_events(server->connection);
    process_client_events(server->connection);
}
