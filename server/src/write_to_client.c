/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** write_to_client
*/

#include "../include/server.h"

// void send_to_client(client_t *client, const char *msg)
// {
//     int len = strlen(msg);

//     if (len > BUFFER_SIZE)
//         len = BUFFER_SIZE;
//     memcpy(client->write_buffer, msg, len);
//     client->write_total = len;
//     client->write_index = 0;
// }

void handle_client_write(server_connection_t *connection, int client_idx)
{
    client_t *client = connection->clients[client_idx];
    int bytes = write(client->fd,
        client->write_buffer + client->write_index,
        client->write_total - client->write_index);

    if (bytes <= 0) {
        disconnect_client(connection, client_idx);
        return;
    }
    client->write_index += bytes;
    if (client->write_index >= client->write_total) {
        client->write_index = 0;
        client->write_total = 0;
    }
}

// int strlen_fd(char *str)
// {
//     int len = 0;

//     if (!str)
//         return 0;
//     while (str[len] != '\0')
//         len++;
//     return len;
// }

// void put_str_fd(int fd, char *str)
// {
//     int len = 0;
//     int write_ret = 0;

//     if (!str)
//         return;
//     len = strlen_fd(str);
//     write_ret = write(fd, str, len);
//     if (write_ret < 0) {
//         perror("write");
//         return;
//     }
//     if (write_ret == 0) {
//         fprintf(stderr, "Write returned 0, nothing written.\n");
//         return;
//     }
//     if (write_ret < len)
//         put_str_fd(fd, str + write_ret);
// }
