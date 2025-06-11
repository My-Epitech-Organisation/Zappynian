/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Protocol functional tests for libzappy_net
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

/* Test Zappy protocol message format */
Test(protocol, zappy_message_format, .init = setup, .fini = teardown)
{
    int server_fd, client_fd, accepted_fd;
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);
    
    server_fd = create_test_server(TEST_SERVER_PORT + 20);
    cr_assert(server_fd >= 0, "Failed to create test server");
    
    usleep(100000);
    client_fd = create_test_client("127.0.0.1", TEST_SERVER_PORT + 20);
    cr_assert(client_fd >= 0, "Failed to create test client");
    
    accepted_fd = accept(server_fd, (struct sockaddr *)&addr, &addr_len);
    cr_assert(accepted_fd >= 0, "Failed to accept client connection");
    
    /* Test various Zappy protocol messages */
    const char *test_commands[] = {
        "Forward\n",
        "Right\n",
        "Left\n",
        "Look\n",
        "Inventory\n",
        "Broadcast hello\n",
        "Connect_nbr\n",
        "Fork\n",
        "Eject\n",
        "Take food\n",
        "Set linemate\n",
        "Incantation\n"
    };
    
    int command_count = sizeof(test_commands) / sizeof(test_commands[0]);
    
    for (int i = 0; i < command_count; i++) {
        size_t cmd_len = strlen(test_commands[i]);
        char recv_buffer[256];
        
        /* Send command from client */
        ssize_t sent = write(client_fd, test_commands[i], cmd_len);
        cr_assert(sent == (ssize_t)cmd_len,
            "Failed to send command: %s", test_commands[i]);
        
        /* Receive on server */
        ssize_t received = read(accepted_fd, recv_buffer, sizeof(recv_buffer));
        cr_assert(received == (ssize_t)cmd_len,
            "Failed to receive full command: %s", test_commands[i]);
        
        recv_buffer[received] = '\0';
        cr_assert(strcmp(recv_buffer, test_commands[i]) == 0,
            "Command mismatch: expected '%s', got '%s'",
            test_commands[i], recv_buffer);
    }
    
    close(accepted_fd);
}

/* Test protocol response handling */
Test(protocol, response_handling, .init = setup, .fini = teardown)
{
    int server_fd, client_fd, accepted_fd;
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);
    
    server_fd = create_test_server(TEST_SERVER_PORT + 21);
    cr_assert(server_fd >= 0, "Failed to create test server");
    
    usleep(100000);
    client_fd = create_test_client("127.0.0.1", TEST_SERVER_PORT + 21);
    cr_assert(client_fd >= 0, "Failed to create test client");
    
    accepted_fd = accept(server_fd, (struct sockaddr *)&addr, &addr_len);
    cr_assert(accepted_fd >= 0, "Failed to accept client connection");
    
    /* Test command-response pairs */
    struct {
        const char *command;
        const char *response;
    } test_pairs[] = {
        {"Look\n", "[ player, food, , , , , , , ]\n"},
        {"Inventory\n", "[ food 10, linemate 0, deraumere 0, sibur 0, mendiane 0, phiras 0, thystame 0 ]\n"},
        {"Connect_nbr\n", "5\n"},
        {"Forward\n", "ok\n"},
        {"Right\n", "ok\n"},
        {"Left\n", "ok\n"},
        {"Take food\n", "ok\n"},
        {"Invalid command\n", "ko\n"}
    };
    
    int pair_count = sizeof(test_pairs) / sizeof(test_pairs[0]);
    
    for (int i = 0; i < pair_count; i++) {
        char recv_buffer[512];
        size_t cmd_len = strlen(test_pairs[i].command);
        size_t resp_len = strlen(test_pairs[i].response);
        
        /* Client sends command */
        ssize_t sent = write(client_fd, test_pairs[i].command, cmd_len);
        cr_assert(sent == (ssize_t)cmd_len,
            "Failed to send command: %s", test_pairs[i].command);
        
        /* Server receives command */
        ssize_t received = read(accepted_fd, recv_buffer, sizeof(recv_buffer));
        cr_assert(received == (ssize_t)cmd_len,
            "Failed to receive command: %s", test_pairs[i].command);
        
        /* Server sends response */
        sent = write(accepted_fd, test_pairs[i].response, resp_len);
        cr_assert(sent == (ssize_t)resp_len,
            "Failed to send response: %s", test_pairs[i].response);
        
        /* Client receives response */
        received = read(client_fd, recv_buffer, sizeof(recv_buffer));
        cr_assert(received == (ssize_t)resp_len,
            "Failed to receive response: %s", test_pairs[i].response);
        
        recv_buffer[received] = '\0';
        cr_assert(strcmp(recv_buffer, test_pairs[i].response) == 0,
            "Response mismatch for command '%s': expected '%s', got '%s'",
            test_pairs[i].command, test_pairs[i].response, recv_buffer);
    }
    
    close(accepted_fd);
}

/* Test line-based protocol parsing with ring buffer */
Test(protocol, line_based_parsing, .init = setup, .fini = teardown)
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

/* Test protocol timeout handling */
Test(protocol, timeout_handling, .init = setup, .fini = teardown)
{
    int server_fd, client_fd, accepted_fd;
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);
    fd_set readfds;
    struct timeval timeout;
    
    server_fd = create_test_server(TEST_SERVER_PORT + 22);
    cr_assert(server_fd >= 0, "Failed to create test server");
    
    usleep(100000);
    client_fd = create_test_client("127.0.0.1", TEST_SERVER_PORT + 22);
    cr_assert(client_fd >= 0, "Failed to create test client");
    
    accepted_fd = accept(server_fd, (struct sockaddr *)&addr, &addr_len);
    cr_assert(accepted_fd >= 0, "Failed to accept client connection");
    
    /* Test read timeout on server side */
    FD_ZERO(&readfds);
    FD_SET(accepted_fd, &readfds);
    
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;
    
    /* Should timeout since client doesn't send anything */
    int select_result = select(accepted_fd + 1, &readfds, NULL, NULL,
        &timeout);
    cr_assert(select_result == 0, "Should timeout waiting for client data");
    
    /* Now send data and verify it arrives */
    const char *test_msg = "Test message\n";
    ssize_t sent = write(client_fd, test_msg, strlen(test_msg));
    cr_assert(sent == (ssize_t)strlen(test_msg),
        "Failed to send test message");
    
    FD_ZERO(&readfds);
    FD_SET(accepted_fd, &readfds);
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;
    
    select_result = select(accepted_fd + 1, &readfds, NULL, NULL, &timeout);
    cr_assert(select_result == 1, "Should detect incoming data");
    cr_assert(FD_ISSET(accepted_fd, &readfds),
        "Server socket should be ready for reading");
    
    close(accepted_fd);
}

/* Test protocol error recovery */
Test(protocol, error_recovery, .init = setup, .fini = teardown)
{
    int server_fd, client_fd, accepted_fd;
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);
    
    server_fd = create_test_server(TEST_SERVER_PORT + 23);
    cr_assert(server_fd >= 0, "Failed to create test server");
    
    usleep(100000);
    client_fd = create_test_client("127.0.0.1", TEST_SERVER_PORT + 23);
    cr_assert(client_fd >= 0, "Failed to create test client");
    
    accepted_fd = accept(server_fd, (struct sockaddr *)&addr, &addr_len);
    cr_assert(accepted_fd >= 0, "Failed to accept client connection");
    
    /* Send invalid commands followed by valid ones */
    const char *commands[] = {
        "InvalidCommand\n",
        "AnotherInvalid\n",
        "Look\n",            /* Valid command */
        "Inventory\n",       /* Valid command */
        "BadCommand\n",
        "Forward\n"          /* Valid command */
    };
    
    int command_count = sizeof(commands) / sizeof(commands[0]);
    char recv_buffer[256];
    
    for (int i = 0; i < command_count; i++) {
        size_t cmd_len = strlen(commands[i]);
        
        /* Send command */
        ssize_t sent = write(client_fd, commands[i], cmd_len);
        cr_assert(sent == (ssize_t)cmd_len,
            "Failed to send command: %s", commands[i]);
        
        /* Receive command on server */
        ssize_t received = read(accepted_fd, recv_buffer, sizeof(recv_buffer));
        cr_assert(received == (ssize_t)cmd_len,
            "Failed to receive command: %s", commands[i]);
        
        recv_buffer[received] = '\0';
        cr_assert(strcmp(recv_buffer, commands[i]) == 0,
            "Command mismatch: expected '%s', got '%s'",
            commands[i], recv_buffer);
        
        /* Send appropriate response */
        const char *response = (strstr(commands[i], "Invalid") ||
                               strstr(commands[i], "Bad")) ? "ko\n" : "ok\n";
        sent = write(accepted_fd, response, strlen(response));
        cr_assert(sent == (ssize_t)strlen(response),
            "Failed to send response");
    }
    
    close(accepted_fd);
}
