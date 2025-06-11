/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Zappy Network Library - Ring Buffer Implementation
*/

#include "../include/zappy_net_ringbuf.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <errno.h>

#define DEFAULT_BUFFER_SIZE (64 * 1024) /* 64 KiB */
#define MAX_QUEUED_LINES 10

int zn_ringbuf_init(zn_ringbuf_t *rb, size_t capacity)
{
    if (rb == NULL) {
        return -1;
    }

    capacity = (capacity > 0) ? capacity : DEFAULT_BUFFER_SIZE;

    rb->buffer = malloc(capacity);
    if (rb->buffer == NULL) {
        return -1;
    }

    rb->capacity = capacity;
    rb->read_pos = 0;
    rb->write_pos = 0;
    rb->is_empty = 1;
    rb->line_count = 0;

    return 0;
}

void zn_ringbuf_cleanup(zn_ringbuf_t *rb)
{
    if (rb == NULL) {
        return;
    }

    free(rb->buffer);
    rb->buffer = NULL;
    rb->capacity = 0;
    rb->read_pos = 0;
    rb->write_pos = 0;
    rb->is_empty = 1;
    rb->line_count = 0;
}

size_t zn_ringbuf_write_available(const zn_ringbuf_t *rb)
{
    if (rb == NULL || rb->buffer == NULL) {
        return 0;
    }

    if (rb->is_empty) {
        return rb->capacity - 1; /* Keep one byte to distinguish full from empty */
    }

    if (rb->write_pos >= rb->read_pos) {
        return rb->capacity - (rb->write_pos - rb->read_pos) - 1;
    } else {
        return rb->read_pos - rb->write_pos - 1;
    }
}

size_t zn_ringbuf_read_available(const zn_ringbuf_t *rb)
{
    if (rb == NULL || rb->buffer == NULL || rb->is_empty) {
        return 0;
    }

    if (rb->write_pos > rb->read_pos) {
        return rb->write_pos - rb->read_pos;
    } else {
        return rb->capacity - rb->read_pos + rb->write_pos;
    }
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
    if (rb == NULL) {
        return 0;
    }
    return rb->line_count;
}

/* Count newlines in a buffer segment */
static int count_newlines(const uint8_t *data, size_t len)
{
    int count = 0;
    size_t i;

    for (i = 0; i < len; i++) {
        if (data[i] == '\n') {
            count++;
        }
    }

    return count;
}

ssize_t zn_ringbuf_write(zn_ringbuf_t *rb, const void *data, size_t len)
{
    size_t available;
    size_t first_chunk;
    size_t second_chunk;
    const uint8_t *src = (const uint8_t *)data;
    int newline_count;

    if (rb == NULL || rb->buffer == NULL || data == NULL || len == 0) {
        return -1;
    }

    /* Check for max queued lines limit */
    newline_count = count_newlines(src, len);
    if (rb->line_count + newline_count > MAX_QUEUED_LINES) {
        errno = ENOBUFS;  /* Buffer space not available */
        return -1;
    }

    available = zn_ringbuf_write_available(rb);
    if (len > available) {
        errno = ENOBUFS;
        return -1;
    }

    /* Write data to buffer, handling wrap-around */
    if (rb->write_pos + len <= rb->capacity) {
        /* Simple case: no wrap-around needed */
        memcpy(rb->buffer + rb->write_pos, src, len);
        rb->write_pos += len;
        if (rb->write_pos == rb->capacity) {
            rb->write_pos = 0;
        }
    } else {
        /* Write wraps around the end of the buffer */
        first_chunk = rb->capacity - rb->write_pos;
        second_chunk = len - first_chunk;

        memcpy(rb->buffer + rb->write_pos, src, first_chunk);
        memcpy(rb->buffer, src + first_chunk, second_chunk);

        rb->write_pos = second_chunk;
    }

    rb->is_empty = 0;
    rb->line_count += newline_count;

    return len;
}

ssize_t zn_ringbuf_read(zn_ringbuf_t *rb, void *data, size_t len)
{
    size_t available;
    size_t first_chunk;
    size_t second_chunk;
    uint8_t *dst = (uint8_t *)data;
    int newline_count;

    if (rb == NULL || rb->buffer == NULL || data == NULL || len == 0 || rb->is_empty) {
        return 0;
    }

    available = zn_ringbuf_read_available(rb);
    len = (len < available) ? len : available;

    /* Read data from buffer, handling wrap-around */
    if (rb->read_pos + len <= rb->capacity) {
        /* Simple case: no wrap-around needed */
        newline_count = count_newlines(rb->buffer + rb->read_pos, len);
        memcpy(dst, rb->buffer + rb->read_pos, len);
        rb->read_pos += len;
        if (rb->read_pos == rb->capacity) {
            rb->read_pos = 0;
        }
    } else {
        /* Read wraps around the end of the buffer */
        first_chunk = rb->capacity - rb->read_pos;
        second_chunk = len - first_chunk;

        newline_count = count_newlines(rb->buffer + rb->read_pos, first_chunk);
        newline_count += count_newlines(rb->buffer, second_chunk);

        memcpy(dst, rb->buffer + rb->read_pos, first_chunk);
        memcpy(dst + first_chunk, rb->buffer, second_chunk);

        rb->read_pos = second_chunk;
    }

    rb->line_count -= newline_count;

    /* Check if buffer is now empty */
    if (rb->read_pos == rb->write_pos) {
        rb->is_empty = 1;
    }

    return len;
}

ssize_t zn_ringbuf_read_line(zn_ringbuf_t *rb, void *data, size_t max_len)
{
    size_t available;
    size_t i, pos, bytes_read = 0;
    uint8_t *dst = (uint8_t *)data;
    uint8_t c;
    int found_newline = 0;

    if (rb == NULL || rb->buffer == NULL || data == NULL || max_len == 0 || rb->is_empty) {
        return -1;
    }

    available = zn_ringbuf_read_available(rb);
    max_len = (max_len < available) ? max_len : available;

    /* Look for a newline character */
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

    /* Check if buffer is now empty */
    if (rb->read_pos == rb->write_pos) {
        rb->is_empty = 1;
    }

    return found_newline ? bytes_read : -1;
}

ssize_t zn_ringbuf_flush_to_fd(zn_ringbuf_t *rb, int fd)
{
    size_t available;
    ssize_t written = 0;
    ssize_t result;
    size_t first_chunk, second_chunk;

    if (rb == NULL || rb->buffer == NULL || fd < 0 || rb->is_empty) {
        return -1;
    }

    available = zn_ringbuf_read_available(rb);

    /* Write data to fd, handling wrap-around */
    if (rb->read_pos + available <= rb->capacity) {
        /* Simple case: no wrap-around needed */
        result = write(fd, rb->buffer + rb->read_pos, available);
        if (result < 0) {
            return -1;
        }
        written = result;
        rb->read_pos += result;
        if (rb->read_pos == rb->capacity) {
            rb->read_pos = 0;
        }
    } else {
        /* Write wraps around the end of the buffer */
        first_chunk = rb->capacity - rb->read_pos;

        result = write(fd, rb->buffer + rb->read_pos, first_chunk);
        if (result < 0) {
            return -1;
        }
        written = result;

        /* If we wrote the entire first chunk, try to write the second */
        if (written == (ssize_t)first_chunk) {
            second_chunk = available - first_chunk;
            result = write(fd, rb->buffer, second_chunk);
            if (result < 0) {
                /* Update read_pos to reflect what was successfully written */
                rb->read_pos = (rb->read_pos + written) % rb->capacity;
                return written;
            }
            written += result;
            rb->read_pos = result;
        } else {
            /* Only a portion of the first chunk was written */
            rb->read_pos += written;
        }
    }

    /* Count newlines in what was written */
    int newlines = count_newlines(rb->buffer + rb->read_pos - written, written);
    rb->line_count -= newlines;

    /* Check if buffer is now empty */
    if (rb->read_pos == rb->write_pos) {
        rb->is_empty = 1;
    }

    return written;
}

ssize_t zn_ringbuf_read_from_fd(zn_ringbuf_t *rb, int fd)
{
    size_t available;
    ssize_t bytes_read = 0;
    ssize_t result;
    size_t first_chunk, second_chunk;

    if (rb == NULL || rb->buffer == NULL || fd < 0) {
        return -1;
    }

    available = zn_ringbuf_write_available(rb);
    if (available == 0) {
        errno = ENOBUFS;
        return -1;
    }

    /* Read data from fd, handling wrap-around */
    if (rb->write_pos + available <= rb->capacity) {
        /* Simple case: no wrap-around needed */
        result = read(fd, rb->buffer + rb->write_pos, available);
        if (result <= 0) {
            return result;  /* EOF or error */
        }
        bytes_read = result;
        rb->write_pos += result;
        if (rb->write_pos == rb->capacity) {
            rb->write_pos = 0;
        }
    } else {
        /* Read wraps around the end of the buffer */
        first_chunk = rb->capacity - rb->write_pos;

        result = read(fd, rb->buffer + rb->write_pos, first_chunk);
        if (result <= 0) {
            return result;  /* EOF or error */
        }
        bytes_read = result;

        /* If we read the entire first chunk, try to read the second */
        if (bytes_read == (ssize_t)first_chunk) {
            second_chunk = available - first_chunk;
            result = read(fd, rb->buffer, second_chunk);
            if (result <= 0) {
                /* EOF or error, but we've already read something */
                rb->write_pos = (rb->write_pos + bytes_read) % rb->capacity;
                rb->is_empty = 0;
                rb->line_count += count_newlines(rb->buffer + rb->write_pos - bytes_read, bytes_read);
                return bytes_read;
            }
            bytes_read += result;
            rb->write_pos = result;
        } else {
            /* Only a portion of the first chunk was read */
            rb->write_pos += bytes_read;
        }
    }

    rb->is_empty = 0;

    /* Count newlines in what was read */
    rb->line_count += count_newlines(rb->buffer + rb->write_pos - bytes_read, bytes_read);

    /* Check if we're exceeding the max queued lines limit */
    if (rb->line_count > MAX_QUEUED_LINES) {
        /* We need to rollback the read if we'd exceed the limit */
        /* For simplicity, we'll just reset to previous state instead of partial rollback */
        rb->write_pos = (rb->write_pos - bytes_read + rb->capacity) % rb->capacity;
        rb->line_count -= count_newlines(rb->buffer + rb->write_pos, bytes_read);
        if (rb->read_pos == rb->write_pos) {
            rb->is_empty = 1;
        }
        errno = ENOBUFS;
        return -1;
    }

    return bytes_read;
}
