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

static ssize_t try_read_from_socket(zn_socket_t sock)
{
    ssize_t result;

    result = zn_ringbuf_read_from_fd(&sock->read_buffer, sock->fd);
    if (result < 0) {
        if (errno == EAGAIN || errno == EWOULDBLOCK)
            return 0;
        return -1;
    } else if (result == 0) {
        return 0;
    }
    return result;
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
 * @brief Read data from the socket's receive buffer
 *
 * This function reads data from the socket's internal receive buffer.
 * If the buffer is empty, it attempts to read from the socket into
 * the buffer first. This function does not block if no data is available.
 *
 * @param sock The socket handle
 * @param data Buffer to store read data
 * @param len Maximum amount of data to read
 * @return Number of bytes read, -1 on error with errno set,
 *         0 if no data available
 */
ssize_t zn_read(zn_socket_t sock, void *data, size_t len)
{
    ssize_t result;

    if (validate_socket_params(sock, data, len) < 0)
        return -1;
    if (init_socket_buffers_if_needed(sock) < 0) {
        errno = ENOMEM;
        return -1;
    }
    if (zn_ringbuf_is_empty(&sock->read_buffer)) {
        result = try_read_from_socket(sock);
        if (result <= 0)
            return result;
    }
    return zn_ringbuf_read(&sock->read_buffer, data, len);
}

/**
 * @brief Read a line from the socket's receive buffer
 *
 * This function reads a complete line (ending with '\n') from the socket's
 * internal receive buffer. If no complete line is available, it attempts to
 * read from the socket into the buffer first.
 *
 * @param sock The socket handle
 * @param data Buffer to store read data
 * @param len Maximum amount of data to read
 * @return Number of bytes read (including newline), -1 on error with errno set
 *         or if no complete line is available
 */
static ssize_t try_read_line_from_socket(zn_socket_t sock)
{
    ssize_t result;

    result = zn_ringbuf_read_from_fd(&sock->read_buffer, sock->fd);
    if (result < 0) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            errno = EAGAIN;
            return -1;
        }
        return -1;
    } else if (result == 0) {
        errno = EAGAIN;
        return -1;
    }
    return result;
}

ssize_t zn_readln(zn_socket_t sock, void *data, size_t len)
{
    ssize_t result;

    if (validate_socket_params(sock, data, len) < 0)
        return -1;
    if (init_socket_buffers_if_needed(sock) < 0) {
        errno = ENOMEM;
        return -1;
    }
    if (zn_ringbuf_count_lines(&sock->read_buffer) == 0) {
        result = try_read_line_from_socket(sock);
        if (result < 0)
            return -1;
    }
    return zn_ringbuf_read_line(&sock->read_buffer, data, len);
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

/**
 * @brief Check if the socket has data available to read
 *
 * This function checks if there is data available in the socket's
 * internal buffer or if the socket is ready for reading.
 *
 * @param sock The socket handle
 * @return 1 if data is available, 0 if not, -1 on error with errno set
 */
static int check_socket_ready_for_reading(int fd)
{
    struct pollfd pfd;
    int result;

    pfd.fd = fd;
    pfd.events = POLLIN;
    pfd.revents = 0;
    result = poll(&pfd, 1, 0);
    if (result < 0)
        return -1;
    return (pfd.revents & POLLIN) ? 1 : 0;
}

int zn_has_data(zn_socket_t sock)
{
    if (sock == NULL || !sock->initialized) {
        errno = EINVAL;
        return -1;
    }
    if (!sock->buffer_initialized)
        return 0;
    if (!zn_ringbuf_is_empty(&sock->read_buffer))
        return 1;
    return check_socket_ready_for_reading(sock->fd);
}

static char *extract_line_from_buffer(zn_socket_t sock)
{
    char temp_buffer[DEFAULT_BUFFER_SIZE];
    ssize_t bytes_read;
    char *newline_pos;
    char *result;
    size_t line_length;

    bytes_read = zn_ringbuf_read_line(&sock->read_buffer, temp_buffer,
        DEFAULT_BUFFER_SIZE - 1);
    if (bytes_read <= 0)
        return NULL;
    temp_buffer[bytes_read] = '\0';
    newline_pos = strchr(temp_buffer, '\n');
    if (newline_pos == NULL)
        return NULL;
    line_length = newline_pos - temp_buffer;
    result = malloc(line_length + 1);
    if (result == NULL) {
        errno = ENOMEM;
        return NULL;
    }
    strncpy(result, temp_buffer, line_length);
    result[line_length] = '\0';
    return result;
}

static int try_read_more_data(zn_socket_t sock)
{
    ssize_t result;

    result = zn_ringbuf_read_from_fd(&sock->read_buffer, sock->fd);
    if (result < 0) {
        if (errno == EAGAIN || errno == EWOULDBLOCK)
            return 0;
        return -1;
    }
    return (result > 0) ? 1 : 0;
}

char *zn_readline(zn_socket_t sock)
{
    char *result;
    int read_result;

    if (sock == NULL || !sock->initialized) {
        errno = EINVAL;
        return NULL;
    }
    if (init_socket_buffers_if_needed(sock) < 0) {
        errno = ENOMEM;
        return NULL;
    }
    if (zn_ringbuf_count_lines(&sock->read_buffer) == 0) {
        read_result = try_read_more_data(sock);
        if (read_result < 0)
            return NULL;
        if (read_result == 0
            && zn_ringbuf_count_lines(&sock->read_buffer) == 0)
            return NULL;
    }
    result = extract_line_from_buffer(sock);
    return result;
}
