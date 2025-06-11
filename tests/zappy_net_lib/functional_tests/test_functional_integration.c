/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Integration functional tests for libzappy_net
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

/* Test basic server-client integration */
Test(integration, basic_server_client, .init = setup, .fini = teardown)
{
    int server_fd, client_fd, accepted_fd;
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);
    test_message_t send_msg, recv_msg;

    /* Create and start server */
    server_fd = create_test_server(TEST_SERVER_PORT);
    cr_assert(server_fd >= 0, "Failed to create test server");

    /* Create client and connect */
    usleep(100000); /* 100ms delay */
    client_fd = create_test_client("127.0.0.1", TEST_SERVER_PORT);
    cr_assert(client_fd >= 0, "Failed to create test client");

    /* Accept connection on server */
    accepted_fd = accept(server_fd, (struct sockaddr *)&addr, &addr_len);
    cr_assert(accepted_fd >= 0, "Failed to accept client connection");

    /* Test message exchange */
    generate_random_message(&send_msg, 1);
    cr_assert(send_test_message(client_fd, &send_msg) == 0,
        "Failed to send message from client");

    cr_assert(receive_test_message(accepted_fd, &recv_msg) == 0,
        "Failed to receive message on server");

    cr_assert(recv_msg.id == send_msg.id, "Message ID mismatch");
    cr_assert(strcmp(recv_msg.data, send_msg.data) == 0,
        "Message data mismatch");

    close(accepted_fd);
}

/* Test multiple client connections */
Test(integration, multiple_clients, .init = setup, .fini = teardown)
{
    int server_fd;
    int client_fds[5];
    int accepted_fds[5];
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);
    test_message_t messages[5];

    server_fd = create_test_server(TEST_SERVER_PORT + 1);
    cr_assert(server_fd >= 0, "Failed to create test server");

    /* Create multiple clients */
    for (int i = 0; i < 5; i++) {
        usleep(50000);
        client_fds[i] = create_test_client("127.0.0.1", TEST_SERVER_PORT + 1);
        cr_assert(client_fds[i] >= 0, "Failed to create client %d", i);

        accepted_fds[i] = accept(server_fd, (struct sockaddr *)&addr,
            &addr_len);
        cr_assert(accepted_fds[i] >= 0,
            "Failed to accept client %d connection", i);
    }

    /* Test concurrent message exchange */
    for (int i = 0; i < 5; i++) {
        generate_random_message(&messages[i], i + 1);
        cr_assert(send_test_message(client_fds[i], &messages[i]) == 0,
            "Failed to send message from client %d", i);
    }

    for (int i = 0; i < 5; i++) {
        test_message_t recv_msg;
        cr_assert(receive_test_message(accepted_fds[i], &recv_msg) == 0,
            "Failed to receive message from client %d", i);

        cr_assert(recv_msg.id == messages[i].id,
            "Message ID mismatch for client %d", i);
        close(accepted_fds[i]);
    }
}

/* Test ring buffer integration with network I/O */
Test(integration, ringbuf_network_io, .init = setup, .fini = teardown)
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

/* Test connection error handling */
Test(integration, connection_error_handling, .init = setup, .fini = teardown)
{
    int client_fd;
    zn_socket_t *socket;

    /* Test connection to non-existent server */
    client_fd = create_test_client("127.0.0.1", 65535);
    cr_assert(client_fd < 0, "Should fail to connect to non-existent server");

    /* Test invalid hostname */
    socket = zn_socket_create();
    cr_assert(socket != NULL, "Failed to create socket");

    cr_assert(zn_socket_connect(socket, "invalid.hostname.test", 8080) != 0,
        "Should fail to connect to invalid hostname");

    zn_socket_destroy(socket);
}

/* Test graceful disconnection */
Test(integration, graceful_disconnection, .init = setup, .fini = teardown)
{
    int server_fd, client_fd, accepted_fd;
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);
    test_message_t msg;

    server_fd = create_test_server(TEST_SERVER_PORT + 2);
    cr_assert(server_fd >= 0, "Failed to create test server");

    usleep(100000);
    client_fd = create_test_client("127.0.0.1", TEST_SERVER_PORT + 2);
    cr_assert(client_fd >= 0, "Failed to create test client");

    accepted_fd = accept(server_fd, (struct sockaddr *)&addr, &addr_len);
    cr_assert(accepted_fd >= 0, "Failed to accept client connection");

    /* Send a message then close client */
    generate_random_message(&msg, 999);
    cr_assert(send_test_message(client_fd, &msg) == 0,
        "Failed to send message before disconnect");

    close(client_fd);

    /* Server should detect disconnection */
    test_message_t recv_msg;
    cr_assert(receive_test_message(accepted_fd, &recv_msg) == 0,
        "Failed to receive message before disconnect");

    /* Next read should return 0 (EOF) or -1 (error) */
    ssize_t result = read(accepted_fd, &recv_msg, sizeof(recv_msg));
    cr_assert(result <= 0, "Should detect client disconnection");

    close(accepted_fd);
}
