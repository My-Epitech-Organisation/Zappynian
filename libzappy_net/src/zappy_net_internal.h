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


/**
 * @brief Read a complete line from socket with partial segment support
 *
 * This function reads a complete newline-terminated line from the socket.
 * It handles TCP fragmentation by accumulating partial segments until
 * a complete line is available. Returns dynamically allocated string
 * containing the complete line (without newline) or NULL if no complete
 * line is available.
 *
 * @param sock The socket handle
 * @return Dynamically allocated string with complete line (caller must
 *         free), or NULL if no complete line available or on error
 */
char *zn_readline(zn_socket_t sock);


#endif /* !ZAPPY_NET_INTERNAL_H_ */
