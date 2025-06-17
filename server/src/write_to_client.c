/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** write_to_client
*/

#include "../includes/server.h"

void send_to_client(client_t *client, const char *msg)
{
    int len = strlen(msg);

    if (len > 0) {
        if (zn_write(client->zn_sock, msg, len) < len) {
            int remaining = len - client->write_index;
            if (remaining > BUFFER_SIZE)
                remaining = BUFFER_SIZE;
            memcpy(client->write_buffer, msg + client->write_index, remaining);
            client->write_total = remaining;
            client->write_index = 0;
        }
        zn_flush(client->zn_sock);
    }
}

void handle_client_write(server_connection_t *connection, int client_idx)
{
    client_t *client = connection->clients[client_idx];
    ssize_t bytes;

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

    bytes = zn_flush(client->zn_sock);
    if (bytes < 0 && errno != EAGAIN && errno != EWOULDBLOCK) {
        disconnect_client(connection, client_idx);
    }
}

int strlen_custom(char *str)
{
    int len = 0;

    if (!str)
        return 0;
    while (str[len] != '\0')
        len++;
    return len;
}

void send_string_to_client(client_t *client, char *str)
{
    int len = 0;
    ssize_t write_ret = 0;

    if (!str || !client || !client->zn_sock)
        return;

    len = strlen_custom(str);
    write_ret = zn_write(client->zn_sock, str, len);
    if (write_ret < 0) {
        perror("write");
        return;
    }
    if (write_ret == 0) {
        fprintf(stderr, "Write returned 0, nothing written.\n");
        return;
    }
    if (write_ret < len)
        send_string_to_client(client, str + write_ret);

    zn_flush(client->zn_sock);
}
