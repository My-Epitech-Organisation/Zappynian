/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Zappy Network Library - Ring Buffer Flush Operations
*/

#include "../include/zappy_net_ringbuf.h"

static ssize_t flush_no_wrap(zn_ringbuf_t *rb, int fd, size_t available)
{
    ssize_t result;

    result = write(fd, rb->buffer + rb->read_pos, available);
    if (result < 0)
        return -1;
    rb->read_pos += result;
    if (rb->read_pos == rb->capacity)
        rb->read_pos = 0;
    return result;
}

static ssize_t write_first_chunk(zn_ringbuf_t *rb, int fd)
{
    size_t first_chunk;

    first_chunk = rb->capacity - rb->read_pos;
    return write(fd, rb->buffer + rb->read_pos, first_chunk);
}

static ssize_t write_second_chunk(zn_ringbuf_t *rb, int fd, size_t available,
    size_t first_chunk)
{
    size_t second_chunk;

    second_chunk = available - first_chunk;
    return write(fd, rb->buffer, second_chunk);
}

static ssize_t handle_wrap_write_success(zn_ringbuf_t *rb, int fd,
    size_t available, size_t first_chunk)
{
    ssize_t result;
    ssize_t written;

    written = first_chunk;
    result = write_second_chunk(rb, fd, available, first_chunk);
    if (result < 0) {
        rb->read_pos = (rb->read_pos + written) % rb->capacity;
        return written;
    }
    written += result;
    rb->read_pos = result;
    return written;
}

static ssize_t flush_with_wrap(zn_ringbuf_t *rb, int fd, size_t available)
{
    ssize_t written = 0;
    ssize_t result;
    size_t first_chunk;

    first_chunk = rb->capacity - rb->read_pos;
    result = write_first_chunk(rb, fd);
    if (result < 0)
        return -1;
    written = result;
    if (written == (ssize_t)first_chunk) {
        return handle_wrap_write_success(rb, fd, available, first_chunk);
    } else {
        rb->read_pos += written;
    }
    return written;
}

static int validate_flush_params(zn_ringbuf_t *rb, int fd)
{
    return (rb == NULL || rb->buffer == NULL || fd < 0 || rb->is_empty);
}

static void update_after_flush(zn_ringbuf_t *rb, ssize_t written)
{
    int newlines;

    newlines = zn_count_newlines(rb->buffer + rb->read_pos - written,
        written);
    rb->line_count -= newlines;
    if (rb->read_pos == rb->write_pos)
        rb->is_empty = 1;
}

ssize_t zn_ringbuf_flush_to_fd(zn_ringbuf_t *rb, int fd)
{
    size_t available;
    ssize_t written;

    if (validate_flush_params(rb, fd))
        return -1;
    available = zn_ringbuf_read_available(rb);
    if (rb->read_pos + available <= rb->capacity)
        written = flush_no_wrap(rb, fd, available);
    else
        written = flush_with_wrap(rb, fd, available);
    if (written < 0)
        return -1;
    update_after_flush(rb, written);
    return written;
}
