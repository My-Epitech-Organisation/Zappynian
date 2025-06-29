/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Zappy Network Library - Socket I/O Functions
*/

#include "zappy_net_internal.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

static int init_socket_buffers_if_needed(zn_socket_t sock)
{
    if (sock == NULL || !sock->initialized)
        return -1;
    if (!sock->buffer_initialized) {
        if (zn_ringbuf_init(&sock->read_buffer, 0) < 0)
            return -1;
        if (zn_ringbuf_init(&sock->write_buffer, 0) < 0) {
            zn_ringbuf_cleanup(&sock->read_buffer);
            return -1;
        }
        sock->buffer_initialized = 1;
    }
    return 0;
}

static int validate_socket_params(zn_socket_t sock, void *data, size_t len)
{
    if (sock == NULL || !sock->initialized || data == NULL || len == 0) {
        errno = EINVAL;
        return -1;
    }
    return 0;
}

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
ssize_t zn_write(zn_socket_t sock, const void *data, size_t len)
{
    if (validate_socket_params(sock, (void *)data, len) < 0)
        return -1;
    if (init_socket_buffers_if_needed(sock) < 0) {
        errno = ENOMEM;
        return -1;
    }
    return zn_ringbuf_write(&sock->write_buffer, data, len);
}

/**
 * @brief Flush the socket's send buffer to the network
 *
 * This function attempts to send all data in the socket's send buffer
 * to the network. It may not send all data in a single call if the network
 * is not ready to accept more data.
 *
 * @param sock The socket handle
 * @return Number of bytes flushed, -1 on error with errno set
 */
ssize_t zn_flush(zn_socket_t sock)
{
    if (sock == NULL || !sock->initialized) {
        errno = EINVAL;
        return -1;
    }
    if (!sock->buffer_initialized ||
        zn_ringbuf_is_empty(&sock->write_buffer)) {
        return 0;
    }
    return zn_ringbuf_flush_to_fd(&sock->write_buffer, sock->fd);
}

static int is_broken_pipe_error(void)
{
    return (errno == EPIPE || errno == ECONNRESET || errno == ENOTCONN);
}

/**
 * @brief Check if there are pending writes in the socket's send buffer
 *
 * This function checks if there is any data pending in the socket's
 * send buffer that has not yet been sent over the network.
 *
 * @param sock The socket handle
 * @return 1 if there are pending writes, 0 if not, -1 on error with errno set
 */
int zn_has_pending_writes(zn_socket_t sock)
{
    if (sock == NULL || !sock->initialized)
        return 0;
    if (!sock->buffer_initialized)
        return 0;
    return !zn_ringbuf_is_empty(&sock->write_buffer);
}
