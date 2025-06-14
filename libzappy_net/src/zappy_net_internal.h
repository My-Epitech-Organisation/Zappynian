/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Zappy Network Library - Internal Header
*/

#ifndef ZAPPY_NET_INTERNAL_H_
    #define ZAPPY_NET_INTERNAL_H_

    #include "../include/zappy_net.h"
    #include "../include/zappy_net_ringbuf.h"
    #include <netinet/in.h>
    #include <string.h>
    #include <stdio.h>
    #include <stdlib.h>

/**
 * @brief Internal socket structure definition
 */
struct zn_socket {
    int fd;
    int initialized;
    struct sockaddr_in addr;
    int type;
    zn_ringbuf_t read_buffer;  /* Buffer for reading from socket */
    zn_ringbuf_t write_buffer; /* Buffer for writing to socket */
    int buffer_initialized;    /* Flag to track if buffers are initialized */
};

/**
 * @brief Internal handshake utility functions
 */
int zn_send_message(zn_socket_t sock, const char *message);
char *zn_receive_message(zn_socket_t sock);

#endif /* !ZAPPY_NET_INTERNAL_H_ */
