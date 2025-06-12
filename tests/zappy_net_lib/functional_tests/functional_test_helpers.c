/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Functional test helpers implementation
*/

#include "functional_test_helpers.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>

static int test_sockets[256];
static int socket_count = 0;

void setup_test_environment(void)
{
    socket_count = 0;
    memset(test_sockets, -1, sizeof(test_sockets));
    srand(time(NULL));
}

void teardown_test_environment(void)
{
    for (int i = 0; i < socket_count; i++) {
        if (test_sockets[i] != -1) {
            close(test_sockets[i]);
            test_sockets[i] = -1;
        }
    }
    socket_count = 0;
}

int create_test_server(int port)
{
    int server_fd;
    struct sockaddr_in addr;
    int opt = 1;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0)
        return -1;

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        close(server_fd);
        return -1;
    }

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        close(server_fd);
        return -1;
    }

    if (listen(server_fd, 5) < 0) {
        close(server_fd);
        return -1;
    }

    if (socket_count < 256) {
        test_sockets[socket_count++] = server_fd;
    }
    return server_fd;
}

int create_test_client(const char *host, int port)
{
    int client_fd;
    struct sockaddr_in addr;

    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd < 0)
        return -1;

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);

    if (inet_pton(AF_INET, host, &addr.sin_addr) <= 0) {
        close(client_fd);
        return -1;
    }

    if (connect(client_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        close(client_fd);
        return -1;
    }

    if (socket_count < 256) {
        test_sockets[socket_count++] = client_fd;
    }
    return client_fd;
}

void generate_random_message(test_message_t *msg, int id)
{
    msg->id = id;
    msg->size = rand() % 200 + 50;

    for (size_t i = 0; i < msg->size && i < 255; i++) {
        msg->data[i] = 'A' + (rand() % 26);
    }
    msg->data[msg->size] = '\0';
}

int send_test_message(int socket, const test_message_t *msg)
{
    ssize_t sent = write(socket, msg, sizeof(test_message_t));
    return (sent == sizeof(test_message_t)) ? 0 : -1;
}

int receive_test_message(int socket, test_message_t *msg)
{
    ssize_t received = read(socket, msg, sizeof(test_message_t));
    return (received == sizeof(test_message_t)) ? 0 : -1;
}

double get_time_diff_ms(struct timespec start, struct timespec end)
{
    return (end.tv_sec - start.tv_sec) * 1000.0 +
           (end.tv_nsec - start.tv_nsec) / 1000000.0;
}

int wait_for_connection(int server_socket, double timeout_sec)
{
    fd_set readfds;
    struct timeval timeout;

    FD_ZERO(&readfds);
    FD_SET(server_socket, &readfds);

    timeout.tv_sec = (int)timeout_sec;
    timeout.tv_usec = ((int)(timeout_sec * 1000000)) % 1000000;

    return select(server_socket + 1, &readfds, NULL, NULL, &timeout);
}

void init_performance_stats(performance_stats_t *stats)
{
    stats->min_latency = 1000000.0;
    stats->max_latency = 0.0;
    stats->avg_latency = 0.0;
    stats->total_time = 0.0;
    stats->success_count = 0;
    stats->error_count = 0;
}

void update_performance_stats(performance_stats_t *stats, double latency)
{
    stats->success_count++;
    stats->total_time += latency;

    if (latency < stats->min_latency)
        stats->min_latency = latency;
    if (latency > stats->max_latency)
        stats->max_latency = latency;
}

void finalize_performance_stats(performance_stats_t *stats)
{
    if (stats->success_count > 0) {
        stats->avg_latency = stats->total_time / stats->success_count;
    }
}

void print_performance_stats(const performance_stats_t *stats,
    const char *test_name)
{
    printf("\n=== Performance Stats for %s ===\n", test_name);
    printf("Success count: %d\n", stats->success_count);
    printf("Error count: %d\n", stats->error_count);
    printf("Min latency: %.2f ms\n", stats->min_latency);
    printf("Max latency: %.2f ms\n", stats->max_latency);
    printf("Avg latency: %.2f ms\n", stats->avg_latency);
    printf("Total time: %.2f ms\n", stats->total_time);
    printf("================================\n\n");
}
