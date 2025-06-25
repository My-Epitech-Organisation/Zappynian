/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Zappy Network Library - Ring Buffer Header (Public)
*/

#ifndef ZAPPY_NET_RINGBUF_H_
    #define ZAPPY_NET_RINGBUF_H_

    #include <stddef.h>
    #include <stdint.h>
    #include <sys/types.h>
    #include <stdlib.h>
    #include <string.h>
    #include <unistd.h>
    #include <errno.h>

    #define MAX_QUEUED_LINES 200
    #define DEFAULT_BUFFER_SIZE 65536 // 64 KiB

/**
 * @brief Ring buffer structure for socket I/O operations
 *
 * This structure implements a circular buffer for efficient read/write
 * operations with network sockets. It includes separate read and write
 * pointers to allow concurrent operations.
 */
typedef struct zn_ringbuf {
    uint8_t *buffer;       /**< Buffer memory area */
    size_t capacity;       /**< Total buffer capacity in bytes */
    size_t read_pos;       /**< Current read position */
    size_t write_pos;      /**< Current write position */
    size_t line_count;     /**< Number of complete lines in buffer */
    int is_empty;          /**< Flag indicating if buffer is empty */
} zn_ringbuf_t;

typedef struct read_wrap_params {
    zn_ringbuf_t *rb;
    int fd;
    size_t available;
    ssize_t bytes_read;
    size_t first_chunk;
} read_wrap_params_t;

/**
 * @brief Initialize a ring buffer with given capacity
 *
 * @param rb Pointer to ring buffer structure
 * @param capacity Buffer capacity in bytes (default 64 KiB if 0)
 * @return 0 on success, -1 on failure
 */
int zn_ringbuf_init(zn_ringbuf_t *rb, size_t capacity);

/**
 * @brief Clean up a ring buffer and free its resources
 *
 * @param rb Pointer to ring buffer structure
 */
void zn_ringbuf_cleanup(zn_ringbuf_t *rb);

/**
 * @brief Write data to the ring buffer
 *
 * @param rb Pointer to ring buffer structure
 * @param data Pointer to data to write
 * @param len Length of data in bytes
 * @return Number of bytes written, or -1 on failure
 */
ssize_t zn_ringbuf_write(zn_ringbuf_t *rb, const void *data, size_t len);

/**
 * @brief Read data from the ring buffer
 *
 * @param rb Pointer to ring buffer structure
 * @param data Buffer to store read data
 * @param len Maximum amount of data to read
 * @return Number of bytes read, or -1 on failure
 */
ssize_t zn_ringbuf_read(zn_ringbuf_t *rb, void *data, size_t len);

/**
 * @brief Flush ring buffer to a file descriptor
 *
 * @param rb Pointer to ring buffer structure
 * @param fd File descriptor to write to
 * @return Number of bytes written, or -1 on failure
 */
ssize_t zn_ringbuf_flush_to_fd(zn_ringbuf_t *rb, int fd);

/**
 * @brief Read from file descriptor into ring buffer
 *
 * @param rb Pointer to ring buffer structure
 * @param fd File descriptor to read from
 * @return Number of bytes read, or -1 on failure
 */
ssize_t zn_ringbuf_read_from_fd(zn_ringbuf_t *rb, int fd);

/**
 * @brief Count number of complete lines in buffer
 *
 * @param rb Pointer to ring buffer structure
 * @return Number of complete lines
 */
size_t zn_ringbuf_count_lines(const zn_ringbuf_t *rb);

/**
 * @brief Check if the buffer is empty
 *
 * @param rb Pointer to ring buffer structure
 * @return 1 if empty, 0 if not empty
 */
int zn_ringbuf_is_empty(const zn_ringbuf_t *rb);

/**
 * @brief Read a complete line from the buffer
 *
 * @param rb Pointer to ring buffer structure
 * @param data Buffer to store the line
 * @param max_len Maximum length to read
 * @return Number of bytes read (including newline), or -1 on failure
 */
ssize_t zn_ringbuf_read_line(zn_ringbuf_t *rb, void *data, size_t max_len);

/**
 * @brief Get available write space in the buffer
 *
 * @param rb Pointer to ring buffer structure
 * @return Number of bytes that can be written
 */
size_t zn_ringbuf_write_available(const zn_ringbuf_t *rb);

/**
 * @brief Get available read data in the buffer
 *
 * @param rb Pointer to ring buffer structure
 * @return Number of bytes that can be read
 */
size_t zn_ringbuf_read_available(const zn_ringbuf_t *rb);

/**
 * @brief Check if the buffer is full
 *
 * @param rb Pointer to ring buffer structure
 * @return 1 if full, 0 if not full
 */
int zn_ringbuf_is_full(const zn_ringbuf_t *rb);

/**
 * @brief Count newlines in a data segment
 *
 * @param data Pointer to data buffer
 * @param len Length of data
 * @return Number of newline characters found
 */
int zn_count_newlines(const uint8_t *data, size_t len);

/* Helper functions for ring buffer read operations */
ssize_t handle_wrap_read_first(zn_ringbuf_t *rb, int fd, size_t first_chunk);
ssize_t handle_wrap_read_second(zn_ringbuf_t *rb, int fd, size_t second_chunk,
    ssize_t first_bytes_read);
void update_write_pos_after_wrap(zn_ringbuf_t *rb, ssize_t bytes_read,
    size_t first_chunk, ssize_t result);
int validate_read_fd_params(zn_ringbuf_t *rb, int fd);
int check_line_limit_after_read(zn_ringbuf_t *rb, ssize_t bytes_read);

#endif /* !ZAPPY_NET_RINGBUF_H_ */
