/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Ring Buffer Tests with Criterion
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "zappy_net_ringbuf.h"
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>

#define BUFFER_SIZE 1024
#define MAX_MSG_SIZE 256
#define TEST_MESSAGES 100

void generate_random_message(char *buffer, size_t max_size)
{
    size_t length = 1 + (rand() % (max_size - 2));  /* At least 1 char + newline */
    size_t i;

    for (i = 0; i < length; i++) {
        /* Printable ASCII characters only (32-126) */
        buffer[i] = 32 + (rand() % 95);
    }
    buffer[length] = '\n';
    buffer[length + 1] = '\0';
}

/* Initialize and cleanup for each test */
static zn_ringbuf_t rb;
static int initialized = 0;

void setup(void)
{
    int result = zn_ringbuf_init(&rb, BUFFER_SIZE);
    initialized = (result == 0);
    cr_assert(result == 0, "Failed to initialize ring buffer");
}

void teardown(void)
{
    if (initialized) {
        zn_ringbuf_cleanup(&rb);
        initialized = 0;
    }
}

/* Test simple initialization and cleanup */
Test(ringbuf, init_cleanup, .init = setup, .fini = teardown)
{
    cr_assert(rb.buffer != NULL, "Buffer should be allocated");
    cr_assert(rb.capacity == BUFFER_SIZE, "Buffer should have the right capacity");
    cr_assert(rb.is_empty == 1, "Buffer should be empty after init");
}

/* Test writing and reading a single message */
Test(ringbuf, single_write_read, .init = setup, .fini = teardown)
{
    const char *test_msg = "Hello, world!\n";
    char buffer[128] = {0};
    ssize_t write_result, read_result;

    /* Write message to buffer */
    write_result = zn_ringbuf_write(&rb, test_msg, strlen(test_msg));
    cr_assert(write_result == (ssize_t)strlen(test_msg),
              "Write should return number of bytes written");
    cr_assert(!rb.is_empty, "Buffer should not be empty after write");

    /* Read message from buffer */
    read_result = zn_ringbuf_read(&rb, buffer, sizeof(buffer));
    cr_assert(read_result == (ssize_t)strlen(test_msg),
              "Read should return number of bytes read");
    cr_assert(rb.is_empty, "Buffer should be empty after read");

    /* Verify message content */
    cr_assert_str_eq(buffer, test_msg,
                    "Read data should match written data");
}

/* Test reading a line */
Test(ringbuf, read_line, .init = setup, .fini = teardown)
{
    const char *test_msg = "First line\nSecond line\n";
    char buffer[128] = {0};
    ssize_t write_result, read_result;

    /* Write message to buffer */
    write_result = zn_ringbuf_write(&rb, test_msg, strlen(test_msg));
    cr_assert(write_result == (ssize_t)strlen(test_msg));

    /* Read first line */
    read_result = zn_ringbuf_read_line(&rb, buffer, sizeof(buffer));
    cr_assert(read_result == 11, "Read line should return correct length");  // "First line\n" = 11 bytes
    cr_assert_str_eq(buffer, "First line\n");

    /* Read second line */
    memset(buffer, 0, sizeof(buffer));
    read_result = zn_ringbuf_read_line(&rb, buffer, sizeof(buffer));
    cr_assert(read_result == 12, "Read line should return correct length"); // "Second line\n" = 12 bytes
    cr_assert_str_eq(buffer, "Second line\n");

    /* Buffer should be empty now */
    cr_assert(rb.is_empty, "Buffer should be empty after reading all lines");
}

/* Test maximum line limit (10 lines) */
Test(ringbuf, max_lines_limit, .init = setup, .fini = teardown)
{
    const char *line = "This is a test line\n";
    ssize_t result;
    int i;

    /* Write 10 lines - should succeed */
    for (i = 0; i < 10; i++) {
        result = zn_ringbuf_write(&rb, line, strlen(line));
        cr_assert(result == (ssize_t)strlen(line),
                 "Should be able to write line %d", i);
    }

    /* Try to write one more line - should fail */
    result = zn_ringbuf_write(&rb, line, strlen(line));
    cr_assert(result == -1, "Writing 11th line should fail");
    cr_assert(errno == ENOBUFS, "Error should be ENOBUFS");
}

/* Test buffer wrapping (write/read across buffer boundary) */
Test(ringbuf, buffer_wrap, .init = setup, .fini = teardown)
{
    char test_data[BUFFER_SIZE / 2];  /* Half the buffer size */
    char result[BUFFER_SIZE];
    ssize_t write_result, read_result;
    int i;

    /* Fill test data buffer with pattern */
    for (i = 0; i < (BUFFER_SIZE / 2) - 1; i++) {
        test_data[i] = 'A' + (i % 26);
    }
    test_data[BUFFER_SIZE / 2 - 1] = '\0';

    /* First write to advance write pointer */
    write_result = zn_ringbuf_write(&rb, test_data, strlen(test_data));
    cr_assert(write_result == (ssize_t)strlen(test_data));

    /* Read to advance read pointer */
    read_result = zn_ringbuf_read(&rb, result, BUFFER_SIZE);
    cr_assert(read_result == (ssize_t)strlen(test_data));

    /* Now buffer pointers are advanced. Write again, which should wrap */
    write_result = zn_ringbuf_write(&rb, test_data, strlen(test_data));
    cr_assert(write_result == (ssize_t)strlen(test_data));

    /* Read the wrapped data */
    memset(result, 0, BUFFER_SIZE);
    read_result = zn_ringbuf_read(&rb, result, BUFFER_SIZE);
    cr_assert(read_result == (ssize_t)strlen(test_data));

    /* Verify read data matches written data */
    cr_assert_str_eq(result, test_data);
}

/* Test pipe I/O with ring buffer */
Test(ringbuf, pipe_io, .init = setup, .fini = teardown)
{
    int pipes[2];
    char test_msg[] = "This is a test message for pipe I/O\n";
    char buffer[128];
    ssize_t result;

    /* Create pipe */
    cr_assert(pipe(pipes) == 0, "Failed to create pipe");

    /* Write to ring buffer and flush to pipe */
    result = zn_ringbuf_write(&rb, test_msg, strlen(test_msg));
    cr_assert(result == (ssize_t)strlen(test_msg));

    result = zn_ringbuf_flush_to_fd(&rb, pipes[1]);
    cr_assert(result == (ssize_t)strlen(test_msg));

    /* Read from pipe into another ring buffer */
    zn_ringbuf_t read_rb;
    cr_assert(zn_ringbuf_init(&read_rb, BUFFER_SIZE) == 0);

    result = zn_ringbuf_read_from_fd(&read_rb, pipes[0]);
    cr_assert(result >= 0);

    /* Read from second ring buffer into memory */
    memset(buffer, 0, sizeof(buffer));
    result = zn_ringbuf_read(&read_rb, buffer, sizeof(buffer));
    cr_assert(result == (ssize_t)strlen(test_msg));

    /* Verify message content */
    cr_assert_str_eq(buffer, test_msg);

    /* Cleanup */
    zn_ringbuf_cleanup(&read_rb);
    close(pipes[0]);
    close(pipes[1]);
}

/* Test random message batch processing */
Test(ringbuf, random_messages, .init = setup, .fini = teardown)
{
    char send_buf[MAX_MSG_SIZE];
    char recv_buf[MAX_MSG_SIZE];
    ssize_t write_result, read_result;
    int i, success_count = 0;

    /* Set consistent seed for reproducible tests */
    srand(12345);

    for (i = 0; i < TEST_MESSAGES; i++) {
        /* Generate random message */
        generate_random_message(send_buf, MAX_MSG_SIZE);

        /* Write to buffer */
        write_result = zn_ringbuf_write(&rb, send_buf, strlen(send_buf));
        cr_assert(write_result == (ssize_t)strlen(send_buf),
                 "Write failed for message %d", i);

        /* Read from buffer */
        memset(recv_buf, 0, sizeof(recv_buf));
        read_result = zn_ringbuf_read(&rb, recv_buf, sizeof(recv_buf));
        cr_assert(read_result == (ssize_t)strlen(send_buf),
                 "Read failed for message %d", i);

        /* Verify message */
        cr_assert_str_eq(recv_buf, send_buf,
                        "Message %d content mismatch", i);
        success_count++;
    }

    cr_assert(success_count == TEST_MESSAGES,
             "Not all messages were successfully processed");
}
