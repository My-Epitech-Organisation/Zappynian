/*
** EPITECH PROJECT, 2025
** Jetpack
** File description:
** Error handling functions
*/

#include "../include/server.h"

void close_connection(server_connection_t *connection)
{
    if (connection == NULL) {
        fprintf(stderr, "Connection is NULL.\n");
        return;
    }
    if (connection->fd != -1) {
        close(connection->fd);
    }
    free(connection->clients);
    free(connection->fds);
    free(connection);
}

void handle_error_connection(char *msg, server_connection_t *connection)
{
    close_connection(connection);
    perror(msg);
    exit(EXIT_FAILURE);
}

int check_correct_read(server_connection_t *connection, int idx,
    ssize_t bytes_read, client_t *client)
{
    if (bytes_read <= 0) {
        if (bytes_read < 0)
            perror("read");
        else
            printf("Client %d disconnected.\n", idx);
        disconnect_client(connection, idx);
        return 84;
    }
    if (client->read_index + bytes_read >= BUFFER_SIZE) {
        fprintf(stderr, "Buffer overflow for client %d\n", idx);
        disconnect_client(connection, idx);
        return 84;
    }
    return 0;
}
