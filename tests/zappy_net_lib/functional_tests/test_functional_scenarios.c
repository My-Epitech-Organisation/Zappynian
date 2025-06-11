/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Real-world scenario functional tests for libzappy_net
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

/* Test game server simulation */
Test(scenarios, game_server_simulation, .init = setup, .fini = teardown,
    .timeout = 20)
{
    int server_fd;
    int player_clients[5];
    int accepted_fds[5];
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);
    
    server_fd = create_test_server(TEST_SERVER_PORT + 60);
    cr_assert(server_fd >= 0, "Failed to create game server");
    
    /* Simulate 5 players connecting */
    for (int i = 0; i < 5; i++) {
        usleep(100000);
        player_clients[i] = create_test_client("127.0.0.1",
            TEST_SERVER_PORT + 60);
        cr_assert(player_clients[i] >= 0, "Player %d failed to connect", i);
        
        accepted_fds[i] = accept(server_fd, (struct sockaddr *)&addr,
            &addr_len);
        cr_assert(accepted_fds[i] >= 0,
            "Failed to accept player %d connection", i);
    }
    
    /* Simulate game commands sequence */
    const char *game_commands[] = {
        "Forward\n",
        "Look\n",
        "Inventory\n",
        "Take food\n",
        "Right\n",
        "Forward\n",
        "Look\n"
    };
    
    int cmd_count = sizeof(game_commands) / sizeof(game_commands[0]);
    
    /* Each player sends a sequence of commands */
    for (int player = 0; player < 5; player++) {
        for (int cmd = 0; cmd < cmd_count; cmd++) {
            size_t cmd_len = strlen(game_commands[cmd]);
            char recv_buffer[256];
            
            /* Player sends command */
            ssize_t sent = write(player_clients[player], game_commands[cmd],
                cmd_len);
            cr_assert(sent == (ssize_t)cmd_len,
                "Player %d failed to send command: %s",
                player, game_commands[cmd]);
            
            /* Server receives command */
            ssize_t received = read(accepted_fds[player], recv_buffer,
                sizeof(recv_buffer));
            cr_assert(received == (ssize_t)cmd_len,
                "Server failed to receive command from player %d", player);
            
            recv_buffer[received] = '\0';
            cr_assert(strcmp(recv_buffer, game_commands[cmd]) == 0,
                "Command mismatch for player %d: '%s' vs '%s'",
                player, game_commands[cmd], recv_buffer);
            
            /* Server sends response */
            const char *response = "ok\n";
            sent = write(accepted_fds[player], response, strlen(response));
            cr_assert(sent == (ssize_t)strlen(response),
                "Server failed to send response to player %d", player);
            
            /* Player receives response */
            received = read(player_clients[player], recv_buffer,
                sizeof(recv_buffer));
            cr_assert(received == (ssize_t)strlen(response),
                "Player %d failed to receive response", player);
        }
        
        close(accepted_fds[player]);
    }
}

/* Test broadcast messaging scenario */
Test(scenarios, broadcast_messaging, .init = setup, .fini = teardown,
    .timeout = 15)
{
    int server_fd;
    int clients[3];
    int accepted_fds[3];
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);
    
    server_fd = create_test_server(TEST_SERVER_PORT + 61);
    cr_assert(server_fd >= 0, "Failed to create server");
    
    /* Connect 3 clients */
    for (int i = 0; i < 3; i++) {
        usleep(50000);
        clients[i] = create_test_client("127.0.0.1", TEST_SERVER_PORT + 61);
        cr_assert(clients[i] >= 0, "Client %d failed to connect", i);
        
        accepted_fds[i] = accept(server_fd, (struct sockaddr *)&addr,
            &addr_len);
        cr_assert(accepted_fds[i] >= 0,
            "Failed to accept client %d connection", i);
    }
    
    /* Client 0 sends a broadcast message */
    const char *broadcast_cmd = "Broadcast Hello everyone!\n";
    ssize_t sent = write(clients[0], broadcast_cmd, strlen(broadcast_cmd));
    cr_assert(sent == (ssize_t)strlen(broadcast_cmd),
        "Failed to send broadcast command");
    
    /* Server receives broadcast command */
    char recv_buffer[256];
    ssize_t received = read(accepted_fds[0], recv_buffer, sizeof(recv_buffer));
    cr_assert(received == (ssize_t)strlen(broadcast_cmd),
        "Server failed to receive broadcast command");
    
    recv_buffer[received] = '\0';
    cr_assert(strcmp(recv_buffer, broadcast_cmd) == 0,
        "Broadcast command mismatch");
    
    /* Server broadcasts message to all other clients */
    const char *broadcast_msg = "message 0 Hello everyone!\n";
    for (int i = 1; i < 3; i++) {
        sent = write(accepted_fds[i], broadcast_msg, strlen(broadcast_msg));
        cr_assert(sent == (ssize_t)strlen(broadcast_msg),
            "Failed to broadcast to client %d", i);
        
        /* Client receives broadcast */
        received = read(clients[i], recv_buffer, sizeof(recv_buffer));
        cr_assert(received == (ssize_t)strlen(broadcast_msg),
            "Client %d failed to receive broadcast", i);
        
        recv_buffer[received] = '\0';
        cr_assert(strcmp(recv_buffer, broadcast_msg) == 0,
            "Broadcast message mismatch for client %d", i);
    }
    
    for (int i = 0; i < 3; i++) {
        close(accepted_fds[i]);
    }
}

/* Test team coordination scenario */
Test(scenarios, team_coordination, .init = setup, .fini = teardown,
    .timeout = 20)
{
    int server_fd;
    int team_clients[4];
    int accepted_fds[4];
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);
    zn_ringbuf_t message_buffer;
    
    /* Initialize message buffer for queuing */
    cr_assert(zn_ringbuf_init(&message_buffer, 1024) == 0,
        "Failed to initialize message buffer");
    
    server_fd = create_test_server(TEST_SERVER_PORT + 62);
    cr_assert(server_fd >= 0, "Failed to create server");
    
    /* Connect team members */
    for (int i = 0; i < 4; i++) {
        usleep(75000);
        team_clients[i] = create_test_client("127.0.0.1",
            TEST_SERVER_PORT + 62);
        cr_assert(team_clients[i] >= 0, "Team member %d failed to connect", i);
        
        accepted_fds[i] = accept(server_fd, (struct sockaddr *)&addr,
            &addr_len);
        cr_assert(accepted_fds[i] >= 0,
            "Failed to accept team member %d connection", i);
    }
    
    /* Simulate team coordination commands */
    struct {
        int sender;
        const char *command;
        const char *response;
    } coordination_sequence[] = {
        {0, "Connect_nbr\n", "3\n"},
        {1, "Fork\n", "ok\n"},
        {0, "Broadcast Ready for incantation\n", "ok\n"},
        {2, "Inventory\n", "[ food 10, linemate 1, deraumere 0, sibur 0, mendiane 0, phiras 0, thystame 0 ]\n"},
        {3, "Take linemate\n", "ok\n"},
        {0, "Incantation\n", "Elevation underway\n"}
    };
    
    int seq_count = sizeof(coordination_sequence) /
        sizeof(coordination_sequence[0]);
    
    for (int i = 0; i < seq_count; i++) {
        int sender = coordination_sequence[i].sender;
        const char *cmd = coordination_sequence[i].command;
        const char *resp = coordination_sequence[i].response;
        
        size_t cmd_len = strlen(cmd);
        size_t resp_len = strlen(resp);
        char recv_buffer[512];
        
        /* Team member sends command */
        ssize_t sent = write(team_clients[sender], cmd, cmd_len);
        cr_assert(sent == (ssize_t)cmd_len,
            "Team member %d failed to send: %s", sender, cmd);
        
        /* Store command in message buffer */
        ssize_t buffered = zn_ringbuf_write(&message_buffer, cmd, cmd_len);
        cr_assert(buffered == (ssize_t)cmd_len,
            "Failed to buffer command from team member %d", sender);
        
        /* Server receives command */
        ssize_t received = read(accepted_fds[sender], recv_buffer,
            sizeof(recv_buffer));
        cr_assert(received == (ssize_t)cmd_len,
            "Server failed to receive command from team member %d", sender);
        
        /* Verify command from buffer */
        char buffer_cmd[256];
        ssize_t read_from_buffer = zn_ringbuf_read_line(&message_buffer,
            buffer_cmd, sizeof(buffer_cmd));
        cr_assert(read_from_buffer == (ssize_t)cmd_len,
            "Failed to read command from buffer");
        cr_assert(strcmp(buffer_cmd, cmd) == 0,
            "Buffered command mismatch");
        
        /* Server sends response */
        sent = write(accepted_fds[sender], resp, resp_len);
        cr_assert(sent == (ssize_t)resp_len,
            "Server failed to send response to team member %d", sender);
        
        /* Team member receives response */
        received = read(team_clients[sender], recv_buffer,
            sizeof(recv_buffer));
        cr_assert(received == (ssize_t)resp_len,
            "Team member %d failed to receive response", sender);
        
        recv_buffer[received] = '\0';
        cr_assert(strcmp(recv_buffer, resp) == 0,
            "Response mismatch for team member %d", sender);
    }
    
    for (int i = 0; i < 4; i++) {
        close(accepted_fds[i]);
    }
    
    zn_ringbuf_cleanup(&message_buffer);
}

/* Test client reconnection scenario */
Test(scenarios, client_reconnection, .init = setup, .fini = teardown,
    .timeout = 15)
{
    int server_fd, client_fd, accepted_fd;
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);
    
    server_fd = create_test_server(TEST_SERVER_PORT + 63);
    cr_assert(server_fd >= 0, "Failed to create server");
    
    /* Initial connection */
    usleep(100000);
    client_fd = create_test_client("127.0.0.1", TEST_SERVER_PORT + 63);
    cr_assert(client_fd >= 0, "Initial client connection failed");
    
    accepted_fd = accept(server_fd, (struct sockaddr *)&addr, &addr_len);
    cr_assert(accepted_fd >= 0, "Failed to accept initial connection");
    
    /* Send a command */
    const char *cmd1 = "Look\n";
    ssize_t sent = write(client_fd, cmd1, strlen(cmd1));
    cr_assert(sent == (ssize_t)strlen(cmd1), "Failed to send initial command");
    
    char recv_buffer[256];
    ssize_t received = read(accepted_fd, recv_buffer, sizeof(recv_buffer));
    cr_assert(received == (ssize_t)strlen(cmd1),
        "Failed to receive initial command");
    
    /* Simulate client disconnection */
    close(client_fd);
    close(accepted_fd);
    
    usleep(200000); /* Wait a bit */
    
    /* Client reconnects */
    client_fd = create_test_client("127.0.0.1", TEST_SERVER_PORT + 63);
    cr_assert(client_fd >= 0, "Client reconnection failed");
    
    accepted_fd = accept(server_fd, (struct sockaddr *)&addr, &addr_len);
    cr_assert(accepted_fd >= 0, "Failed to accept reconnection");
    
    /* Send another command after reconnection */
    const char *cmd2 = "Inventory\n";
    sent = write(client_fd, cmd2, strlen(cmd2));
    cr_assert(sent == (ssize_t)strlen(cmd2),
        "Failed to send command after reconnection");
    
    received = read(accepted_fd, recv_buffer, sizeof(recv_buffer));
    cr_assert(received == (ssize_t)strlen(cmd2),
        "Failed to receive command after reconnection");
    
    recv_buffer[received] = '\0';
    cr_assert(strcmp(recv_buffer, cmd2) == 0,
        "Command mismatch after reconnection");
    
    close(accepted_fd);
}

/* Test data persistence scenario */
Test(scenarios, data_persistence, .init = setup, .fini = teardown)
{
    zn_ringbuf_t persistent_buffer;
    
    cr_assert(zn_ringbuf_init(&persistent_buffer, 512) == 0,
        "Failed to initialize persistent buffer");
    
    /* Simulate game state data being stored */
    const char *game_state[] = {
        "player_1_pos 5 3\n",
        "player_1_inventory food 5 linemate 2\n",
        "player_2_pos 7 8\n",
        "player_2_inventory food 3 deraumere 1\n",
        "map_tile 5 3 food linemate\n",
        "map_tile 7 8 sibur phiras\n"
    };
    
    int state_count = sizeof(game_state) / sizeof(game_state[0]);
    
    /* Store all game state */
    for (int i = 0; i < state_count; i++) {
        ssize_t written = zn_ringbuf_write(&persistent_buffer, game_state[i],
            strlen(game_state[i]));
        cr_assert(written == (ssize_t)strlen(game_state[i]),
            "Failed to store game state entry %d", i);
    }
    
    /* Verify all data can be retrieved */
    for (int i = 0; i < state_count; i++) {
        char read_buffer[128];
        ssize_t read_bytes = zn_ringbuf_read_line(&persistent_buffer,
            read_buffer, sizeof(read_buffer));
        
        cr_assert(read_bytes == (ssize_t)strlen(game_state[i]),
            "Failed to read game state entry %d", i);
        cr_assert(strcmp(read_buffer, game_state[i]) == 0,
            "Game state data mismatch for entry %d", i);
    }
    
    /* Verify buffer is now empty */
    char final_buffer[128];
    ssize_t final_read = zn_ringbuf_read_line(&persistent_buffer,
        final_buffer, sizeof(final_buffer));
    cr_assert(final_read == -1, "Buffer should be empty after reading all data");
    
    zn_ringbuf_cleanup(&persistent_buffer);
}
