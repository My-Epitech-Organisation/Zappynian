/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Ring Buffer Core Functions
*/

#include "../include/zappy_net_ringbuf.h"

#define DEFAULT_BUFFER_SIZE (64 * 1024)

int zn_ringbuf_init(zn_ringbuf_t *rb, size_t capacity)
{
    if (rb == NULL)
        return -1;
    capacity = (capacity > 0) ? capacity : DEFAULT_BUFFER_SIZE;
    rb->buffer = malloc(capacity);
    if (rb->buffer == NULL)
        return -1;
    rb->capacity = capacity;
    rb->read_pos = 0;
    rb->write_pos = 0;
    rb->is_empty = 1;
    rb->line_count = 0;
    return 0;
}

void zn_ringbuf_cleanup(zn_ringbuf_t *rb)
{
    if (rb == NULL)
        return;
    free(rb->buffer);
    rb->buffer = NULL;
    rb->capacity = 0;
    rb->read_pos = 0;
    rb->write_pos = 0;
    rb->is_empty = 1;
    rb->line_count = 0;
}
