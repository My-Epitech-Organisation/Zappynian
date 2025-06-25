/*
** EPITECH PROJECT, 2025
** Jetpack
** File description:
** Client connection handling with libzappy_net integration
*/

#include "../include/server.h"

static int configure_client_properties(client_t *client)
{
    static int client_id = 0;

    client_id++;
    client->id = client_id;
    if (zn_set_nonblocking(client->zn_sock, 1) == -1) {
        return -1;
    }
    client->type = CLIENT_UNKNOWN;
    return 0;
}

static void cleanup_failed_client(client_t *client)
{
    cleanup_client_zappy_socket(client);
}

static int find_free_client_slot(client_t **clients, int max_clients)
{
    for (int i = 0; i < max_clients; i++) {
        if (clients[i] == NULL || clients[i]->zn_sock == NULL) {
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
    init_client_zappy_socket(client, zn_sock);
    if (configure_client_properties(client) == -1) {
        cleanup_failed_client(client);
        free(client);
        return NULL;
    }
    return client;
}

static void finalize_client_connection(server_connection_t *connection,
    int slot, client_t *new_client)
{
    connection->clients[slot] = new_client;
    if (slot >= connection->client_count) {
        connection->client_count = slot + 1;
    }
}

static int send_welcome_to_client(client_t *client,
    server_connection_t *connection, int slot)
{
    if (zn_send_welcome(client->zn_sock) != 0) {
        disconnect_client(connection, slot);
        return -1;
    }
    return 0;
}

void accept_client(server_connection_t *connection, server_args_t *unused)
{
    int slot = find_free_client_slot(connection->clients, MAX_CLIENTS);
    zn_socket_t new_sock = NULL;
    client_t *new_client = NULL;

    (void)unused;
    if (slot == -1)
        return;
    new_sock = zn_accept(connection->zn_server, NULL, NULL);
    if (new_sock == NULL)
        return;
    new_client = create_new_client(new_sock);
    if (new_client == NULL) {
        zn_close(new_sock);
        return;
    }
    finalize_client_connection(connection, slot, new_client);
    send_welcome_to_client(new_client, connection, slot);
}
