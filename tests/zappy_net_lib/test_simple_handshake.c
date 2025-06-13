/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Simple integration test for handshake
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include "zappy_net.h"

TestSuite(simple_handshake);

Test(simple_handshake, mock_server_test)
{
    int server_fd = 0;
    int client_fd = 0;
    struct sockaddr_in addr = {0};
    char buffer[256] = {0};
    ssize_t bytes_read = 0;

    cr_assert_eq(zn_init(), 0, "Library initialization failed");
    
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    cr_assert_neq(server_fd, -1, "Server socket creation failed");
    
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(4444);
    
    cr_assert_neq(bind(server_fd, (struct sockaddr *)&addr, sizeof(addr)), -1,
        "Server bind failed");
    cr_assert_neq(listen(server_fd, 1), -1, "Server listen failed");
    
    if (fork() == 0) {
        close(server_fd);
        sleep(1);
        
        zn_socket_t client = zn_client_connect("127.0.0.1", 4444);
        if (client != NULL) {
            zn_handshake_result_t result = {0};
            int ret = zn_perform_handshake(client, ZN_ROLE_AI, "team1", &result);
            zn_socket_destroy(client);
            exit(ret == 0 ? 0 : 1);
        }
        exit(1);
    } else {
        socklen_t addr_len = sizeof(addr);
        client_fd = accept(server_fd, (struct sockaddr *)&addr, &addr_len);
        cr_assert_neq(client_fd, -1, "Accept failed");
        
        cr_assert_neq(send(client_fd, "WELCOME\n", 8, 0), -1, "Send WELCOME failed");
        
        bytes_read = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
        cr_assert_gt(bytes_read, 0, "Recv team name failed");
        buffer[bytes_read] = '\0';
        
        cr_assert_neq(send(client_fd, "5\n", 2, 0), -1, "Send client num failed");
        cr_assert_neq(send(client_fd, "10 10\n", 6, 0), -1, "Send world size failed");
        
        close(client_fd);
        close(server_fd);
        int status;
        wait(&status);
        cr_assert_eq(WEXITSTATUS(status), 0, "Child process failed");
    }
    
    zn_cleanup();
}
