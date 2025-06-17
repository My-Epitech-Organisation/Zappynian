/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Handshake message handling utilities
*/

#include "zappy_net.h"
#include "zappy_net_internal.h"

int zn_send_message(zn_socket_t sock, const char *message)
{
    size_t message_len = 0;
    ssize_t written = 0;

    if (sock == NULL || message == NULL) {
        return -1;
    }
    message_len = strlen(message);
    written = zn_write(sock, message, message_len);
    if (written != (ssize_t)message_len) {
        return -1;
    }
    written = zn_write(sock, "\n", 1);
    if (written != 1) {
        return -1;
    }
    if (zn_flush(sock) == -1) {
        return -1;
    }
    return 0;
}

char *zn_receive_message(zn_socket_t sock)
{
    char *line = NULL;

    if (sock == NULL) {
        return NULL;
    }
    line = zn_readline(sock);
    return line;
}
