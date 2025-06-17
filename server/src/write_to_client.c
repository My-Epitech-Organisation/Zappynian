/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** write_to_client
*/

#include "../includes/server.h"

void send_to_client(client_t *client, const char *msg)
{
    // We'll use the client's write_buffer as a staging area
    // before calling zn_write in handle_client_write
    int len = strlen(msg);

    if (len > BUFFER_SIZE)
        len = BUFFER_SIZE;
    memcpy(client->write_buffer, msg, len);
    client->write_total = len;
    client->write_index = 0;
}

void handle_client_write(server_connection_t *connection, int client_idx)
{
    client_t *client = connection->clients[client_idx];
    ssize_t bytes;

    // First, write any pending data to the socket's internal buffer
    if (client->write_total > 0) {
        bytes = zn_write(client->zn_sock,
            client->write_buffer + client->write_index,
            client->write_total - client->write_index);

        if (bytes < 0) {
            if (errno != EAGAIN && errno != EWOULDBLOCK) {
                disconnect_client(connection, client_idx);
            }
            return;
        }

        client->write_index += bytes;
        if (client->write_index >= client->write_total) {
            client->write_index = 0;
            client->write_total = 0;
        }
    }

    // Then flush the data to the network
    bytes = zn_flush(client->zn_sock);
    if (bytes < 0 && errno != EAGAIN && errno != EWOULDBLOCK) {
        disconnect_client(connection, client_idx);
    }
}

int strlen_fd(char *str)
{
    int len = 0;

    if (!str)
        return 0;
    while (str[len] != '\0')
        len++;
    return len;
}

/**
 * Helper function to find a client from fd
 * Note: this is currently unused as we're still transitioning
 * from direct fd usage to using zn_socket_t.
 */
static client_t *find_client_by_fd(server_connection_t *connection, int fd)
    __attribute__((unused));

static client_t *find_client_by_fd(server_connection_t *connection, int fd)
{
    for (int i = 0; i < connection->client_count; i++) {
        if (connection->clients[i] && connection->clients[i]->fd == fd) {
            return connection->clients[i];
        }
    }
    return NULL;
}

void put_str_fd(int fd, char *str)
{
    // Note: This function is now less efficient as we have to look up
    // the zn_socket_t by fd. In production code, we might want to
    // change the function signature to accept zn_socket_t directly.

    int len = 0;
    ssize_t write_ret = 0;

    if (!str)
        return;
    len = strlen_fd(str);

    // We'll continue using write() for now since we don't have access to
    // the server_connection_t here to find the client by fd
    write_ret = write(fd, str, len);
    if (write_ret < 0) {
        perror("write");
        return;
    }
    if (write_ret == 0) {
        fprintf(stderr, "Write returned 0, nothing written.\n");
        return;
    }
    if (write_ret < len)
        put_str_fd(fd, str + write_ret);
}
