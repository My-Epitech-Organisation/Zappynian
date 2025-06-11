/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Zappy Network Library - Ring Buffer Read Operations
*/

#include "../include/zappy_net_ringbuf.h"
#include <string.h>

static void read_no_wrap(zn_ringbuf_t *rb, uint8_t *dst, size_t len)
{
    memcpy(dst, rb->buffer + rb->read_pos, len);
    rb->read_pos += len;
    if (rb->read_pos == rb->capacity)
        rb->read_pos = 0;
}

static void read_with_wrap(zn_ringbuf_t *rb, uint8_t *dst, size_t len)
{
    size_t first_chunk;
    size_t second_chunk;

    first_chunk = rb->capacity - rb->read_pos;
    second_chunk = len - first_chunk;
    memcpy(dst, rb->buffer + rb->read_pos, first_chunk);
    memcpy(dst + first_chunk, rb->buffer, second_chunk);
    rb->read_pos = second_chunk;
}

ssize_t zn_ringbuf_read(zn_ringbuf_t *rb, void *data, size_t len)
{
    size_t available;
    uint8_t *dst = (uint8_t *)data;
    int newline_count;

    if (rb == NULL || rb->buffer == NULL || data == NULL || len == 0 ||
        rb->is_empty)
        return 0;
    available = zn_ringbuf_read_available(rb);
    len = (len < available) ? len : available;
    if (rb->read_pos + len <= rb->capacity) {
        newline_count = zn_count_newlines(rb->buffer + rb->read_pos, len);
        read_no_wrap(rb, dst, len);
    } else {
        newline_count = zn_count_newlines(rb->buffer + rb->read_pos,
            rb->capacity - rb->read_pos);
        newline_count += zn_count_newlines(rb->buffer,
            len - (rb->capacity - rb->read_pos));
        read_with_wrap(rb, dst, len);
    }
    rb->line_count -= newline_count;
    if (rb->read_pos == rb->write_pos)
        rb->is_empty = 1;
    return len;
}

ssize_t zn_ringbuf_read_line(zn_ringbuf_t *rb, void *data, size_t max_len)
{
    size_t available;
    size_t i, pos, bytes_read = 0;
    uint8_t *dst = (uint8_t *)data;
    uint8_t c;
    int found_newline = 0;

    if (rb == NULL || rb->buffer == NULL || data == NULL || max_len == 0 ||
        rb->is_empty)
        return -1;
    available = zn_ringbuf_read_available(rb);
    max_len = (max_len < available) ? max_len : available;
    pos = rb->read_pos;
    for (i = 0; i < max_len; i++) {
        c = rb->buffer[pos];
        dst[bytes_read++] = c;
        pos = (pos + 1) % rb->capacity;
        if (c == '\n') {
            found_newline = 1;
            rb->line_count--;
            rb->read_pos = pos;
            break;
        }
    }
    if (rb->read_pos == rb->write_pos)
        rb->is_empty = 1;
    return found_newline ? bytes_read : -1;
}
