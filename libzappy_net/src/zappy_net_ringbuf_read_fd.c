/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Zappy Network Library - Ring Buffer Read From FD Operations
*/

#include "../include/zappy_net_ringbuf.h"

static ssize_t read_no_wrap(zn_ringbuf_t *rb, int fd, size_t available)
{
    ssize_t result;

    result = read(fd, rb->buffer + rb->write_pos, available);
    if (result <= 0)
        return result;
    rb->write_pos += result;
    if (rb->write_pos == rb->capacity)
        rb->write_pos = 0;
    return result;
}

static ssize_t handle_wrap_read_first(zn_ringbuf_t *rb, int fd,
    size_t first_chunk)
{
    return read(fd, rb->buffer + rb->write_pos, first_chunk);
}

static ssize_t handle_wrap_read_second(zn_ringbuf_t *rb, int fd,
    size_t second_chunk, ssize_t bytes_read)
{
    ssize_t result;

    result = read(fd, rb->buffer, second_chunk);
    if (result <= 0) {
        rb->write_pos = (rb->write_pos + bytes_read) % rb->capacity;
        return bytes_read;
    }
    return bytes_read + result;
}

static ssize_t read_with_wrap(zn_ringbuf_t *rb, int fd, size_t available)
{
    ssize_t bytes_read = 0;
    ssize_t result;
    size_t first_chunk;
    size_t second_chunk;

    first_chunk = rb->capacity - rb->write_pos;
    result = handle_wrap_read_first(rb, fd, first_chunk);
    if (result <= 0)
        return result;
    bytes_read = result;
    if (bytes_read == (ssize_t)first_chunk) {
        second_chunk = available - first_chunk;
        bytes_read = handle_wrap_read_second(rb, fd, second_chunk,
            bytes_read);
        if (bytes_read > result)
            rb->write_pos = bytes_read - result;
    } else {
        rb->write_pos += bytes_read;
    }
    return bytes_read;
}

static int check_line_limit_after_read(zn_ringbuf_t *rb, ssize_t bytes_read)
{
    if (rb->line_count > MAX_QUEUED_LINES) {
        rb->write_pos = (rb->write_pos - bytes_read + rb->capacity) %
            rb->capacity;
        rb->line_count -= zn_count_newlines(rb->buffer + rb->write_pos,
            bytes_read);
        if (rb->read_pos == rb->write_pos)
            rb->is_empty = 1;
        errno = ENOBUFS;
        return -1;
    }
    return 0;
}

static int validate_read_fd_params(zn_ringbuf_t *rb, int fd)
{
    return (rb == NULL || rb->buffer == NULL || fd < 0);
}

static void update_after_read(zn_ringbuf_t *rb, ssize_t bytes_read)
{
    rb->is_empty = 0;
    rb->line_count += zn_count_newlines(rb->buffer + rb->write_pos -
        bytes_read, bytes_read);
}

ssize_t zn_ringbuf_read_from_fd(zn_ringbuf_t *rb, int fd)
{
    size_t available;
    ssize_t bytes_read;

    if (validate_read_fd_params(rb, fd))
        return -1;
    available = zn_ringbuf_write_available(rb);
    if (available == 0) {
        errno = ENOBUFS;
        return -1;
    }
    if (rb->write_pos + available <= rb->capacity)
        bytes_read = read_no_wrap(rb, fd, available);
    else
        bytes_read = read_with_wrap(rb, fd, available);
    if (bytes_read <= 0)
        return bytes_read;
    update_after_read(rb, bytes_read);
    if (check_line_limit_after_read(rb, bytes_read) < 0)
        return -1;
    return bytes_read;
}
