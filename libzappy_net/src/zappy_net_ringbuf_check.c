/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Zappy Network Library - Ring Buffer Checker Functions
*/

#include "../include/zappy_net_ringbuf.h"

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
