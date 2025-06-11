/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Zappy Network Library - Ring Buffer I/O Operations
*/

#include "../include/zappy_net_ringbuf.h"

#define MAX_QUEUED_LINES 10

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

static ssize_t flush_with_wrap(zn_ringbuf_t *rb, int fd, size_t available)
{
    ssize_t written = 0;
    ssize_t result;
    size_t first_chunk, second_chunk;

    first_chunk = rb->capacity - rb->read_pos;
    result = write(fd, rb->buffer + rb->read_pos, first_chunk);
    if (result < 0)
        return -1;
    written = result;
    if (written == (ssize_t)first_chunk) {
        second_chunk = available - first_chunk;
        result = write(fd, rb->buffer, second_chunk);
        if (result < 0) {
            rb->read_pos = (rb->read_pos + written) % rb->capacity;
            return written;
        }
        written += result;
        rb->read_pos = result;
    } else {
        rb->read_pos += written;
    }
    return written;
}

ssize_t zn_ringbuf_flush_to_fd(zn_ringbuf_t *rb, int fd)
{
    size_t available;
    ssize_t written;
    int newlines;

    if (rb == NULL || rb->buffer == NULL || fd < 0 || rb->is_empty)
        return -1;
    available = zn_ringbuf_read_available(rb);
    if (rb->read_pos + available <= rb->capacity)
        written = flush_no_wrap(rb, fd, available);
    else
        written = flush_with_wrap(rb, fd, available);
    if (written < 0)
        return -1;
    newlines = zn_count_newlines(rb->buffer + rb->read_pos - written,
        written);
    rb->line_count -= newlines;
    if (rb->read_pos == rb->write_pos)
        rb->is_empty = 1;
    return written;
}

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

static ssize_t read_with_wrap(zn_ringbuf_t *rb, int fd, size_t available)
{
    ssize_t bytes_read = 0;
    ssize_t result;
    size_t first_chunk, second_chunk;

    first_chunk = rb->capacity - rb->write_pos;
    result = read(fd, rb->buffer + rb->write_pos, first_chunk);
    if (result <= 0)
        return result;
    bytes_read = result;
    if (bytes_read == (ssize_t)first_chunk) {
        second_chunk = available - first_chunk;
        result = read(fd, rb->buffer, second_chunk);
        if (result <= 0) {
            rb->write_pos = (rb->write_pos + bytes_read) % rb->capacity;
            return bytes_read;
        }
        bytes_read += result;
        rb->write_pos = result;
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

ssize_t zn_ringbuf_read_from_fd(zn_ringbuf_t *rb, int fd)
{
    size_t available;
    ssize_t bytes_read;

    if (rb == NULL || rb->buffer == NULL || fd < 0)
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
    rb->is_empty = 0;
    rb->line_count += zn_count_newlines(rb->buffer + rb->write_pos -
        bytes_read, bytes_read);
    if (check_line_limit_after_read(rb, bytes_read) < 0)
        return -1;
    return bytes_read;
}
