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

/**
 * @brief Write data to the socket's send buffer
 *
 * This function writes data to the socket's internal send buffer.
 * The data is not immediately sent over the network but queued for
 * later transmission when zn_flush() is called.
 *
 * @param sock The socket handle
 * @param data Pointer to data to write
 * @param len Length of data in bytes
 * @return Number of bytes written, -1 on error with errno set
 */
ssize_t zn_write(zn_socket_t sock, const void *data, size_t len);

#endif /* !ZAPPY_NET_INTERNAL_H_ */
