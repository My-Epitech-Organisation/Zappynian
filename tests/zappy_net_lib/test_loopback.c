/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Test loopback server-client communication
*/

#define _POSIX_C_SOURCE 200809L
#include <criterion/criterion.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <poll.h>
#include <string.h>
#include "../../libzappy_net/include/zappy_net.h"
#include "test_helpers.h"

static void *server_thread(void *arg)
{
    zn_socket_t server = (zn_socket_t)arg;
    struct pollfd pfd;
    int client_fd;
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    char buffer[1024];
    ssize_t received;

    pfd.fd = test_get_socket_fd(server);
    pfd.events = POLLIN;

    if (poll(&pfd, 1, 5000) > 0 && (pfd.revents & POLLIN)) {
        client_fd = accept(pfd.fd, (struct sockaddr *)&client_addr,
            &client_len);
        if (client_fd >= 0) {
            received = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
            if (received > 0) {
                buffer[received] = '\0';
                send(client_fd, "PONG", 4, 0);
            }
            close(client_fd);
        }
    }
    return NULL;
}

Test(loopback, test_server_client_communication)
{
    zn_socket_t server;
    zn_socket_t client;
    pthread_t server_tid;
    struct pollfd pfd;
    char buffer[1024];
    ssize_t sent, received;

    server = zn_server_listen(8082);
    cr_assert_not_null(server, "Server should start successfully");

    pthread_create(&server_tid, NULL, server_thread, server);

    sleep(1);

    client = zn_client_connect("localhost", 8082);
    cr_assert_not_null(client, "Client should connect successfully");

    pfd.fd = test_get_socket_fd(client);
    pfd.events = POLLOUT;

    if (poll(&pfd, 1, 1000) > 0 && (pfd.revents & POLLOUT)) {
        sent = send(pfd.fd, "PING", 4, 0);
        cr_assert_eq(sent, 4, "Should send 4 bytes");

        pfd.events = POLLIN;
        if (poll(&pfd, 1, 1000) > 0 && (pfd.revents & POLLIN)) {
            received = recv(pfd.fd, buffer, sizeof(buffer) - 1, 0);
            if (received > 0) {
                buffer[received] = '\0';
                cr_assert_str_eq(buffer, "PONG", "Should receive PONG");
            }
        }
    }

    pthread_join(server_tid, NULL);
    zn_close(client);
    zn_close(server);
}
