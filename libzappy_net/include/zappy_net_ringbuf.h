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

#endif /* !ZAPPY_NET_RINGBUF_H_ */
