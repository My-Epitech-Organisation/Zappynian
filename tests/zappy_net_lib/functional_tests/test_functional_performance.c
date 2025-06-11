/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Performance functional tests for libzappy_net
*/

#include "functional_test_helpers.h"

void setup(void)
{
    setup_test_environment();
}

void teardown(void)
{
    teardown_test_environment();
}

/* Test connection establishment performance */
Test(performance, connection_latency, .init = setup, .fini = teardown,
    .timeout = 20)
{
    performance_stats_t stats;
    init_performance_stats(&stats);
    
    /* Measure connection establishment time */
    for (int i = 0; i < 100; i++) {
        struct timespec start, end;
        int server_fd, client_fd;
        
        clock_gettime(CLOCK_MONOTONIC, &start);
        
        server_fd = create_test_server(TEST_SERVER_PORT + 30 + i);
        if (server_fd >= 0) {
            usleep(10000); /* Small delay */
            client_fd = create_test_client("127.0.0.1",
                TEST_SERVER_PORT + 30 + i);
            
            if (client_fd >= 0) {
                clock_gettime(CLOCK_MONOTONIC, &end);
                double latency = get_time_diff_ms(start, end);
                update_performance_stats(&stats, latency);
                close(client_fd);
            } else {
                stats.error_count++;
            }
            close(server_fd);
        } else {
            stats.error_count++;
        }
    }
    
    finalize_performance_stats(&stats);
    print_performance_stats(&stats, "Connection Establishment");
    
    cr_assert(stats.success_count >= 95,
        "Too many connection failures: %d/100", stats.error_count);
    cr_assert(stats.avg_latency < 50.0,
        "Average connection latency too high: %.2f ms", stats.avg_latency);
}

/* Test message throughput performance */
Test(performance, message_throughput, .init = setup, .fini = teardown,
    .timeout = 25)
{
    int server_fd, client_fd, accepted_fd;
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);
    performance_stats_t stats;
    struct timespec start, end;
    
    init_performance_stats(&stats);
    
    server_fd = create_test_server(TEST_SERVER_PORT + 40);
    cr_assert(server_fd >= 0, "Failed to create test server");
    
    usleep(100000);
    client_fd = create_test_client("127.0.0.1", TEST_SERVER_PORT + 40);
    cr_assert(client_fd >= 0, "Failed to create test client");
    
    accepted_fd = accept(server_fd, (struct sockaddr *)&addr, &addr_len);
    cr_assert(accepted_fd >= 0, "Failed to accept client connection");
    
    /* Measure throughput with different message sizes */
    int message_sizes[] = {64, 128, 256, 512, 1024};
    int size_count = sizeof(message_sizes) / sizeof(message_sizes[0]);
    
    for (int s = 0; s < size_count; s++) {
        int msg_size = message_sizes[s];
        char *send_buffer = malloc(msg_size);
        char *recv_buffer = malloc(msg_size);
        
        cr_assert(send_buffer && recv_buffer,
            "Failed to allocate buffers for size %d", msg_size);
        
        /* Fill send buffer with pattern */
        for (int i = 0; i < msg_size - 1; i++) {
            send_buffer[i] = 'A' + (i % 26);
        }
        send_buffer[msg_size - 1] = '\0';
        
        clock_gettime(CLOCK_MONOTONIC, &start);
        
        /* Send 100 messages of this size */
        int success_count = 0;
        for (int i = 0; i < 100; i++) {
            ssize_t sent = write(client_fd, send_buffer, msg_size);
            ssize_t received = read(accepted_fd, recv_buffer, msg_size);
            
            if (sent == msg_size && received == msg_size) {
                success_count++;
            }
        }
        
        clock_gettime(CLOCK_MONOTONIC, &end);
        
        if (success_count > 0) {
            double total_time = get_time_diff_ms(start, end);
            double avg_per_message = total_time / success_count;
            update_performance_stats(&stats, avg_per_message);
            
            printf("Size %d: %d/%d messages, %.2f ms avg per message\n",
                msg_size, success_count, 100, avg_per_message);
        }
        
        free(send_buffer);
        free(recv_buffer);
    }
    
    finalize_performance_stats(&stats);
    print_performance_stats(&stats, "Message Throughput");
    
    cr_assert(stats.avg_latency < 5.0,
        "Average message latency too high: %.2f ms", stats.avg_latency);
    
    close(accepted_fd);
}

/* Test ring buffer performance */
Test(performance, ringbuf_operations, .init = setup, .fini = teardown,
    .timeout = 15)
{
    zn_ringbuf_t rb;
    performance_stats_t write_stats, read_stats;
    struct timespec start, end;
    
    init_performance_stats(&write_stats);
    init_performance_stats(&read_stats);
    
    cr_assert(zn_ringbuf_init(&rb, 8192) == 0,
        "Failed to initialize ring buffer");
    
    /* Test write performance */
    const char *test_line = "Performance test line with some data\n";
    size_t line_len = strlen(test_line);
    
    for (int i = 0; i < 1000; i++) {
        clock_gettime(CLOCK_MONOTONIC, &start);
        
        ssize_t written = zn_ringbuf_write(&rb, test_line, line_len);
        
        clock_gettime(CLOCK_MONOTONIC, &end);
        
        if (written == (ssize_t)line_len) {
            double latency = get_time_diff_ms(start, end);
            update_performance_stats(&write_stats, latency);
        } else {
            write_stats.error_count++;
        }
    }
    
    /* Test read performance */
    char read_buffer[128];
    for (int i = 0; i < 1000; i++) {
        clock_gettime(CLOCK_MONOTONIC, &start);
        
        ssize_t read_bytes = zn_ringbuf_read_line(&rb, read_buffer,
            sizeof(read_buffer));
        
        clock_gettime(CLOCK_MONOTONIC, &end);
        
        if (read_bytes > 0) {
            double latency = get_time_diff_ms(start, end);
            update_performance_stats(&read_stats, latency);
        } else {
            read_stats.error_count++;
            break; /* No more data */
        }
    }
    
    finalize_performance_stats(&write_stats);
    finalize_performance_stats(&read_stats);
    
    print_performance_stats(&write_stats, "Ring Buffer Write");
    print_performance_stats(&read_stats, "Ring Buffer Read");
    
    cr_assert(write_stats.avg_latency < 0.1,
        "Ring buffer write latency too high: %.4f ms",
        write_stats.avg_latency);
    cr_assert(read_stats.avg_latency < 0.1,
        "Ring buffer read latency too high: %.4f ms",
        read_stats.avg_latency);
    
    zn_ringbuf_cleanup(&rb);
}

/* Test memory usage efficiency */
Test(performance, memory_efficiency, .init = setup, .fini = teardown)
{
    /* Test ring buffer memory efficiency */
    zn_ringbuf_t small_rb, medium_rb, large_rb;
    
    /* Test different buffer sizes */
    cr_assert(zn_ringbuf_init(&small_rb, 256) == 0,
        "Failed to init small ring buffer");
    cr_assert(zn_ringbuf_init(&medium_rb, 1024) == 0,
        "Failed to init medium ring buffer");
    cr_assert(zn_ringbuf_init(&large_rb, 4096) == 0,
        "Failed to init large ring buffer");
    
    /* Test that we can fill buffers to capacity */
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

/* Test scalability with multiple connections */
Test(performance, scalability, .init = setup, .fini = teardown, .timeout = 30)
{
    int server_fd;
    int client_fds[20];
    int accepted_fds[20];
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);
    performance_stats_t stats;
    struct timespec start, end;
    
    init_performance_stats(&stats);
    
    server_fd = create_test_server(TEST_SERVER_PORT + 50);
    cr_assert(server_fd >= 0, "Failed to create test server");
    
    clock_gettime(CLOCK_MONOTONIC, &start);
    
    /* Create 20 simultaneous connections */
    for (int i = 0; i < 20; i++) {
        usleep(10000); /* Small delay between connections */
        
        client_fds[i] = create_test_client("127.0.0.1", TEST_SERVER_PORT + 50);
        if (client_fds[i] >= 0) {
            accepted_fds[i] = accept(server_fd, (struct sockaddr *)&addr,
                &addr_len);
            if (accepted_fds[i] >= 0) {
                stats.success_count++;
            } else {
                stats.error_count++;
                close(client_fds[i]);
            }
        } else {
            stats.error_count++;
        }
    }
    
    clock_gettime(CLOCK_MONOTONIC, &end);
    double total_time = get_time_diff_ms(start, end);
    
    printf("Scalability test: %d/%d connections in %.2f ms\n",
        stats.success_count, 20, total_time);
    
    /* Test message exchange on all connections */
    for (int i = 0; i < stats.success_count; i++) {
        test_message_t msg;
        generate_random_message(&msg, i);
        
        if (send_test_message(client_fds[i], &msg) == 0) {
            test_message_t recv_msg;
            if (receive_test_message(accepted_fds[i], &recv_msg) == 0) {
                cr_assert(recv_msg.id == msg.id,
                    "Message ID mismatch on connection %d", i);
            }
        }
        
        close(accepted_fds[i]);
    }
    
    cr_assert(stats.success_count >= 18,
        "Too few successful connections: %d/20", stats.success_count);
    cr_assert(total_time < 5000.0,
        "Connection setup took too long: %.2f ms", total_time);
}
