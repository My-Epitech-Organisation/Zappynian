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

static ssize_t handle_second_chunk_read(read_wrap_params_t *params)
{
    size_t second_chunk;
    ssize_t result;

    second_chunk = params->available - params->first_chunk;
    result = handle_wrap_read_second(params->rb, params->fd, second_chunk,
        params->bytes_read);
    update_write_pos_after_wrap(params->rb, params->bytes_read,
        params->first_chunk, result);
    return (result > params->bytes_read) ? result : params->bytes_read;
}

static ssize_t read_with_wrap(zn_ringbuf_t *rb, int fd, size_t available)
{
    read_wrap_params_t params;
    ssize_t result;

    params.rb = rb;
    params.fd = fd;
    params.available = available;
    params.first_chunk = rb->capacity - rb->write_pos;
    result = handle_wrap_read_first(rb, fd, params.first_chunk);
    if (result <= 0)
        return result;
    params.bytes_read = result;
    if (params.bytes_read == (ssize_t)params.first_chunk &&
        available > params.first_chunk) {
        return handle_second_chunk_read(&params);
    }
    rb->write_pos += params.bytes_read;
    return params.bytes_read;
}

static void update_after_read(zn_ringbuf_t *rb, ssize_t bytes_read,
    size_t old_write_pos)
{
    size_t newlines_count = 0;
    size_t first_chunk = 0;
    size_t second_chunk = 0;

    rb->is_empty = 0;
    if (old_write_pos + bytes_read <= rb->capacity) {
        newlines_count = zn_count_newlines(rb->buffer + old_write_pos,
            bytes_read);
    } else {
        first_chunk = rb->capacity - old_write_pos;
        second_chunk = bytes_read - first_chunk;
        newlines_count = zn_count_newlines(rb->buffer + old_write_pos,
            first_chunk);
        if (second_chunk > 0) {
            newlines_count += zn_count_newlines(rb->buffer,
                second_chunk);
        }
    }
    rb->line_count += newlines_count;
}

static ssize_t perform_read_operation(zn_ringbuf_t *rb, int fd,
    size_t available)
{
    if (rb->write_pos + available <= rb->capacity)
        return read_no_wrap(rb, fd, available);
    return read_with_wrap(rb, fd, available);
}

ssize_t zn_ringbuf_read_from_fd(zn_ringbuf_t *rb, int fd)
{
    size_t available;
    ssize_t bytes_read;
    size_t old_write_pos;

    if (validate_read_fd_params(rb, fd))
        return -1;
    available = zn_ringbuf_write_available(rb);
    if (available == 0) {
        errno = ENOBUFS;
        return -1;
    }
    old_write_pos = rb->write_pos;
    bytes_read = perform_read_operation(rb, fd, available);
    if (bytes_read <= 0)
        return bytes_read;
    update_after_read(rb, bytes_read, old_write_pos);
    if (check_line_limit_after_read(rb, bytes_read) < 0)
        return -1;
    return bytes_read;
}
