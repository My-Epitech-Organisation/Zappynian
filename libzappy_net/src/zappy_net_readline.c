/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Zappy Network Library - Readline Functions
*/

#include "zappy_net_internal.h"
#include <stdlib.h>
#include <string.h>
#include <errno.h>

static char *allocate_line_buffer(size_t line_length)
{
    char *result;

    result = malloc(line_length + 1);
    if (result == NULL) {
        errno = ENOMEM;
        return NULL;
    }
    return result;
}

static char *copy_line_content(const char *source, size_t length)
{
    char *result;

    result = allocate_line_buffer(length);
    if (result == NULL)
        return NULL;
    strncpy(result, source, length);
    result[length] = '\0';
    return result;
}

static char *extract_line_from_buffer(zn_socket_t sock)
{
    char temp_buffer[DEFAULT_BUFFER_SIZE];
    ssize_t bytes_read;
    char *newline_pos;
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
    return copy_line_content(temp_buffer, line_length);
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

static int validate_readline_params(zn_socket_t sock)
{
    if (sock == NULL || !sock->initialized) {
        errno = EINVAL;
        return -1;
    }
    return 0;
}

static int init_buffers_for_readline(zn_socket_t sock)
{
    if (zn_ringbuf_init(&sock->read_buffer, 0) < 0)
        return -1;
    if (zn_ringbuf_init(&sock->write_buffer, 0) < 0) {
        zn_ringbuf_cleanup(&sock->read_buffer);
        return -1;
    }
    sock->buffer_initialized = 1;
    return 0;
}

static int ensure_buffers_ready(zn_socket_t sock)
{
    if (!sock->buffer_initialized) {
        if (init_buffers_for_readline(sock) < 0) {
            errno = ENOMEM;
            return -1;
        }
    }
    return 0;
}

char *zn_readline(zn_socket_t sock)
{
    int read_result;

    if (validate_readline_params(sock) < 0)
        return NULL;
    if (ensure_buffers_ready(sock) < 0)
        return NULL;
    if (zn_ringbuf_count_lines(&sock->read_buffer) == 0) {
        read_result = try_read_more_data(sock);
        if (read_result < 0)
            return NULL;
        if (read_result == 0
            && zn_ringbuf_count_lines(&sock->read_buffer) == 0)
            return NULL;
    }
    return extract_line_from_buffer(sock);
}
