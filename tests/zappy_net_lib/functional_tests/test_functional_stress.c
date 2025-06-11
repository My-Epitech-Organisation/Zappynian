/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Stress functional tests for libzappy_net
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

/* Test high-frequency message exchange */
Test(stress, high_frequency_messages, .init = setup, .fini = teardown,
    .timeout = 30)
{
    int server_fd, client_fd, accepted_fd;
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);
    performance_stats_t stats;
    struct timespec start, end;
    
    init_performance_stats(&stats);
    
    server_fd = create_test_server(TEST_SERVER_PORT + 10);
    cr_assert(server_fd >= 0, "Failed to create test server");
    
    usleep(100000);
    client_fd = create_test_client("127.0.0.1", TEST_SERVER_PORT + 10);
    cr_assert(client_fd >= 0, "Failed to create test client");
    
    accepted_fd = accept(server_fd, (struct sockaddr *)&addr, &addr_len);
    cr_assert(accepted_fd >= 0, "Failed to accept client connection");
    
    /* Send 1000 messages rapidly */
    for (int i = 0; i < 1000; i++) {
        test_message_t send_msg, recv_msg;
        
        clock_gettime(CLOCK_MONOTONIC, &start);
        
        generate_random_message(&send_msg, i);
        
        if (send_test_message(client_fd, &send_msg) == 0 &&
            receive_test_message(accepted_fd, &recv_msg) == 0) {
            
            clock_gettime(CLOCK_MONOTONIC, &end);
            double latency = get_time_diff_ms(start, end);
            update_performance_stats(&stats, latency);
            
            cr_assert(recv_msg.id == send_msg.id,
                "Message ID mismatch in iteration %d", i);
        } else {
            stats.error_count++;
        }
    }
    
    finalize_performance_stats(&stats);
    print_performance_stats(&stats, "High Frequency Messages");
    
    cr_assert(stats.success_count >= 950,
        "Too many failed messages: %d errors out of 1000",
        stats.error_count);
    cr_assert(stats.avg_latency < 10.0,
        "Average latency too high: %.2f ms", stats.avg_latency);
    
    close(accepted_fd);
}

/* Test large message handling */
Test(stress, large_messages, .init = setup, .fini = teardown, .timeout = 20)
{
    int server_fd, client_fd, accepted_fd;
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);
    
    server_fd = create_test_server(TEST_SERVER_PORT + 11);
    cr_assert(server_fd >= 0, "Failed to create test server");
    
    usleep(100000);
    client_fd = create_test_client("127.0.0.1", TEST_SERVER_PORT + 11);
    cr_assert(client_fd >= 0, "Failed to create test client");
    
    accepted_fd = accept(server_fd, (struct sockaddr *)&addr, &addr_len);
    cr_assert(accepted_fd >= 0, "Failed to accept client connection");
    
    /* Test increasingly large messages */
    for (int size = 1024; size <= 8192; size *= 2) {
        char *large_buffer = malloc(size);
        cr_assert(large_buffer != NULL, "Failed to allocate large buffer");
        
        /* Fill with pattern */
        for (int i = 0; i < size - 1; i++) {
            large_buffer[i] = 'A' + (i % 26);
        }
        large_buffer[size - 1] = '\0';
        
        /* Send large message */
        ssize_t sent = write(client_fd, large_buffer, size);
        cr_assert(sent == size, "Failed to send large message of size %d",
            size);
        
        /* Receive large message */
        char *recv_buffer = malloc(size);
        cr_assert(recv_buffer != NULL,
            "Failed to allocate receive buffer");
        
        ssize_t received = 0;
        while (received < size) {
            ssize_t chunk = read(accepted_fd, recv_buffer + received,
                size - received);
            cr_assert(chunk > 0, "Failed to receive large message chunk");
            received += chunk;
        }
        
        cr_assert(received == size,
            "Incomplete large message received: %zd/%d", received, size);
        cr_assert(memcmp(large_buffer, recv_buffer, size) == 0,
            "Large message data corruption");
        
        free(large_buffer);
        free(recv_buffer);
    }
    
    close(accepted_fd);
}

/* Test ring buffer stress with rapid writes and reads */
Test(stress, ringbuf_rapid_operations, .init = setup, .fini = teardown,
    .timeout = 15)
{
    zn_ringbuf_t rb;
    performance_stats_t stats;
    struct timespec start, end;
    
    init_performance_stats(&stats);
    
    cr_assert(zn_ringbuf_init(&rb, 4096) == 0,
        "Failed to initialize ring buffer");
    
    /* Rapid write/read cycles */
    for (int i = 0; i < 5000; i++) {
        clock_gettime(CLOCK_MONOTONIC, &start);
        
        char test_line[64];
        snprintf(test_line, sizeof(test_line), "Test line %d\n", i);
        
        ssize_t written = zn_ringbuf_write(&rb, test_line,
            strlen(test_line));
        
        if (written > 0) {
            char read_buffer[128];
            ssize_t read_bytes = zn_ringbuf_read_line(&rb, read_buffer,
                sizeof(read_buffer));
            
            if (read_bytes > 0) {
                clock_gettime(CLOCK_MONOTONIC, &end);
                double latency = get_time_diff_ms(start, end);
                update_performance_stats(&stats, latency);
                
                cr_assert(strcmp(read_buffer, test_line) == 0,
                    "Ring buffer data corruption in iteration %d", i);
            } else {
                stats.error_count++;
            }
        } else {
            stats.error_count++;
        }
    }
    
    finalize_performance_stats(&stats);
    print_performance_stats(&stats, "Ring Buffer Rapid Operations");
    
    cr_assert(stats.success_count >= 4950,
        "Too many ring buffer operation failures: %d", stats.error_count);
    
    zn_ringbuf_cleanup(&rb);
}

/* Test memory allocation stress */
Test(stress, memory_allocation, .init = setup, .fini = teardown,
    .timeout = 20)
{
    zn_socket_t *sockets[100];
    zn_ringbuf_t buffers[50];
    int successful_sockets = 0;
    int successful_buffers = 0;
    
    /* Allocate many sockets */
    for (int i = 0; i < 100; i++) {
        sockets[i] = zn_socket_create();
        if (sockets[i] != NULL) {
            successful_sockets++;
        }
    }
    
    cr_assert(successful_sockets >= 90,
        "Failed to create enough sockets: %d/100", successful_sockets);
    
    /* Allocate many ring buffers */
    for (int i = 0; i < 50; i++) {
        if (zn_ringbuf_init(&buffers[i], 1024) == 0) {
            successful_buffers++;
        }
    }
    
    cr_assert(successful_buffers >= 45,
        "Failed to create enough ring buffers: %d/50", successful_buffers);
    
    /* Clean up sockets */
    for (int i = 0; i < 100; i++) {
        if (sockets[i] != NULL) {
            zn_socket_destroy(sockets[i]);
        }
    }
    
    /* Clean up buffers */
    for (int i = 0; i < successful_buffers; i++) {
        zn_ringbuf_cleanup(&buffers[i]);
    }
}

/* Test concurrent access simulation */
Test(stress, concurrent_simulation, .init = setup, .fini = teardown,
    .timeout = 25)
{
    int server_fd;
    int client_count = 10;
    pid_t child_pids[10];
    int success_count = 0;
    
    server_fd = create_test_server(TEST_SERVER_PORT + 12);
    cr_assert(server_fd >= 0, "Failed to create test server");
    
    /* Fork multiple client processes */
    for (int i = 0; i < client_count; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            /* Child process - act as client */
            usleep(100000 + i * 10000);
            
            int client_fd = create_test_client("127.0.0.1",
                TEST_SERVER_PORT + 12);
            if (client_fd >= 0) {
                test_message_t msg;
                generate_random_message(&msg, i);
                
                if (send_test_message(client_fd, &msg) == 0) {
                    exit(0); /* Success */
                }
            }
            exit(1); /* Failure */
        } else if (pid > 0) {
            child_pids[i] = pid;
        }
    }
    
    /* Accept connections and handle them */
    for (int i = 0; i < client_count; i++) {
        struct sockaddr_in addr;
        socklen_t addr_len = sizeof(addr);
        
        int accepted_fd = accept(server_fd, (struct sockaddr *)&addr,
            &addr_len);
        if (accepted_fd >= 0) {
            test_message_t recv_msg;
            if (receive_test_message(accepted_fd, &recv_msg) == 0) {
                success_count++;
            }
            close(accepted_fd);
        }
    }
    
    /* Wait for all child processes */
    for (int i = 0; i < client_count; i++) {
        int status;
        waitpid(child_pids[i], &status, 0);
    }
    
    cr_assert(success_count >= client_count - 2,
        "Too many concurrent client failures: %d/%d succeeded",
        success_count, client_count);
}
