/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Ring Buffer Write Operations
*/

#include "../include/zappy_net_ringbuf.h"

static ssize_t write_no_wrap(zn_ringbuf_t *rb, const uint8_t *src, size_t len)
{
    memcpy(rb->buffer + rb->write_pos, src, len);
    rb->write_pos += len;
    if (rb->write_pos == rb->capacity)
        rb->write_pos = 0;
    return len;
}

static ssize_t write_with_wrap(zn_ringbuf_t *rb, const uint8_t *src,
    size_t len)
{
    size_t first_chunk = rb->capacity - rb->write_pos;
    size_t second_chunk = len - first_chunk;

    memcpy(rb->buffer + rb->write_pos, src, first_chunk);
    memcpy(rb->buffer, src + first_chunk, second_chunk);
    rb->write_pos = second_chunk;
    return len;
}

static int validate_write_params(zn_ringbuf_t *rb, const void *data,
    size_t len)
{
    return (rb == NULL || rb->buffer == NULL || data == NULL || len == 0);
}

static int check_write_capacity(zn_ringbuf_t *rb, size_t len)
{
    size_t available;

    available = zn_ringbuf_write_available(rb);
    if (len > available) {
        errno = ENOBUFS;
        return -1;
    }
    return 0;
}

ssize_t zn_ringbuf_write(zn_ringbuf_t *rb, const void *data, size_t len)
{
    const uint8_t *src = (const uint8_t *)data;
    int newline_count;

    if (validate_write_params(rb, data, len))
        return -1;
    newline_count = zn_count_newlines(src, len);
    if (rb->line_count + newline_count > MAX_QUEUED_LINES) {
        errno = ENOBUFS;
        return -1;
    }
    if (check_write_capacity(rb, len) < 0)
        return -1;
    if (rb->write_pos + len <= rb->capacity)
        write_no_wrap(rb, src, len);
    else
        write_with_wrap(rb, src, len);
    rb->is_empty = 0;
    rb->line_count += newline_count;
    return len;
}
