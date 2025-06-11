/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Ring Buffer Utility Functions
*/

#include "../include/zappy_net_ringbuf.h"
#include <stdint.h>

int zn_count_newlines(const uint8_t *data, size_t len)
{
    int count = 0;
    size_t i;

    for (i = 0; i < len; i++) {
        if (data[i] == '\n')
            count++;
    }
    return count;
}

size_t zn_ringbuf_write_available(const zn_ringbuf_t *rb)
{
    if (rb == NULL || rb->buffer == NULL)
        return 0;
    if (rb->is_empty)
        return rb->capacity - 1;
    if (rb->write_pos >= rb->read_pos)
        return rb->capacity - (rb->write_pos - rb->read_pos) - 1;
    else
        return rb->read_pos - rb->write_pos - 1;
}

size_t zn_ringbuf_read_available(const zn_ringbuf_t *rb)
{
    if (rb == NULL || rb->buffer == NULL || rb->is_empty)
        return 0;
    if (rb->write_pos > rb->read_pos)
        return rb->write_pos - rb->read_pos;
    else
        return rb->capacity - rb->read_pos + rb->write_pos;
}

int zn_ringbuf_is_full(const zn_ringbuf_t *rb)
{
    return zn_ringbuf_write_available(rb) == 0;
}

int zn_ringbuf_is_empty(const zn_ringbuf_t *rb)
{
    return (rb == NULL || rb->buffer == NULL || rb->is_empty);
}

size_t zn_ringbuf_count_lines(const zn_ringbuf_t *rb)
{
    if (rb == NULL)
        return 0;
    return rb->line_count;
}
