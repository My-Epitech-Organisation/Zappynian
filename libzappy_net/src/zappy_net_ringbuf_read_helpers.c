/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Zappy Network Library - Ring Buffer Read Helper Functions
*/

#include "../include/zappy_net_ringbuf.h"

ssize_t handle_wrap_read_first(zn_ringbuf_t *rb, int fd, size_t first_chunk)
{
    return read(fd, rb->buffer + rb->write_pos, first_chunk);
}

ssize_t handle_wrap_read_second(zn_ringbuf_t *rb, int fd, size_t second_chunk,
    ssize_t first_bytes_read)
{
    ssize_t result;

    result = read(fd, rb->buffer, second_chunk);
    if (result <= 0) {
        return first_bytes_read;
    }
    return first_bytes_read + result;
}

void update_write_pos_after_wrap(zn_ringbuf_t *rb, ssize_t bytes_read,
    size_t first_chunk, ssize_t result)
{
    if (result > bytes_read) {
        rb->write_pos = result - first_chunk;
    } else {
        rb->write_pos = 0;
    }
}

int validate_read_fd_params(zn_ringbuf_t *rb, int fd)
{
    return (rb == NULL || rb->buffer == NULL || fd < 0);
}

int check_line_limit_after_read(zn_ringbuf_t *rb, ssize_t bytes_read)
{
    (void)bytes_read;
    if (rb->line_count > MAX_QUEUED_LINES) {
        errno = ENOBUFS;
        return -1;
    }
    return 0;
}
