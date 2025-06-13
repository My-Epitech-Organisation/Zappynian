/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Simple functional tests for libzappy_net ring buffer
*/

#define _POSIX_C_SOURCE 200809L

#include <criterion/criterion.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

/* Include the ring buffer header with full path resolution */
#include "../../../libzappy_net/include/zappy_net_ringbuf.h"

/* Test ring buffer integration with pipes */
Test(functional, ringbuf_pipe_integration)
{
    zn_ringbuf_t rb;
    int pipe_fds[2];
    const char *test_data = "Line 1\nLine 2\nLine 3\n";
    char read_buffer[128];
    ssize_t written, read_bytes;

    /* Initialize ring buffer */
    cr_assert(zn_ringbuf_init(&rb, 256) == 0,
        "Failed to initialize ring buffer");

    /* Create pipe for testing */
    cr_assert(pipe(pipe_fds) == 0, "Failed to create pipe");

    /* Write test data to ring buffer */
    written = zn_ringbuf_write(&rb, test_data, strlen(test_data));
    cr_assert(written == (ssize_t)strlen(test_data),
        "Failed to write all data to ring buffer");

    /* Flush ring buffer to pipe */
    ssize_t flushed = zn_ringbuf_flush_to_fd(&rb, pipe_fds[1]);
    cr_assert(flushed == (ssize_t)strlen(test_data),
        "Failed to flush all data from ring buffer");

    /* Read data from pipe */
    read_bytes = read(pipe_fds[0], read_buffer, sizeof(read_buffer) - 1);
    cr_assert(read_bytes == (ssize_t)strlen(test_data),
        "Failed to read all data from pipe");

    read_buffer[read_bytes] = '\0';
    cr_assert(strcmp(read_buffer, test_data) == 0,
        "Data corruption during ring buffer I/O");

    /* Clean up */
    close(pipe_fds[0]);
    close(pipe_fds[1]);
    zn_ringbuf_cleanup(&rb);
}

/* Test rapid read/write operations */
Test(functional, ringbuf_rapid_operations)
{
    zn_ringbuf_t rb;
    struct timespec start, end;
    int success_count = 0;

    cr_assert(zn_ringbuf_init(&rb, 4096) == 0,
        "Failed to initialize ring buffer");

    clock_gettime(CLOCK_MONOTONIC, &start);

    /* Rapid write/read cycles */
    for (int i = 0; i < 1000; i++) {
        char test_line[64];
        snprintf(test_line, sizeof(test_line), "Test line %d\n", i);

        ssize_t written = zn_ringbuf_write(&rb, test_line,
            strlen(test_line));

        if (written > 0) {
            char read_buffer[128];
            ssize_t read_bytes = zn_ringbuf_read_line(&rb, read_buffer,
                sizeof(read_buffer));

            if (read_bytes > 0 && strcmp(read_buffer, test_line) == 0) {
                success_count++;
            }
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &end);
    double total_time = (end.tv_sec - start.tv_sec) * 1000.0 +
                       (end.tv_nsec - start.tv_nsec) / 1000000.0;

    printf("Ring buffer operations: %d/1000 successful in %.2f ms\n",
        success_count, total_time);

    cr_assert(success_count >= 950,
        "Too many ring buffer operation failures: %d", 1000 - success_count);
    cr_assert(total_time < 100.0,
        "Ring buffer operations too slow: %.2f ms", total_time);

    zn_ringbuf_cleanup(&rb);
}

/* Test buffer capacity and wrapping */
Test(functional, ringbuf_capacity_test)
{
    zn_ringbuf_t rb;
    char fill_data[100];

    cr_assert(zn_ringbuf_init(&rb, 256) == 0,
        "Failed to initialize ring buffer");

    /* Create fill pattern */
    for (int i = 0; i < 99; i++) {
        fill_data[i] = 'A' + (i % 26);
    }
    fill_data[99] = '\0';

    /* Fill buffer multiple times to test wrapping */
    int total_written = 0;
    for (int round = 0; round < 5; round++) {
        ssize_t written = zn_ringbuf_write(&rb, fill_data, strlen(fill_data));
        if (written > 0) {
            total_written += written;
        }

        /* Read some data to make space */
        if (round % 2 == 1) {
            char read_buffer[128];
            zn_ringbuf_read(&rb, read_buffer, 50);
        }
    }

    cr_assert(total_written >= 200,
        "Buffer utilization too low: %d bytes written", total_written);

    zn_ringbuf_cleanup(&rb);
}

/* Test line-based parsing */
Test(functional, line_parsing_test)
{
    zn_ringbuf_t rb;
    const char *test_input = "Command1\nCommand2\nIncomplete";
    const char *more_input = " Command\nCommand3\n";
    char line_buffer[128];
    ssize_t result;

    cr_assert(zn_ringbuf_init(&rb, 256) == 0,
        "Failed to initialize ring buffer");

    /* Write partial input */
    ssize_t written = zn_ringbuf_write(&rb, test_input, strlen(test_input));
    cr_assert(written == (ssize_t)strlen(test_input),
        "Failed to write test input to ring buffer");

    /* Read first complete line */
    result = zn_ringbuf_read_line(&rb, line_buffer, sizeof(line_buffer));
    cr_assert(result == 9, "Failed to read first line");
    cr_assert(strcmp(line_buffer, "Command1\n") == 0,
        "First line mismatch: '%s'", line_buffer);

    /* Read second complete line */
    result = zn_ringbuf_read_line(&rb, line_buffer, sizeof(line_buffer));
    cr_assert(result == 9, "Failed to read second line");
    cr_assert(strcmp(line_buffer, "Command2\n") == 0,
        "Second line mismatch: '%s'", line_buffer);

    /* Try to read incomplete line (should fail) */
    result = zn_ringbuf_read_line(&rb, line_buffer, sizeof(line_buffer));
    cr_assert(result == -1, "Should fail to read incomplete line");

    /* Add more input to complete the line */
    written = zn_ringbuf_write(&rb, more_input, strlen(more_input));
    cr_assert(written == (ssize_t)strlen(more_input),
        "Failed to write additional input");

    /* Now read the completed line */
    result = zn_ringbuf_read_line(&rb, line_buffer, sizeof(line_buffer));
    cr_assert(result == 19, "Failed to read completed line");
    cr_assert(strcmp(line_buffer, "Incomplete Command\n") == 0,
        "Completed line mismatch: '%s'", line_buffer);

    /* Read final line */
    result = zn_ringbuf_read_line(&rb, line_buffer, sizeof(line_buffer));
    cr_assert(result == 9, "Failed to read final line");
    cr_assert(strcmp(line_buffer, "Command3\n") == 0,
        "Final line mismatch: '%s'", line_buffer);

    zn_ringbuf_cleanup(&rb);
}

/* Test memory efficiency */
Test(functional, memory_efficiency_test)
{
    zn_ringbuf_t small_rb, medium_rb, large_rb;

    /* Test different buffer sizes */
    cr_assert(zn_ringbuf_init(&small_rb, 256) == 0,
        "Failed to init small ring buffer");
    cr_assert(zn_ringbuf_init(&medium_rb, 1024) == 0,
        "Failed to init medium ring buffer");
    cr_assert(zn_ringbuf_init(&large_rb, 4096) == 0,
        "Failed to init large ring buffer");

    /* Test that we can fill buffers efficiently */
    char test_data[256];
    memset(test_data, 'X', 255);
    test_data[255] = '\0';

    /* Fill small buffer */
    ssize_t written = 0;
    while (written < 256) {
        ssize_t chunk = zn_ringbuf_write(&small_rb, "X", 1);
        if (chunk <= 0) break;
        written += chunk;
    }
    cr_assert(written >= 250, "Small buffer utilization too low: %zd/256",
        written);

    /* Fill medium buffer */
    written = 0;
    while (written < 1024) {
        ssize_t chunk = zn_ringbuf_write(&medium_rb, test_data,
            strlen(test_data));
        if (chunk <= 0) break;
        written += chunk;
    }
    cr_assert(written >= 1000, "Medium buffer utilization too low: %zd/1024",
        written);

    /* Fill large buffer */
    written = 0;
    while (written < 4096) {
        ssize_t chunk = zn_ringbuf_write(&large_rb, test_data,
            strlen(test_data));
        if (chunk <= 0) break;
        written += chunk;
    }
    cr_assert(written >= 4000, "Large buffer utilization too low: %zd/4096",
        written);

    zn_ringbuf_cleanup(&small_rb);
    zn_ringbuf_cleanup(&medium_rb);
    zn_ringbuf_cleanup(&large_rb);
}

/* Test error handling */
Test(functional, error_handling_test)
{
    zn_ringbuf_t rb;
    char buffer[128];

    /* Initialize properly first */
    cr_assert(zn_ringbuf_init(&rb, 64) == 0,
        "Failed to initialize ring buffer");

    /* Test NULL pointer handling - expect failure */
    ssize_t result = zn_ringbuf_write(&rb, NULL, 10);
    cr_assert(result <= 0, "Should handle NULL data pointer gracefully");

    /* Test zero length operations */
    result = zn_ringbuf_write(&rb, "test", 0);
    cr_assert(result <= 0, "Should handle zero length write");

    result = zn_ringbuf_read(&rb, buffer, 0);
    cr_assert(result <= 0, "Should handle zero length read");

    zn_ringbuf_cleanup(&rb);

    /* Test double cleanup (should be safe) */
    zn_ringbuf_cleanup(&rb);
}
